#pragma once

#include "config.h"
#include "devices/device.h"
#include "devices/cpu/cpu_device.h"
#include "timing/stopwatch.h"
#include <string_view>
#include <cstdint>
#include <chrono>

// ===================================================================
// Forkbench - Main Public Header
// ===================================================================

namespace forkbench {

// Result struct - full definition
struct Result {
    std::string_view name;
    std::string_view kernel_name;
    double time_seconds = 0.0;
    uint64_t iterations = 0;
    double throughput = 0.0;
    double min_time = 0.0;
    double max_time = 0.0;
    double mean_time = 0.0;
    double stddev = 0.0;
};

// Base benchmark interface
class Benchmark {
public:
    virtual ~Benchmark() = default;
    virtual Result run(uint64_t iterations = FORKBENCH_DEFAULT_REPETITIONS) = 0;
    virtual std::string_view name() const = 0;
    virtual std::string_view device_type() const = 0;
};

// Public API
CpuDevice& get_cpu_device();
void register_benchmark(Benchmark* bench);
Result run_benchmark(std::string_view name, uint64_t iterations = FORKBENCH_DEFAULT_REPETITIONS);

} // namespace forkbench
