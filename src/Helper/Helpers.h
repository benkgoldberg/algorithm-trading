#ifndef HELPERS_H
#define HELPERS_H

#include <ctime>

namespace Helpers {

std::time_t getCurrentUTCTime();
std::time_t convertToEasternTime(std::time_t utc_time);

}  // namespace Helpers

#endif // HELPERS_H