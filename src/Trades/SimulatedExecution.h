#pragma once
#include "Data/MyClient.h"
#include "Helper/AtomicDouble.h"
#include "Order.h"
#include <queue>
#include <vector>
#include <mutex>
#include <atomic>
#include <unordered_map>

struct Position {
    std::string symbol;
    double quantity;
    double averagePrice;
};

class SimulatedExecution {
public:
    SimulatedExecution(MyClient& client, double initialBalance = 100000.0);
    void placeOrder(const Order& order);
    void processOrders(double currentBid, double currentAsk);
    Position getPosition(const std::string& symbol) const;
    void printOrderPlacement(const Order& order) const;
    void printOrderExecution(const Order& order, double executionPrice) const;
    double getAccountBalance() const;

private:
    MyClient& m_client;
    std::queue<Order> m_orderQueue;
    std::vector<Order> m_limitOrders;
    std::unordered_map<std::string, Position> m_positions;
    mutable std::mutex m_mutex;
    AtomicDouble m_accountBalance;

    void executeOrder(const Order& order, double executionPrice);
    void updatePosition(const std::string& symbol, double quantity, double price);
    bool canExecuteOrder(const Order& order, double executionPrice) const;
};