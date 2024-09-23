#include "MyClient.h"
#include "LiveData.h"

MyClient::MyClient() : client(this) {}

void MyClient::currentTime(long time) {
    std::cout << "Current TWS time: " << time << std::endl;
}

void MyClient::error(const int id, const int errorCode, const IBString errorString) {
    std::cerr << "Error: " << id << " | " << errorCode << " - " << errorString << std::endl;
}

void MyClient::historicalData(TickerId reqId, const IBString& date, double open, double high,
                              double low, double close, int volume, int barCount, double WAP, int hasGaps) {
    if (date.find("finished") == std::string::npos) {
        std::ostringstream oss;
        oss << date << " - Open: " << open << ", High: " << high 
            << ", Low: " << low << ", Close: " << close << ", Volume: " << volume;
        historicalDataPoints.push_back(oss.str());
    }
    marketDataReceived = true;
}

void MyClient::tickPrice(TickerId tickerId, TickType field, double price, int canAutoExecute) {
    std::cout << "Received tick: TickerId=" << tickerId << ", Field=" << field << ", Price=" << price << std::endl;
    switch (field) {
        case LAST:
            lastPrice = price;
            std::cout << "Updated lastPrice: " << lastPrice << std::endl;
            break;
        case BID:
            bidPrice = price;
            std::cout << "Updated bidPrice: " << bidPrice << std::endl;
            break;
        case ASK:
            askPrice = price;
            std::cout << "Updated askPrice: " << askPrice << std::endl;
            break;
        default:
            std::cout << "Unhandled tick type: " << field << std::endl;
            break;
    }
    marketDataReceived = true;
    std::cout << "marketDataReceived set to true" << std::endl;
}

// Implement other EWrapper methods with empty bodies
void MyClient::tickSize(TickerId tickerId, TickType field, int size) {}
void MyClient::tickOptionComputation(TickerId tickerId, TickType tickType, double impliedVol, double delta,
                                     double optPrice, double pvDividend, double gamma, double vega, double theta, double undPrice) {}
void MyClient::tickGeneric(TickerId tickerId, TickType tickType, double value) {}
void MyClient::tickString(TickerId tickerId, TickType tickType, const IBString& value) {}
void MyClient::tickEFP(TickerId tickerId, TickType tickType, double basisPoints, const IBString& formattedBasisPoints,
                       double totalDividends, int holdDays, const IBString& futureLastTradeDate, double dividendImpact, double dividendsToLastTradeDate) {}
void MyClient::orderStatus(OrderId orderId, const IBString &status, int filled,
                           int remaining, double avgFillPrice, int permId, int parentId,
                           double lastFillPrice, int clientId, const IBString& whyHeld) {}
void MyClient::openOrder(OrderId orderId, const Contract&, const Order&, const OrderState&) {}
void MyClient::openOrderEnd() {}
void MyClient::winError(const IBString &str, int lastError) {}
void MyClient::connectionClosed() {}
void MyClient::updateAccountValue(const IBString& key, const IBString& val,
                                  const IBString& currency, const IBString& accountName) {}
void MyClient::updatePortfolio(const Contract& contract, int position,
                               double marketPrice, double marketValue, double averageCost,
                               double unrealizedPNL, double realizedPNL, const IBString& accountName) {}
void MyClient::updateAccountTime(const IBString& timeStamp) {}
void MyClient::accountDownloadEnd(const IBString& accountName) {}
void MyClient::nextValidId(OrderId orderId) {}
void MyClient::contractDetails(int reqId, const ContractDetails& contractDetails) {}
void MyClient::bondContractDetails(int reqId, const ContractDetails& contractDetails) {}
void MyClient::contractDetailsEnd(int reqId) {}
void MyClient::execDetails(int reqId, const Contract& contract, const Execution& execution) {}
void MyClient::execDetailsEnd(int reqId) {}
void MyClient::updateMktDepth(TickerId id, int position, int operation, int side,
                              double price, int size) {}
void MyClient::updateMktDepthL2(TickerId id, int position, IBString marketMaker, int operation,
                                int side, double price, int size) {}
void MyClient::updateNewsBulletin(int msgId, int msgType, const IBString& newsMessage, const IBString& originExch) {}
void MyClient::managedAccounts(const IBString& accountsList) {}
void MyClient::receiveFA(faDataType pFaDataType, const IBString& cxml) {}
void MyClient::scannerParameters(const IBString &xml) {}
void MyClient::scannerData(int reqId, int rank, const ContractDetails &contractDetails,
                           const IBString &distance, const IBString &benchmark, const IBString &projection,
                           const IBString &legsStr) {}
void MyClient::scannerDataEnd(int reqId) {}
void MyClient::realtimeBar(TickerId reqId, long time, double open, double high, double low, double close,
                           long volume, double wap, int count) {}
void MyClient::fundamentalData(TickerId reqId, const IBString& data) {}
void MyClient::deltaNeutralValidation(int reqId, const UnderComp& underComp) {}
void MyClient::tickSnapshotEnd(int reqId) {}
void MyClient::marketDataType(TickerId reqId, int marketDataType) {}
void MyClient::commissionReport(const CommissionReport &commissionReport) {}
void MyClient::position(const IBString& account, const Contract& contract, int position, double avgCost) {}
void MyClient::positionEnd() {}
void MyClient::accountSummary(int reqId, const IBString& account, const IBString& tag, const IBString& value, const IBString& curency) {}
void MyClient::accountSummaryEnd(int reqId) {}
void MyClient::verifyMessageAPI(const IBString& apiData) {}
void MyClient::verifyCompleted(bool isSuccessful, const IBString& errorText) {}
void MyClient::displayGroupList(int reqId, const IBString& groups) {}
void MyClient::displayGroupUpdated(int reqId, const IBString& contractInfo) {}