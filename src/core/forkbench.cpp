#include "../../include/forkbench/forkbench.h"
#include "../../include/forkbench/devices/cpu/cpu_device.h"
#include <vector>
#include <algorithm>

namespace forkbench {

namespace {
    std::vector<Benchmark*> g_benchmarks;
}

CpuDevice& get_cpu_device() {
    return CpuDevice::instance();
}

void register_benchmark(Benchmark* bench) {
    if (bench) {
        g_benchmarks.push_back(bench);
    }
}

Result run_benchmark(std::string_view name, uint64_t iterations) {
    auto it = std::find_if(g_benchmarks.begin(), g_benchmarks.end(),
        [&](Benchmark* b) { return b && b->name() == name; });

    if (it != g_benchmarks.end()) {
        return (*it)->run(iterations);
    }

    Result r{};
    r.name = "Error: Benchmark not found";
    r.kernel_name = name;
    return r;
}

} // namespace forkbench
