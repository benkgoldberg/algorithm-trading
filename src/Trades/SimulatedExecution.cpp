#include "SimulatedExecution.h"
#include <algorithm>

SimulatedExecution::SimulatedExecution(MyClient& client) : m_client(client) {}

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
        executeOrder(order, executionPrice);
    }

    // Process limit orders
    auto it = m_limitOrders.begin();
    while (it != m_limitOrders.end()) {
        if (it->getSide() == OrderSide::Buy && currentAsk <= it->getPrice()) {
            executeOrder(*it, currentAsk);
            it = m_limitOrders.erase(it);
        } else if (it->getSide() == OrderSide::Sell && currentBid >= it->getPrice()) {
            executeOrder(*it, currentBid);
            it = m_limitOrders.erase(it);
        } else {
            ++it;
        }
    }
}

void SimulatedExecution::executeOrder(const Order& order, double executionPrice) {
    double quantity = (order.getSide() == OrderSide::Buy) ? order.getQuantity() : -order.getQuantity();
    updatePosition(order.getSymbol(), quantity, executionPrice);
    printOrderExecution(order, executionPrice);
}

void SimulatedExecution::updatePosition(const std::string& symbol, double quantity, double price) {
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

// Add these implementations
void SimulatedExecution::printOrderPlacement(const Order& order) const {
    std::cout << "Order placed: "
              << "Symbol: " << order.getSymbol()
              << ", Type: " << (order.getType() == OrderType::Market ? "Market" : "Limit")
              << ", Side: " << (order.getSide() == OrderSide::Buy ? "Buy" : "Sell")
              << ", Quantity: " << order.getQuantity()
              << ", Price: " << (order.getType() == OrderType::Limit ? std::to_string(order.getPrice()) : "N/A")
              << std::endl;
}

void SimulatedExecution::printOrderExecution(const Order& order, double executionPrice) const {
    std::cout << "Order executed: "
              << "Symbol: " << order.getSymbol()
              << ", Side: " << (order.getSide() == OrderSide::Buy ? "Buy" : "Sell")
              << ", Quantity: " << order.getQuantity()
              << ", Execution Price: " << executionPrice
              << std::endl;
}