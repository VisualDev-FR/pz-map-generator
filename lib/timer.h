#pragma once

#include <chrono>

struct Timer
{
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    TimePoint m_startTime;
    Timer() : m_startTime(Clock::now()) {}

    inline static Timer start()
    {
        return Timer();
    }

    inline void restart()
    {
        m_startTime = Clock::now();
    }

    inline float elapsedMiliseconds(bool reset = false)
    {
        TimePoint endTime = Clock::now();
        std::chrono::duration<float, std::milli> elapsed = endTime - m_startTime;

        if (reset)
        {
            m_startTime = endTime;
        }

        return elapsed.count();
    }
};
