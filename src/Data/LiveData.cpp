#include "LiveData.h"
#include "TickData.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <ctime>
#include "Helpers.h"

LiveData::LiveData(MyClient& client) 
    : m_client(client), m_tickerId(2) {
    std::cout << "Enter ticker symbol for live data: ";
    std::cin >> m_tickerSymbol;
    contract = createStockContract(m_tickerSymbol);
    m_latestTickData.symbol = m_tickerSymbol;  // Initialize the symbol
}

Contract LiveData::createStockContract(const std::string& symbol) {
    Contract contract;
    contract.symbol = symbol;
    contract.secType = "STK";
    contract.exchange = "SMART";
    contract.currency = "USD";
    return contract;
}

void LiveData::requestData() {
    std::cout << "Requesting delayed market data for " << m_tickerSymbol << std::endl;
    m_client.client.reqMarketDataType(3);  // 3 for delayed data
    m_client.client.reqMktData(m_tickerId, contract, "", false, TagValueListSPtr());
    std::cout << "Market data request sent for " << m_tickerSymbol << std::endl;
}

void LiveData::printLiveData() {
    std::cout << "Live market data for " << m_tickerSymbol << ":" << std::endl;
    auto start = std::chrono::steady_clock::now();
    auto end = start + std::chrono::minutes(1);

    bool dataReceived = false;
    while (std::chrono::steady_clock::now() < end) {
        m_client.client.checkMessages();
        if (m_client.lastPrice > 0 || m_client.bidPrice > 0 || m_client.askPrice > 0) {
            std::time_t now_c = Helpers::getCurrentUTCTime();
            
            // Convert to Eastern Time
            std::time_t eastern_time = Helpers::convertToEasternTime(now_c);
            std::tm* eastern_tm = std::localtime(&eastern_time);

            std::cout << "Time (ET): " 
                      << std::put_time(eastern_tm, "%Y-%m-%d %H:%M:%S");
            
            if (m_client.lastPrice > 0) {
                std::cout << ", Last: " << m_client.lastPrice;
            }
            if (m_client.bidPrice > 0) {
                std::cout << ", Bid: " << m_client.bidPrice;
            }
            if (m_client.askPrice > 0) {
                std::cout << ", Ask: " << m_client.askPrice;
            }
            std::cout << std::endl;
            dataReceived = true;
        } else {
            std::cout << "Waiting for data for " << m_tickerSymbol << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    if (!dataReceived) {
        std::cout << "No data received." << std::endl;
    }

    m_client.client.cancelMktData(m_tickerId);
}

void LiveData::processHighFrequencyData() {
    std::lock_guard<std::mutex> lock(m_dataMutex);
    
    bool newDataReceived = false;
    
    if (m_client.lastPrice > 0) {
        m_latestTickData.price = m_client.lastPrice;
        newDataReceived = true;
    }
    if (m_client.bidPrice > 0) {
        m_latestTickData.bid = m_client.bidPrice;
        newDataReceived = true;
    }
    if (m_client.askPrice > 0) {
        m_latestTickData.ask = m_client.askPrice;
        newDataReceived = true;
    }
    
    if (newDataReceived) {
        m_latestTickData.timestamp = std::chrono::system_clock::now();
        
        m_tickDataBuffer.push_back(m_latestTickData);
        if (m_tickDataBuffer.size() > MAX_BUFFER_SIZE) {
            m_tickDataBuffer.erase(m_tickDataBuffer.begin());
        }
    }
}

TickData LiveData::getLatestTickData() {
    std::lock_guard<std::mutex> lock(m_dataMutex);
    if (m_latestTickData.timestamp.time_since_epoch().count() == 0) {
        throw std::runtime_error("No tick data available yet");
    }
    return m_latestTickData;
}

void LiveData::printLatestTickData() const {
    std::lock_guard<std::mutex> lock(m_dataMutex);
    
    if (m_latestTickData.price <= 0 && m_latestTickData.bid <= 0 && m_latestTickData.ask <= 0) {
        std::cout << "No market data available yet for " << m_tickerSymbol << "." << std::endl;
        return;
    }
    
    auto now = std::chrono::system_clock::to_time_t(m_latestTickData.timestamp);
    std::cout << "Time: " << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S")
              << ", Symbol: " << m_latestTickData.symbol;
    
    if (m_latestTickData.price > 0) {
        std::cout << ", Price: " << m_latestTickData.price;
    }
    if (m_latestTickData.bid > 0) {
        std::cout << ", Bid: " << m_latestTickData.bid;
    }
    if (m_latestTickData.ask > 0) {
        std::cout << ", Ask: " << m_latestTickData.ask;
    }
    
    std::cout << std::endl;
}