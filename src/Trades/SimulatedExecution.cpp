#include "SimulatedExecution.h"
#include <algorithm>
#include <iostream>

SimulatedExecution::SimulatedExecution(MyClient& client, double initialBalance) 
    : m_client(client), m_accountBalance(initialBalance) {}

void SimulatedExecution::placeOrder(const Order& order) {
    std::lock_guard<std::mutex> lock(m_mutex);
    printOrderPlacement(order);
    if (order.getType() == OrderType::Market) {
        m_orderQueue.push(order);
    } else {
        m_limitOrders.push_back(order);
    }
}

void SimulatedExecution::processOrders(double currentBid, double currentAsk) {
    std::lock_guard<std::mutex> lock(m_mutex);

    // Process market orders
    while (!m_orderQueue.empty()) {
        auto order = m_orderQueue.front();
        m_orderQueue.pop();
        double executionPrice = (order.getSide() == OrderSide::Buy) ? currentAsk : currentBid;
        if (canExecuteOrder(order, executionPrice)) {
            executeOrder(order, executionPrice);
        } else {
            std::cout << "Insufficient funds to execute order." << std::endl;
        }
    }

    // Process limit orders
    auto it = m_limitOrders.begin();
    while (it != m_limitOrders.end()) {
        if (it->getSide() == OrderSide::Buy && currentAsk <= it->getPrice()) {
            if (canExecuteOrder(*it, currentAsk)) {
                executeOrder(*it, currentAsk);
                it = m_limitOrders.erase(it);
            } else {
                std::cout << "Insufficient funds to execute limit buy order." << std::endl;
                ++it;
            }
        } else if (it->getSide() == OrderSide::Sell && currentBid >= it->getPrice()) {
            executeOrder(*it, currentBid);
            it = m_limitOrders.erase(it);
        } else {
            ++it;
        }
    }
}

void SimulatedExecution::executeOrder(const Order& order, double executionPrice) {
    std::lock_guard<std::mutex> lock(m_mutex);
    double quantity = (order.getSide() == OrderSide::Buy) ? order.getQuantity() : -order.getQuantity();
    double orderCost = std::abs(quantity) * executionPrice;

    if (order.getSide() == OrderSide::Buy) {
        m_accountBalance.fetch_sub(orderCost);
    } else {
        m_accountBalance.fetch_add(orderCost);
    }

    updatePosition(order.getSymbol(), quantity, executionPrice);
    printOrderExecution(order, executionPrice);
    std::cout << "New account balance: $" << m_accountBalance.load() << std::endl;
}

void SimulatedExecution::updatePosition(const std::string& symbol, double quantity, double price) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto& position = m_positions[symbol];
    double oldQuantity = position.quantity;
    double oldAveragePrice = position.averagePrice;

    position.quantity += quantity;
    if (position.quantity != 0) {
        position.averagePrice = (oldQuantity * oldAveragePrice + quantity * price) / position.quantity;
    } else {
        position.averagePrice = 0;
    }
}

Position SimulatedExecution::getPosition(const std::string& symbol) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_positions.find(symbol);
    if (it != m_positions.end()) {
        return it->second;
    }
    return {symbol, 0, 0};
}

void SimulatedExecution::printOrderPlacement(const Order& order) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::cout << "Order placed: "
              << "Symbol: " << order.getSymbol()
              << ", Type: " << (order.getType() == OrderType::Market ? "Market" : "Limit")
              << ", Side: " << (order.getSide() == OrderSide::Buy ? "Buy" : "Sell")
              << ", Quantity: " << order.getQuantity()
              << ", Price: " << (order.getType() == OrderType::Limit ? std::to_string(order.getPrice()) : "N/A")
              << std::endl;
}

void SimulatedExecution::printOrderExecution(const Order& order, double executionPrice) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::cout << "Order executed: "
              << "Symbol: " << order.getSymbol()
              << ", Side: " << (order.getSide() == OrderSide::Buy ? "Buy" : "Sell")
              << ", Quantity: " << order.getQuantity()
              << ", Execution Price: " << executionPrice
              << std::endl;
}

double SimulatedExecution::getAccountBalance() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_accountBalance.load();
}

bool SimulatedExecution::canExecuteOrder(const Order& order, double executionPrice) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (order.getSide() == OrderSide::Buy) {
        double orderCost = order.getQuantity() * executionPrice;
        return m_accountBalance.load() >= orderCost;
    }
    return true;  // We assume you can always sell (no short selling restrictions)
}