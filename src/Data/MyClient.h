#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <atomic>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include "TwsApiCpp/src/EPosixClientSocket.h"
#include "TwsApiCpp/Shared/EWrapper.h"
#include "TwsApiCpp/Shared/CommonDefs.h"
#include "Helper/Helpers.h"

class MyClient : public EWrapper {
public:
    EPosixClientSocket client;
    std::atomic<bool> marketDataReceived{false};
    std::atomic<double> lastPrice{0.0};
    std::atomic<double> bidPrice{0.0};
    std::atomic<double> askPrice{0.0};
    std::vector<std::string> historicalDataPoints;

    MyClient();

    void currentTime(long time) override;
    void error(const int id, const int errorCode, const IBString errorString) override;
    void historicalData(TickerId reqId, const IBString& date, double open, double high,
                        double low, double close, int volume, int barCount, double WAP, int hasGaps) override;
    void tickPrice(TickerId tickerId, TickType field, double price, int canAutoExecute) override;

    // Declare other EWrapper methods here (without implementation)
    void tickSize(TickerId tickerId, TickType field, int size) override;
    void tickOptionComputation(TickerId tickerId, TickType tickType, double impliedVol, double delta,
                               double optPrice, double pvDividend, double gamma, double vega, double theta, double undPrice) override;
    void tickGeneric(TickerId tickerId, TickType tickType, double value) override;
    void tickString(TickerId tickerId, TickType tickType, const IBString& value) override;
    void tickEFP(TickerId tickerId, TickType tickType, double basisPoints, const IBString& formattedBasisPoints,
                 double totalDividends, int holdDays, const IBString& futureLastTradeDate, double dividendImpact, double dividendsToLastTradeDate) override;
    void orderStatus(OrderId orderId, const IBString &status, int filled,
                     int remaining, double avgFillPrice, int permId, int parentId,
                     double lastFillPrice, int clientId, const IBString& whyHeld) override;
    void openOrder(OrderId orderId, const Contract&, const Order&, const OrderState&) override;
    void openOrderEnd() override;
    void winError(const IBString &str, int lastError) override;
    void connectionClosed() override;
    void updateAccountValue(const IBString& key, const IBString& val,
                            const IBString& currency, const IBString& accountName) override;
    void updatePortfolio(const Contract& contract, int position,
                         double marketPrice, double marketValue, double averageCost,
                         double unrealizedPNL, double realizedPNL, const IBString& accountName) override;
    void updateAccountTime(const IBString& timeStamp) override;
    void accountDownloadEnd(const IBString& accountName) override;
    void nextValidId(OrderId orderId) override;
    void contractDetails(int reqId, const ContractDetails& contractDetails) override;
    void bondContractDetails(int reqId, const ContractDetails& contractDetails) override;
    void contractDetailsEnd(int reqId) override;
    void execDetails(int reqId, const Contract& contract, const Execution& execution) override;
    void execDetailsEnd(int reqId) override;
    void updateMktDepth(TickerId id, int position, int operation, int side,
                        double price, int size) override;
    void updateMktDepthL2(TickerId id, int position, IBString marketMaker, int operation,
                          int side, double price, int size) override;
    void updateNewsBulletin(int msgId, int msgType, const IBString& newsMessage, const IBString& originExch) override;
    void managedAccounts(const IBString& accountsList) override;
    void receiveFA(faDataType pFaDataType, const IBString& cxml) override;
    void scannerParameters(const IBString &xml) override;
    void scannerData(int reqId, int rank, const ContractDetails &contractDetails,
                     const IBString &distance, const IBString &benchmark, const IBString &projection,
                     const IBString &legsStr) override;
    void scannerDataEnd(int reqId) override;
    void realtimeBar(TickerId reqId, long time, double open, double high, double low, double close,
                     long volume, double wap, int count) override;
    void fundamentalData(TickerId reqId, const IBString& data) override;
    void deltaNeutralValidation(int reqId, const UnderComp& underComp) override;
    void tickSnapshotEnd(int reqId) override;
    void marketDataType(TickerId reqId, int marketDataType) override;
    void commissionReport(const CommissionReport &commissionReport) override;
    void position(const IBString& account, const Contract& contract, int position, double avgCost) override;
    void positionEnd() override;
    void accountSummary(int reqId, const IBString& account, const IBString& tag, const IBString& value, const IBString& curency) override;
    void accountSummaryEnd(int reqId) override;
    void verifyMessageAPI(const IBString& apiData) override;
    void verifyCompleted(bool isSuccessful, const IBString& errorText) override;
    void displayGroupList(int reqId, const IBString& groups) override;
    void displayGroupUpdated(int reqId, const IBString& contractInfo) override;
};

#endif // MYCLIENT_H
