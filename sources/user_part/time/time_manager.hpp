#pragma once
#include <chrono>

using TimePoint = std::chrono::steady_clock::time_point;

namespace ars_graphics
{
class TimeManager final
{
public:
    TimeManager();
    
    void startTimer();

    double restartTimer();

private:
    TimePoint m_start{};
};
} // namespace ars_graphics
