#pragma once
#include "SimulatedExecution.h"
#include "LiveData.h"
#include "TickData.h"
#include <atomic>

class HFTAlgorithm {
public:
    HFTAlgorithm(SimulatedExecution& executor, LiveData& dataFeed, double stopLossPercentage = 0.10, double takeProfitPercentage = 0.10);
    virtual void processData(const TickData& data) = 0;
    virtual void run() = 0;
    void stop();
    virtual ~HFTAlgorithm() = default;  // Add this line

protected:
    SimulatedExecution& m_executor;
    LiveData& m_dataFeed;
    std::atomic<bool> m_running;
    double m_stopLossPercentage;
    double m_takeProfitPercentage;
    double m_entryPrice;

    bool checkStopLossAndTakeProfit(const TickData& data);
};