#pragma once
#include "HFTAlgorithm.h"
#include "Data/TickData.h"
#include <vector>

class KalmanFilterAlgorithm : public HFTAlgorithm {
public:
    KalmanFilterAlgorithm(SimulatedExecution& executor, LiveData& dataFeed, 
                          double processNoise = 1e-5, double measurementNoise = 1e-1);
    void processData(const TickData& data) override;
    void run() override;

private:
    double m_processNoise;
    double m_measurementNoise;
    double m_estimatedPrice;
    double m_estimatedError;
    bool m_inPosition;

    void updateEstimates(double price);
    void makeDecision(const TickData& data);
};