#pragma once
#include "HFTAlgorithm.h"
#include "Data/TickData.h"
#include <vector>
#include <random>

class MonteCarloAlgorithm : public HFTAlgorithm {
public:
    static const size_t MAX_WINDOW_SIZE = 1000;
    MonteCarloAlgorithm(SimulatedExecution& executor, LiveData& dataFeed, 
                        int numSimulations = 1000, int timeHorizon = 100, 
                        double buyThreshold = 0.8, double sellThreshold = 0.2);
    void processData(const TickData& data) override;
    void run() override;

private:
    int m_numSimulations;
    int m_timeHorizon; // in seconds
    double m_buyThreshold;
    double m_sellThreshold;
    std::vector<double> m_historicalReturns;
    bool m_inPosition;

    std::mt19937 m_generator;
    std::normal_distribution<> m_distribution;

    void updateHistoricalReturns(const TickData& data);
    std::vector<double> simulatePricePath();
    void makeDecision(const TickData& data);
};