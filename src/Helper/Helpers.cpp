#include "Helpers.h"
#include <chrono>

namespace Helpers {

std::time_t getCurrentUTCTime() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::system_clock::to_time_t(now);
}

std::time_t convertToEasternTime(std::time_t utc_time) {
    // Assuming the system is set to Eastern Time
    // If not, you'd need to manually adjust for EDT/EST
    return utc_time - 4 * 3600;  // EDT is UTC-4
}

}  // namespace Helpers