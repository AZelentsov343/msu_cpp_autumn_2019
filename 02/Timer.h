#ifndef INC_02_TIMER_H
#define INC_02_TIMER_H

#include <chrono>

class Timer {
public:
    using clock_t = std::chrono::high_resolution_clock;
    using microseconds = std::chrono::microseconds;

    void start();

    long long finish() const;

private:
    clock_t::time_point start_;
};


#endif //INC_02_TIMER_H
