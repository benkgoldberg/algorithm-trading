#pragma once
#include "HFTAlgorithm.h"
#include "Data/TickData.h"
#include <deque>

class SimpleMovingAverageCrossover : public HFTAlgorithm {
public:
    SimpleMovingAverageCrossover(SimulatedExecution& executor, LiveData& dataFeed, int shortPeriod, int longPeriod, double stopLossPercentage = 0.10, double takeProfitPercentage = 0.10);
    void processData(const TickData& data) override;
    void run() override;

private:
    int m_shortPeriod;
    int m_longPeriod;
    std::deque<double> m_shortPeriodPrices;
    std::deque<double> m_longPeriodPrices;
    double m_shortMA;
    double m_longMA;
    bool m_inPosition;

    void updateMovingAverages(double price);
    void checkForCrossover(const TickData& data);
};