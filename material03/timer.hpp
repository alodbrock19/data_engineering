#ifndef TPCH_JOIN_TIMER_HPP
#define TPCH_JOIN_TIMER_HPP

#include <chrono>
#include <iostream>
#include <string>
#include <utility>

// Prints how long the enclosing scope took, e.g. "load: 1234.5 ms".
// The clock starts in the constructor and stops in the destructor.
class ScopedTimer {
public:
    explicit ScopedTimer(std::string label) : label_(std::move(label)), start_(Clock::now()) {}

    ~ScopedTimer() {
        std::chrono::duration<double, std::milli> elapsed = Clock::now() - start_;
        std::cerr << label_ << ": " << elapsed.count() << " ms\n";
    }

    ScopedTimer(const ScopedTimer&) = delete;
    ScopedTimer& operator=(const ScopedTimer&) = delete;

private:
    using Clock = std::chrono::steady_clock;
    std::string label_;
    Clock::time_point start_;
};
#endif //TPCH_JOIN_TIMER_HPP
