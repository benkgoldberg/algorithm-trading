#include "HistoricalData.h"
#include <iostream>
#include <thread>
#include <chrono>

HistoricalData::HistoricalData(MyClient& client) : m_client(client) {
    std::cout << "Enter ticker symbol: ";
    std::cin >> m_tickerSymbol;
    contract = createStockContract(m_tickerSymbol);
}

Contract HistoricalData::createStockContract(const std::string& symbol) {
    Contract contract;
    contract.symbol = symbol;
    contract.secType = "STK";
    contract.exchange = "SMART";
    contract.currency = "USD";
    return contract;
}

void HistoricalData::requestData() {
    // Parameters for historical data request
    std::string endDateTime = "20230616-19:00:00"; // Specific past date and time (UTC)
    std::string durationStr = "60 S"; // Duration string: 1 minute
    std::string barSizeSetting = "5 secs"; // Bar size: 5 seconds
    std::string whatToShow = "TRADES"; // Show trade data
    int useRTH = 1; // Use regular trading hours
    int formatDate = 1; // Date format (1 for yyyymmdd)

    m_client.client.reqHistoricalData(1, contract, endDateTime, durationStr, barSizeSetting, 
                                    whatToShow, useRTH, formatDate, TagValueListSPtr());

    // Wait for historical data to be received
    for (int i = 0; i < 60; ++i) {
        m_client.client.checkMessages();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (m_client.marketDataReceived) {
            break;
        }
    }
}

void HistoricalData::printHistoricalData() {
    std::cout << "Historical market data:" << std::endl;
    for (const auto& dataPoint : m_client.historicalDataPoints) {
        std::cout << dataPoint << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    
    // Cancel historical data request after printing
    m_client.client.cancelHistoricalData(1);
}