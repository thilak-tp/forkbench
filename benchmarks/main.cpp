#include "../include/forkbench/forkbench.h"
#include "../include/forkbench/kernels/cpu/fma_kernel.h"
#include "../include/forkbench/kernels/cpu/fma_avx2_kernel.h"
#include "../include/forkbench/kernels/cpu/fma_avx512_kernel.h"
#include "../include/forkbench/kernels/cpu/fma_avx2_enhanced_kernel.h"
#include <iostream>
#include <iomanip>
#include <vector>
int main() {
    std::cout << "🚀 Forkbench CPU Benchmarking Library\n";
    std::cout << "Version " << FORKBENCH_VERSION_MAJOR << "."
              << FORKBENCH_VERSION_MINOR << "."
              << FORKBENCH_VERSION_PATCH << "\n\n";
    auto& cpu = forkbench::get_cpu_device();
    std::cout << "CPU Cores: " << cpu.core_count() << "\n";
    cpu.pin_thread(0); // Pin to core 0 for stability
    cpu.set_high_priority();
    std::cout << "\nRunning FMA_Throughput benchmark (10 runs)...\n\n";
    const int num_runs = 10;
    std::vector<double> gflops_results;
    for (int run = 1; run <= num_runs; ++run) {
        auto result = forkbench::run_benchmark("FMA_Throughput", 50'000'000); // Increased iterations
        double gflops = 0.0;
        if (result.time_seconds > 0.0) {
            gflops = (result.iterations * 4.0) / (result.time_seconds * 1e9);
        }
        std::cout << "Run " << std::setw(2) << run
                  << ": " << std::fixed << std::setprecision(2)
                  << gflops << " GFLOPS | "
                  << result.time_seconds << " sec\n";
        gflops_results.push_back(gflops);
    }
    // Simple statistics
    double max_gflops = *std::max_element(gflops_results.begin(), gflops_results.end());
    double min_gflops = *std::min_element(gflops_results.begin(), gflops_results.end());
    double avg_gflops = 0.0;
    for (auto g : gflops_results) avg_gflops += g;
    avg_gflops /= num_runs;
    std::cout << "\n=== Summary ===\n";
    std::cout << "Best : " << max_gflops << " GFLOPS\n";
    std::cout << "Average: " << avg_gflops << " GFLOPS\n";
    std::cout << "Worst : " << min_gflops << " GFLOPS\n";
    std::cout << "\n=== AVX2 FMA ===\n";
    for (int i = 1; i <= 5; ++i) {
        auto r = forkbench::run_benchmark("FMA_AVX2", 50'000'000);
        double gflops = (r.iterations * 8.0) / (r.time_seconds * 1e9);
        std::cout << "Run " << i << ": " << std::fixed << std::setprecision(2) << gflops << " GFLOPS\n";
    }
    std::cout << "\n=== Enhanced AVX2 FMA ===\n";
    for (int i = 1; i <= 6; ++i) {
        auto r = forkbench::run_benchmark("FMA_AVX2_Enhanced", 100'000'000);
        double gflops = (r.iterations * 16.0) / (r.time_seconds * 1e9); // 8 FMAs * 2 accumulators
        std::cout << "Run " << i << ": " << std::fixed << std::setprecision(2) << gflops << " GFLOPS\n";
    }
        std::cout << "\n=== AVX512 FMA ===\n";
    for (int i = 1; i <= 4; ++i) {
        auto r = forkbench::run_benchmark("FMA_AVX512", 100'000'000);
        double gflops = (r.iterations * 32.0) / (r.time_seconds * 1e9); // 16 FMAs * 2 accumulators
        std::cout << "Run " << i << ": " << std::fixed << std::setprecision(2) << gflops << " GFLOPS\n";
    }
    return 0;
}
