#ifndef HISTORICAL_DATA_H
#define HISTORICAL_DATA_H

#include "TwsApiCpp/Shared/Contract.h"
#include "MyClient.h"
#include <string>

class HistoricalData {
public:
    HistoricalData(MyClient& client);
    void requestData();
    void printHistoricalData();

private:
    std::string m_tickerSymbol;
    MyClient& m_client;
    Contract createStockContract(const std::string& symbol);
    Contract contract;
};

#endif // HISTORICAL_DATA_H