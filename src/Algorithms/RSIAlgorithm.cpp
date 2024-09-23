#include "RSIAlgorithm.h"
#include "Trades/SimulatedExecution.h"
#include "Data/LiveData.h"
#include <numeric>
#include <algorithm>
#include <iostream>
#include <thread>

RSIAlgorithm::RSIAlgorithm(SimulatedExecution& executor, LiveData& dataFeed, int period, double overbought, double oversold)
    : HFTAlgorithm(executor, dataFeed), m_period(period), m_overboughtThreshold(overbought), m_oversoldThreshold(oversold), m_lastRSI(0), m_inPosition(false) {}

void RSIAlgorithm::processData(const TickData& data) {
    if (m_prices.size() >= m_period) {
        m_prices.pop_front();
    }
    m_prices.push_back(data.price);

    if (m_prices.size() == m_period) {
        m_lastRSI = calculateRSI();
        checkForSignals(data);
    }
}

void RSIAlgorithm::run() {
    m_running = true;
    std::chrono::system_clock::time_point lastProcessedTime{};
    
    std::cout << "Starting RSI Algorithm..." << std::endl;
    
    while (m_running) {
        try {
            auto data = m_dataFeed.getLatestTickData();
            if (data.timestamp > lastProcessedTime) {
                std::cout << "Processing new data: ";
                m_dataFeed.printLatestTickData();  // Print the latest tick data
                processData(data);
                lastProcessedTime = data.timestamp;
            } else {
                std::cout << "Waiting for new data..." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error in RSIAlgorithm::run(): " << e.what() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << "RSI Algorithm stopped." << std::endl;
}

double RSIAlgorithm::calculateRSI() {
    double avgGain = 0, avgLoss = 0;
    for (size_t i = 1; i < m_prices.size(); ++i) {
        double diff = m_prices[i] - m_prices[i-1];
        if (diff > 0) avgGain += diff;
        else avgLoss -= diff;
    }
    avgGain /= m_period;
    avgLoss /= m_period;
    
    if (avgLoss == 0) return 100;
    double rs = avgGain / avgLoss;
    return 100 - (100 / (1 + rs));
}

void RSIAlgorithm::checkForSignals(const TickData& data) {
    if (m_lastRSI > m_overboughtThreshold && m_inPosition) {
        // Overbought condition - consider selling
        Order sellOrder(data.symbol, OrderType::Market, OrderSide::Sell, 100);
        m_executor.placeOrder(sellOrder);
        m_inPosition = false;
        std::cout << "RSI Overbought signal: Placed sell order at price " << data.price << std::endl;
    } else if (m_lastRSI < m_oversoldThreshold && !m_inPosition) {
        // Oversold condition - consider buying
        Order buyOrder(data.symbol, OrderType::Market, OrderSide::Buy, 100);
        m_executor.placeOrder(buyOrder);
        m_inPosition = true;
        std::cout << "RSI Oversold signal: Placed buy order at price " << data.price << std::endl;
    }
}