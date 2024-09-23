#pragma once
#include "HFTAlgorithm.h"
#include "Data/TickData.h"
#include <deque>

class RSIAlgorithm : public HFTAlgorithm {
public:
    RSIAlgorithm(SimulatedExecution& executor, LiveData& dataFeed, int period = 14, double overbought = 70, double oversold = 30);
    void processData(const TickData& data) override;
    void run() override;

private:
    int m_period;
    double m_overboughtThreshold;
    double m_oversoldThreshold;
    std::deque<double> m_prices;
    double m_lastRSI;
    bool m_inPosition;

    double calculateRSI();
    void checkForSignals(const TickData& data);
};