// main.cpp
#include "MyClient.h"
#include "LiveData.h"
#include <thread>
#include <chrono>
#include "Algorithms/HFTAlgorithm.h"
#include "Algorithms/SimpleMovingAverageCrossover.h"
#include "Algorithms/RSIAlgorithm.h"
#include "Algorithms/MonteCarloAlgorithm.h"
#include "Algorithms/KalmanFilterAlgorithm.h"
#include <vector>
#include <memory>

void runAlgorithm(std::unique_ptr<HFTAlgorithm> algorithm) {
    algorithm->run();
}

int main() {
    MyClient client;
    bool connected = client.client.eConnect("127.0.0.1", 7497, 0);

    if (connected) {
        std::cout << "Connected to TWS successfully." << std::endl;
        
        // Wait a bit to ensure the connection is stable
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // Check if still connected
        if (!client.client.isConnected()) {
            std::cerr << "Lost connection to TWS." << std::endl;
            return 1;
        }

        int choice;
        std::cout << "Choose an option:" << std::endl;
        std::cout << "1. View Historical Data" << std::endl;
        std::cout << "2. View Live Data" << std::endl;
        std::cout << "3. Run HFT Algorithm" << std::endl;
        std::cout << "Enter your choice (1, 2, or 3): ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                // Historical Data
                // HistoricalData historicalData(client);
                // historicalData.requestData();
                // historicalData.printHistoricalData();
                break;
            }
            case 2: {
                // Live Data
                LiveData liveData(client);
                liveData.requestData();
                liveData.printLiveData();  // This method already handles data collection and printing
                break;
            }
            case 3: {
                // HFT Algorithm
                SimulatedExecution executor(client);
                LiveData liveData(client);
                
                // Request data explicitly
                liveData.requestData();

                // Choose algorithm type
                int algoChoice;
                std::cout << "Choose an HFT algorithm:" << std::endl;
                std::cout << "1. Simple Moving Average Crossover" << std::endl;
                std::cout << "2. RSI Algorithm" << std::endl;
                std::cout << "3. Monte Carlo Algorithm" << std::endl;
                std::cout << "4. Kalman Filter Algorithm" << std::endl;
                std::cout << "5. Run All Algorithms Concurrently" << std::endl;
                std::cout << "Enter your choice (1, 2, 3, 4, or 5): ";
                std::cin >> algoChoice;

                double stopLossPercentage, takeProfitPercentage;
                std::cout << "Enter stop loss percentage (default 0.10): ";
                std::cin >> stopLossPercentage;
                std::cout << "Enter take profit percentage (default 0.10): ";
                std::cin >> takeProfitPercentage;

                std::unique_ptr<HFTAlgorithm> algorithm;
                switch (algoChoice) {
                    case 1:
                        algorithm = std::make_unique<SimpleMovingAverageCrossover>(executor, liveData, 10, 30, stopLossPercentage, takeProfitPercentage);
                        break;
                    case 2:
                        algorithm = std::make_unique<RSIAlgorithm>(executor, liveData, 9, stopLossPercentage, takeProfitPercentage);
                        break;
                    case 3:
                        algorithm = std::make_unique<MonteCarloAlgorithm>(executor, liveData);
                        break;
                    case 4:
                        algorithm = std::make_unique<KalmanFilterAlgorithm>(executor, liveData);
                        break;
                    case 5: {
                        // Run all algorithms concurrently
                        std::vector<std::thread> threads;
                        std::vector<std::unique_ptr<HFTAlgorithm>> algorithms;

                        algorithms.push_back(std::make_unique<SimpleMovingAverageCrossover>(executor, liveData, 10, 30, stopLossPercentage, takeProfitPercentage));
                        algorithms.push_back(std::make_unique<RSIAlgorithm>(executor, liveData, 9, stopLossPercentage, takeProfitPercentage));
                        algorithms.push_back(std::make_unique<MonteCarloAlgorithm>(executor, liveData));
                        algorithms.push_back(std::make_unique<KalmanFilterAlgorithm>(executor, liveData));

                        for (auto& algo : algorithms) {
                            threads.emplace_back(runAlgorithm, std::move(algo));
                        }

                        // Wait for all threads to finish
                        for (auto& thread : threads) {
                            if (thread.joinable()) {
                                thread.join();
                            }
                        }

                        break;
                    }
                    default:
                        std::cout << "Invalid choice. Exiting." << std::endl;
                        return 1;
                }

                if (algoChoice != 5) {
                    // Process and print initial data for about 10 seconds
                    auto start = std::chrono::steady_clock::now();
                    auto end = start + std::chrono::seconds(10);

                    std::cout << "Collecting initial data for 10 seconds..." << std::endl;
                    while (std::chrono::steady_clock::now() < end) {
                        client.client.checkMessages();
                        liveData.processHighFrequencyData();
                        liveData.printLatestTickData();  // Print the latest tick data
                        std::this_thread::sleep_for(std::chrono::seconds(1));  // Check every 1 second
                    }

                    algorithm->run();

                    // Add this to allow the algorithm to run for a specified duration
                    std::cout << "Algorithm is running. Press Enter to stop." << std::endl;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.get();
                    algorithm->stop();
                }

                break;
            }
            default:
                std::cout << "Invalid choice. Exiting." << std::endl;
                break;
        }

        // Disconnect
        client.client.eDisconnect();
    } else {
        std::cerr << "Failed to connect to TWS." << std::endl;
    }

    return 0;
}