#include "HFTAlgorithm.h"
#include "Trades/SimulatedExecution.h"
#include "Data/LiveData.h"

HFTAlgorithm::HFTAlgorithm(SimulatedExecution& executor, LiveData& dataFeed, double stopLossPercentage, double takeProfitPercentage)
    : m_executor(executor), m_dataFeed(dataFeed), m_running(false), m_stopLossPercentage(stopLossPercentage), m_takeProfitPercentage(takeProfitPercentage), m_entryPrice(0) {}

void HFTAlgorithm::stop() {
    m_running = false;
}

bool HFTAlgorithm::checkStopLossAndTakeProfit(const TickData& data) {
    if (m_entryPrice == 0) {
        return false;
    }

    double currentPrice = data.price;
    double stopLossPrice = m_entryPrice * (1 - m_stopLossPercentage);
    double takeProfitPrice = m_entryPrice * (1 + m_takeProfitPercentage);

    if (currentPrice <= stopLossPrice || currentPrice >= takeProfitPrice) {
        Order sellOrder(data.symbol, OrderType::Market, OrderSide::Sell, 100);
        m_executor.placeOrder(sellOrder);
        m_entryPrice = 0;
        return true;
    }

    return false;
}