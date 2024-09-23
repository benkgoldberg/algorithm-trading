#include "KalmanFilterAlgorithm.h"
#include "Trades/SimulatedExecution.h"
#include "Data/LiveData.h"
#include <iostream>
#include <thread>

KalmanFilterAlgorithm::KalmanFilterAlgorithm(SimulatedExecution& executor, LiveData& dataFeed, 
                                             double processNoise, double measurementNoise)
    : HFTAlgorithm(executor, dataFeed), 
      m_processNoise(processNoise), m_measurementNoise(measurementNoise),
      m_estimatedPrice(0), m_estimatedError(1), m_inPosition(false) {}

void KalmanFilterAlgorithm::processData(const TickData& data) {
    updateEstimates(data.price);
    makeDecision(data);
}

void KalmanFilterAlgorithm::run() {
    m_running = true;
    std::chrono::system_clock::time_point lastProcessedTime{};
    
    std::cout << "Starting Kalman Filter Algorithm..." << std::endl;
    
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
            std::cerr << "Error in KalmanFilterAlgorithm::run(): " << e.what() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    std::cout << "Kalman Filter Algorithm stopped." << std::endl;
}

void KalmanFilterAlgorithm::updateEstimates(double price) {
    // Prediction update
    double predictedPrice = m_estimatedPrice;
    double predictedError = m_estimatedError + m_processNoise;

    // Measurement update
    double innovation = price - predictedPrice;
    double innovationCovariance = predictedError + m_measurementNoise;
    double kalmanGain = predictedError / innovationCovariance;

    m_estimatedPrice = predictedPrice + kalmanGain * innovation;
    m_estimatedError = (1 - kalmanGain) * predictedError;
}

void KalmanFilterAlgorithm::makeDecision(const TickData& data) {
    if (m_estimatedPrice > data.price && !m_inPosition) {
        // Buy signal
        Order buyOrder(data.symbol, OrderType::Market, OrderSide::Buy, 100);
        m_executor.placeOrder(buyOrder);
        m_inPosition = true;
        std::cout << "Kalman Filter Buy signal: Placed buy order at price " << data.price << std::endl;
    } else if (m_estimatedPrice < data.price && m_inPosition) {
        // Sell signal
        Order sellOrder(data.symbol, OrderType::Market, OrderSide::Sell, 100);
        m_executor.placeOrder(sellOrder);
        m_inPosition = false;
        std::cout << "Kalman Filter Sell signal: Placed sell order at price " << data.price << std::endl;
    }
}