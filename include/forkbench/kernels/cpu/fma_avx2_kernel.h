#pragma once

#include "../kernel_base.h"
#include <immintrin.h>
#include <cstdint>
#include <string_view>

namespace forkbench::kernels::cpu {

class FmaAvx2Kernel final : public CpuKernel<FmaAvx2Kernel> {
public:
    std::string_view name() const override { return "FMA_AVX2"; }

    void run_kernel(uint64_t iterations) {
        __m256d a = _mm256_set1_pd(1.23456789);
        __m256d b = _mm256_set1_pd(9.87654321);
        __m256d c = _mm256_setzero_pd();

        for (uint64_t i = 0; i < iterations; ++i) {
            c = _mm256_fmadd_pd(a, b, c);
            c = _mm256_fmadd_pd(a, c, b);
            c = _mm256_fmadd_pd(b, c, a);
            c = _mm256_fmadd_pd(b, a, c);
        }

        // Prevent optimization away
        double sink[4];
        _mm256_storeu_pd(sink, c);
        volatile double v = sink[0] + sink[1] + sink[2] + sink[3];
        (void)v;
    }
};

// Manual registration
namespace {
    FmaAvx2Kernel avx2_instance;
    bool avx2_registered = []() {
        forkbench::register_benchmark(&avx2_instance);
        return true;
    }();
}

} // namespace forkbench::kernels::cpu
