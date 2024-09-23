#include "SimpleMovingAverageCrossover.h"
#include "Trades/SimulatedExecution.h"
#include "Data/LiveData.h"
#include "Data/TickData.h"
#include <numeric>
#include <thread>

SimpleMovingAverageCrossover::SimpleMovingAverageCrossover(SimulatedExecution& executor, LiveData& dataFeed, int shortPeriod, int longPeriod, double stopLossPercentage, double takeProfitPercentage)
    : HFTAlgorithm(executor, dataFeed, stopLossPercentage, takeProfitPercentage), m_shortPeriod(shortPeriod), m_longPeriod(longPeriod), m_shortMA(0), m_longMA(0), m_inPosition(false) {}

void SimpleMovingAverageCrossover::processData(const TickData& data) {
    updateMovingAverages(data.price);
    
    if (m_inPosition) {
        if (checkStopLossAndTakeProfit(data)) {
            m_inPosition = false;
            return;
        }
    }
    
    checkForCrossover(data);
}

void SimpleMovingAverageCrossover::run() {
    m_running = true;
    std::chrono::system_clock::time_point lastProcessedTime{};
    
    std::cout << "Starting SimpleMovingAverageCrossover algorithm..." << std::endl;
    
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
            std::cerr << "Error in SimpleMovingAverageCrossover::run(): " << e.what() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << "SimpleMovingAverageCrossover algorithm stopped." << std::endl;
}

void SimpleMovingAverageCrossover::updateMovingAverages(double price) {
    m_shortPeriodPrices.push_back(price);
    m_longPeriodPrices.push_back(price);

    if (m_shortPeriodPrices.size() > m_shortPeriod) {
        m_shortPeriodPrices.pop_front();
    }
    if (m_longPeriodPrices.size() > m_longPeriod) {
        m_longPeriodPrices.pop_front();
    }

    if (m_shortPeriodPrices.size() == m_shortPeriod) {
        m_shortMA = std::accumulate(m_shortPeriodPrices.begin(), m_shortPeriodPrices.end(), 0.0) / m_shortPeriod;
    }
    if (m_longPeriodPrices.size() == m_longPeriod) {
        m_longMA = std::accumulate(m_longPeriodPrices.begin(), m_longPeriodPrices.end(), 0.0) / m_longPeriod;
    }
}

void SimpleMovingAverageCrossover::checkForCrossover(const TickData& data) {
    if (m_shortPeriodPrices.size() < m_shortPeriod || m_longPeriodPrices.size() < m_longPeriod) {
        return;
    }

    if (m_shortMA > m_longMA && !m_inPosition) {
        // Buy signal
        Order buyOrder(data.symbol, OrderType::Market, OrderSide::Buy, 100);
        m_executor.placeOrder(buyOrder);
        m_inPosition = true;
        m_entryPrice = data.price;
    }
    else if (m_shortMA < m_longMA && m_inPosition) {
        // Sell signal
        Order sellOrder(data.symbol, OrderType::Market, OrderSide::Sell, 100);
        m_executor.placeOrder(sellOrder);
        m_inPosition = false;
    }
}