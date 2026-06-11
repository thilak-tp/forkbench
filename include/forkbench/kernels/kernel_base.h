#pragma once

#include "../config.h"
#include "../devices/cpu/cpu_device.h"
#include "../timing/stopwatch.h"
#include "../forkbench.h"

namespace forkbench {

template<typename Derived>
class CpuKernel : public Benchmark {
public:
    std::string_view device_type() const override { return "CPU"; }

    Result run(uint64_t iterations = FORKBENCH_DEFAULT_REPETITIONS) override {
        auto& cpu = get_cpu_device();
        cpu.set_high_priority();

        Stopwatch sw(cpu);

        // Warm-up
        for (uint64_t i = 0; i < FORKBENCH_DEFAULT_WARMUP_ITERATIONS; ++i) {
            static_cast<Derived*>(this)->run_kernel(1);
        }

        sw.start();
        for (uint64_t i = 0; i < iterations; ++i) {
            static_cast<Derived*>(this)->run_kernel(1);
        }
        sw.stop();

        Result r{};
        r.name = static_cast<Derived*>(this)->name();
        r.kernel_name = static_cast<Derived*>(this)->name();
        r.iterations = iterations;
        r.time_seconds = sw.elapsed_seconds();
        r.min_time = r.time_seconds;
        r.mean_time = r.time_seconds;

        return r;
    }

protected:
    CpuKernel() = default;
};

} // namespace forkbench
