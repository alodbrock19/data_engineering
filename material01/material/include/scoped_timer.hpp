#pragma once

#include <chrono>
#include <iostream>
#include <string>

class ScopedTimer {
public:
    explicit ScopedTimer(const std::string& name)
        : name_(name), start_(std::chrono::steady_clock::now()) {}

    ~ScopedTimer() {
        std::chrono::duration<double, std::milli> elapsed = std::chrono::steady_clock::now() - start_;
        std::cout << name_ << ": " << elapsed.count() << " ms\n";
    }

private:
    std::string name_;
    std::chrono::steady_clock::time_point start_;
};
