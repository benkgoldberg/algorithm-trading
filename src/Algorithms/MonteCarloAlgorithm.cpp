#include "MonteCarloAlgorithm.h"
#include "Trades/SimulatedExecution.h"
#include "Data/LiveData.h"
#include <iostream>
#include <thread>
#include <numeric>
#include <cmath>

MonteCarloAlgorithm::MonteCarloAlgorithm(SimulatedExecution& executor, LiveData& dataFeed, 
                                         int numSimulations, int timeHorizon, 
                                         double buyThreshold, double sellThreshold)
    : HFTAlgorithm(executor, dataFeed), 
      m_numSimulations(numSimulations), m_timeHorizon(timeHorizon),
      m_buyThreshold(buyThreshold), m_sellThreshold(sellThreshold),
      m_inPosition(false), m_generator(std::random_device{}()) {}

void MonteCarloAlgorithm::processData(const TickData& data) {
    updateHistoricalReturns(data);
    if (m_historicalReturns.size() >= 30) { // Ensure we have enough historical data
        makeDecision(data);
    }
}

void MonteCarloAlgorithm::run() {
    m_running = true;
    std::chrono::system_clock::time_point lastProcessedTime{};
    
    std::cout << "Starting Monte Carlo Algorithm..." << std::endl;
    
    while (m_running) {
        try {
            auto data = m_dataFeed.getLatestTickData();
            if (data.timestamp > lastProcessedTime) {
                std::cout << "Processing new data: ";
                m_dataFeed.printLatestTickData();
                processData(data);
                lastProcessedTime = data.timestamp;
            } else {
                std::cout << "Waiting for new data..." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error in MonteCarloAlgorithm::run(): " << e.what() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    std::cout << "Monte Carlo Algorithm stopped." << std::endl;
}

void MonteCarloAlgorithm::updateHistoricalReturns(const TickData& data) {
    if (!m_historicalReturns.empty()) {
        double return_ = (data.price / m_historicalReturns.back()) - 1.0;
        m_historicalReturns.push_back(return_);
        if (m_historicalReturns.size() > MAX_WINDOW_SIZE) {  // Keep a moving window of returns
            m_historicalReturns.erase(m_historicalReturns.begin());
        }
    } else {
        m_historicalReturns.push_back(data.price);
    }
}

std::vector<double> MonteCarloAlgorithm::simulatePricePath() {
    std::vector<double> pricePath(m_timeHorizon);
    pricePath[0] = m_historicalReturns.back();

    double mean = std::accumulate(m_historicalReturns.begin(), m_historicalReturns.end(), 0.0) / m_historicalReturns.size();
    double variance = std::accumulate(m_historicalReturns.begin(), m_historicalReturns.end(), 0.0,
        [mean](double acc, double x) { return acc + std::pow(x - mean, 2); }) / (m_historicalReturns.size() - 1);
    double stddev = std::sqrt(variance);

    std::normal_distribution<> distribution(mean, stddev);

    for (int i = 1; i < m_timeHorizon; ++i) {
        double return_ = distribution(m_generator);
        pricePath[i] = pricePath[i-1] * (1 + return_);
    }

    return pricePath;
}

void MonteCarloAlgorithm::makeDecision(const TickData& data) {
    int positiveReturns = 0;

    for (int i = 0; i < m_numSimulations; ++i) {
        auto pricePath = simulatePricePath();
        if (pricePath.back() > data.price) {
            positiveReturns++;
        }
    }

    double probabilityOfIncrease = static_cast<double>(positiveReturns) / m_numSimulations;

    if (probabilityOfIncrease > m_buyThreshold && !m_inPosition) {
        // Buy signal
        Order buyOrder(data.symbol, OrderType::Market, OrderSide::Buy, 100);
        m_executor.placeOrder(buyOrder);
        m_inPosition = true;
        std::cout << "Monte Carlo Buy signal: Placed buy order at price " << data.price << std::endl;
    } else if (probabilityOfIncrease < m_sellThreshold && m_inPosition) {
        // Sell signal
        Order sellOrder(data.symbol, OrderType::Market, OrderSide::Sell, 100);
        m_executor.placeOrder(sellOrder);
        m_inPosition = false;
        std::cout << "Monte Carlo Sell signal: Placed sell order at price " << data.price << std::endl;
    }
}