#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <random>
#include "../../src/Trades/SimulatedExecution.h"

class SimulatedExecutionTest : public ::testing::Test {
protected:
    MyClient client;
    const double initialBalance = 10000.0;
    SimulatedExecution execution{client, initialBalance};
};

TEST_F(SimulatedExecutionTest, MarketBuyOrderExecution) {
    Order order("AAPL", OrderType::Market, OrderSide::Buy, 10);
    execution.placeOrder(order);

    double bid = 100.0;
    double ask = 100.5;
    execution.processOrders(bid, ask);

    // Should execute at ask price
    EXPECT_DOUBLE_EQ(execution.getAccountBalance(), initialBalance - (10 * ask));

    Position pos = execution.getPosition("AAPL");
    EXPECT_DOUBLE_EQ(pos.quantity, 10);
    EXPECT_DOUBLE_EQ(pos.averagePrice, ask);
}

TEST_F(SimulatedExecutionTest, MarketSellOrderExecution) {
    // First buy some shares to sell
    Order buyOrder("AAPL", OrderType::Market, OrderSide::Buy, 10);
    execution.placeOrder(buyOrder);
    execution.processOrders(100.0, 100.5);

    Order sellOrder("AAPL", OrderType::Market, OrderSide::Sell, 5);
    execution.placeOrder(sellOrder);

    double bid = 101.0;
    double ask = 101.5;
    execution.processOrders(bid, ask);

    Position pos = execution.getPosition("AAPL");
    EXPECT_DOUBLE_EQ(pos.quantity, 5);  // Should have 5 shares left
}

TEST_F(SimulatedExecutionTest, LimitBuyOrderExecution) {
    Order order("AAPL", OrderType::Limit, OrderSide::Buy, 10, 100.0);
    execution.placeOrder(order);

    // Ask price above limit - should not execute
    execution.processOrders(99.5, 100.5);
    EXPECT_DOUBLE_EQ(execution.getAccountBalance(), initialBalance);

    // Ask price at limit - should execute
    execution.processOrders(99.5, 100.0);
    EXPECT_DOUBLE_EQ(execution.getAccountBalance(), initialBalance - (10 * 100.0));
}

TEST_F(SimulatedExecutionTest, LimitSellOrderExecution) {
    // First buy some shares to sell
    Order buyOrder("AAPL", OrderType::Market, OrderSide::Buy, 10);
    execution.placeOrder(buyOrder);
    execution.processOrders(100.0, 100.5);

    Order sellOrder("AAPL", OrderType::Limit, OrderSide::Sell, 5, 102.0);
    execution.placeOrder(sellOrder);

    // Bid price below limit - should not execute
    execution.processOrders(101.0, 101.5);
    Position pos = execution.getPosition("AAPL");
    EXPECT_DOUBLE_EQ(pos.quantity, 10);

    // Bid price at limit - should execute
    execution.processOrders(102.0, 102.5);
    pos = execution.getPosition("AAPL");
    EXPECT_DOUBLE_EQ(pos.quantity, 5);
}

TEST_F(SimulatedExecutionTest, InsufficientFundsTest) {
    Order order("AAPL", OrderType::Market, OrderSide::Buy, 1000);  // Very large order
    execution.placeOrder(order);

    double bid = 100.0;
    double ask = 100.5;
    execution.processOrders(bid, ask);

    // Balance should remain unchanged since order couldn't execute
    EXPECT_DOUBLE_EQ(execution.getAccountBalance(), initialBalance);
}

TEST_F(SimulatedExecutionTest, ConcurrentOrderProcessing) {
    const int numThreads = 4;
    const int ordersPerThread = 100;
    std::vector<std::thread> threads;

    auto threadFunc = [&]() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> priceDist(90.0, 110.0);
        std::uniform_int_distribution<> quantityDist(1, 10);
        std::uniform_int_distribution<> typeDist(0, 1);
        std::uniform_int_distribution<> sideDist(0, 1);

        for (int i = 0; i < ordersPerThread; ++i) {
            OrderType type = typeDist(gen) == 0 ? OrderType::Market : OrderType::Limit;
            OrderSide side = sideDist(gen) == 0 ? OrderSide::Buy : OrderSide::Sell;
            double price = priceDist(gen);
            int quantity = quantityDist(gen);

            Order order("AAPL", type, side, quantity, price);
            execution.placeOrder(order);

            // Simulate market prices
            double bid = priceDist(gen);
            double ask = bid + 0.5;  // Ensure ask > bid
            execution.processOrders(bid, ask);
        }
    };

    // Start threads
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(threadFunc);
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }

    // Verify the final state is consistent
    EXPECT_GE(execution.getAccountBalance(), 0.0);  // Balance should never go negative
    Position pos = execution.getPosition("AAPL");
    EXPECT_FALSE(std::isnan(pos.averagePrice));
    EXPECT_FALSE(std::isnan(pos.quantity));
}
