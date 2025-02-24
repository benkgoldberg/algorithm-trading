#ifndef LIVE_DATA_H
#define LIVE_DATA_H

#include "TwsApiCpp/Shared/Contract.h"
#include "MyClient.h"
#include "TickData.h"
#include <string>
#include <vector>
#include <mutex>
#include <atomic>
#include <thread>
#include <chrono>

class LiveData {
public:
    static const size_t MAX_BUFFER_SIZE = 1000;

    LiveData(MyClient& client);
    ~LiveData();
    void requestData();
    // Comment out this line:
    // void printLiveData();
    void startAsyncDataCollection(std::chrono::milliseconds interval);
    void stopAsyncDataCollection();
    TickData getLatestTickData();
    void printLatestTickData() const;

private:
    std::string m_tickerSymbol;
    MyClient& m_client;
    Contract createStockContract(const std::string& symbol);
    Contract contract;
    int m_tickerId;
    std::vector<TickData> m_tickDataBuffer;
    TickData m_latestTickData;
    mutable std::mutex m_dataMutex;
    std::atomic<bool> m_running;
    std::thread m_dataCollectionThread;

    void asyncDataCollection(std::chrono::milliseconds interval);
    void processHighFrequencyData();
};

#endif // LIVE_DATA_H