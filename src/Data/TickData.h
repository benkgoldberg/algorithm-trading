#pragma once
#include <string>
#include <chrono>

struct TickData {
    std::string symbol;
    double price;
    double bid;
    double ask;
    std::chrono::system_clock::time_point timestamp;
    // Add any other relevant fields
};