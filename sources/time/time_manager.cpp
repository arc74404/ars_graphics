#include "time_manager.hpp"

namespace ars_graphics
{

TimeManager::TimeManager()
{
    startTimer();
}

void
TimeManager::startTimer()
{
    m_start = std::chrono::high_resolution_clock::now();
}

double
TimeManager::restartTimer()
{
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end_time - m_start;

    double elapsed_seconds = elapsed.count();

    m_start = end_time;

    return elapsed_seconds;
}
} // namespace ars_graphics
