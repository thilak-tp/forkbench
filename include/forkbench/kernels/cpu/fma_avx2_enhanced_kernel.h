#pragma once

#include "../kernel_base.h"
#include <immintrin.h>
#include <cstdint>
#include <string_view>

namespace forkbench::kernels::cpu {

// =============================================
// Enhanced AVX2 FMA Kernel - Heavy unrolling + multiple accumulators
// =============================================

class FmaAvx2EnhancedKernel final : public CpuKernel<FmaAvx2EnhancedKernel> {
public:
    std::string_view name() const override { return "FMA_AVX2_Enhanced"; }

    void run_kernel(uint64_t iterations) {
        // Two independent accumulators for better instruction-level parallelism
        __m256d a1 = _mm256_set1_pd(1.23456789);
        __m256d b1 = _mm256_set1_pd(9.87654321);
        __m256d c1 = _mm256_setzero_pd();

        __m256d a2 = _mm256_set1_pd(2.34567890);
        __m256d b2 = _mm256_set1_pd(8.76543210);
        __m256d c2 = _mm256_setzero_pd();

        for (uint64_t i = 0; i < iterations; ++i) {
            c1 = _mm256_fmadd_pd(a1, b1, c1);
            c2 = _mm256_fmadd_pd(a2, b2, c2);

            c1 = _mm256_fmadd_pd(a1, c1, b1);
            c2 = _mm256_fmadd_pd(a2, c2, b2);

            c1 = _mm256_fmadd_pd(b1, c1, a1);
            c2 = _mm256_fmadd_pd(b2, c2, a2);

            c1 = _mm256_fmadd_pd(b1, a1, c1);
            c2 = _mm256_fmadd_pd(b2, a2, c2);
        }

        // Prevent compiler from optimizing away
        double sink[8];
        _mm256_storeu_pd(sink,     c1);
        _mm256_storeu_pd(sink + 4, c2);
        volatile double v = sink[0] + sink[1] + sink[4] + sink[5];
        (void)v;
    }
};

// Manual registration
namespace {
    FmaAvx2EnhancedKernel enhanced_instance;
    bool enhanced_registered = []() {
        forkbench::register_benchmark(&enhanced_instance);
        return true;
    }();
}

} // namespace forkbench::kernels::cpu
