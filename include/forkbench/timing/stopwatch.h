#pragma once

#include "../config.h"
#include "../devices/device.h"
#include <cstdint>

namespace forkbench {

// Forward declaration to avoid circular include
class CpuDevice;
CpuDevice& get_cpu_device();   // Forward declaration

// =============================================
// Stopwatch - Low-overhead timer
// =============================================

class Stopwatch {
public:
    explicit Stopwatch(const Device& device = get_cpu_device())
        : device_(device) {}

    void start() {
        start_ticks_ = device_.get_timestamp();
    }

    void stop() {
        end_ticks_ = device_.get_timestamp();
    }

    uint64_t elapsed_ticks() const {
        return end_ticks_ - start_ticks_;
    }

    double elapsed_seconds() const {
        return device_.timestamp_to_seconds(elapsed_ticks());
    }

    // Convenience: Run a function multiple times and measure
    template<typename F>
    double measure(F&& func, uint64_t iterations = 1) {
        start();
        for (uint64_t i = 0; i < iterations; ++i) {
            func();
        }
        stop();
        return elapsed_seconds();
    }

private:
    const Device& device_;
    uint64_t start_ticks_ = 0;
    uint64_t end_ticks_ = 0;
};

} // namespace forkbench
