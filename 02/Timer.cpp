#include <chrono>
#include "Timer.h"

using cloc = std::chrono::high_resolution_clock;
using microseconds = std::chrono::microseconds;

void Timer::start() {
    start_ = cloc::now();
}

long long Timer::finish() const {
    const auto finish = cloc::now();
    const auto us =
            std::chrono::duration_cast<microseconds>
                    (finish - start_).count();
    return us;
}