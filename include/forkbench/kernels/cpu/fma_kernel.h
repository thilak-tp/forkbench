#pragma once

#include "../kernel_base.h"
#include <cmath>
#include <cstdint>
#include <string_view>

namespace forkbench::kernels::cpu {

class FmaKernel final : public CpuKernel<FmaKernel> {
public:
    std::string_view name() const override { return "FMA_Throughput"; }

    void run_kernel(uint64_t iterations) {
        double a = 1.23456789;
        double b = 9.87654321;
        double c = 0.0;

        for (uint64_t i = 0; i < iterations; ++i) {
            c = std::fma(a, b, c);
            c = std::fma(a, c, b);
            c = std::fma(b, c, a);
            c = std::fma(b, a, c);
        }

        volatile double sink = c;
        (void)sink;
    }
};

// Manual registration (temporary until macro is fixed)
namespace {
    FmaKernel fma_kernel_instance;
    bool registered = []() {
        forkbench::register_benchmark(&fma_kernel_instance);
        return true;
    }();
}

} // namespace forkbench::kernels::cpu
