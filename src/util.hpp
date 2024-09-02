#pragma once

#include <charconv>
#include <chrono>
#include <iostream>

class Timer
{
private:
    // Type aliases to make accessing nested type easier
    using Clock = std::chrono::steady_clock;

    std::chrono::time_point<Clock> m_beg{ Clock::now() };

public:
    void reset()
    {
        m_beg = Clock::now();
    }

    auto elapsed() const
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - m_beg).count();
    }

    ~Timer() { std::cout << elapsed() << "\n"; }
};

#include <limits>
#include <tgmath.h>

inline bool essentiallyEqual(double a, double b, double epsilon = std::numeric_limits<double>::epsilon())
{
    return fabs(a - b) <= ((fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}
// Yes, I may assume there is no issue with floating-point comparisons in this context.
// However the comparisons for the tests are off if not implemented so i need to have this
// at least for testing purposes

#include <string>

inline std::string format_double(double value, int precision = 8) {
    char buffer[32];  // Buffer large enough for most double representations
    auto [ptr, ec] = std::to_chars(buffer, buffer + sizeof(buffer), value, std::chars_format::fixed, precision);
    if (ec == std::errc()) {
        return std::string(buffer, ptr);
    }
    return {}; // In case of an error, return an empty string
}