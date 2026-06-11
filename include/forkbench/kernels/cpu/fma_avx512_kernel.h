#pragma once

#include "../kernel_base.h"
#include <cstdint>
#include <string_view>

#if defined(FORKBENCH_ENABLE_AVX512) && defined(__AVX512F__) && defined(__AVX512DQ__)

#include <immintrin.h>

namespace forkbench::kernels::cpu {

class FmaAvx512Kernel final : public CpuKernel<FmaAvx512Kernel> {
public:
    std::string_view name() const override { return "FMA_AVX512"; }

    void run_kernel(uint64_t iterations) {
        __m512d a1 = _mm512_set1_pd(1.23456789);
        __m512d b1 = _mm512_set1_pd(9.87654321);
        __m512d c1 = _mm512_setzero_pd();

        __m512d a2 = _mm512_set1_pd(2.34567890);
        __m512d b2 = _mm512_set1_pd(8.76543210);
        __m512d c2 = _mm512_setzero_pd();

        for (uint64_t i = 0; i < iterations; ++i) {
            c1 = _mm512_fmadd_pd(a1, b1, c1);
            c2 = _mm512_fmadd_pd(a2, b2, c2);

            c1 = _mm512_fmadd_pd(a1, c1, b1);
            c2 = _mm512_fmadd_pd(a2, c2, b2);

            c1 = _mm512_fmadd_pd(b1, c1, a1);
            c2 = _mm512_fmadd_pd(b2, c2, a2);

            c1 = _mm512_fmadd_pd(b1, a1, c1);
            c2 = _mm512_fmadd_pd(b2, a2, c2);
        }

        double sink[16];
        _mm512_storeu_pd(sink, c1);
        _mm512_storeu_pd(sink + 8, c2);
        volatile double v = sink[0] + sink[8];
        (void)v;
    }
};

namespace {
    FmaAvx512Kernel instance;
    bool registered = []() {
        forkbench::register_benchmark(&instance);
        return true;
    }();
}

} // namespace forkbench::kernels::cpu

#else

// AVX512 not enabled or not supported - no kernel registered

#endif
