#pragma once

#include <chrono>

namespace engine {

using Clock = std::chrono::steady_clock;

inline double secondsBetween(Clock::time_point start, Clock::time_point end) {
    return std::chrono::duration<double>(end - start).count();
}

}
