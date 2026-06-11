#include "../../../include/forkbench/devices/cpu/cpu_device.h"
#include <chrono>
#include <thread>

namespace forkbench {

// Static instance
CpuDevice& CpuDevice::instance() {
    static CpuDevice instance;
    return instance;
}

uint32_t CpuDevice::core_count() const {
#if defined(FORKBENCH_PLATFORM_LINUX)
    return static_cast<uint32_t>(sysconf(_SC_NPROCESSORS_ONLN));
#elif defined(FORKBENCH_PLATFORM_WINDOWS)
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#else
    return 1;
#endif
}

uint64_t CpuDevice::get_timestamp() const {
#if defined(__x86_64__) || defined(__i386__)
    uint32_t lo, hi;
    asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return static_cast<uint64_t>(hi) << 32 | lo;
#else
    // Fallback
    return static_cast<uint64_t>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count());
#endif
}

double CpuDevice::timestamp_to_seconds(uint64_t ticks) const {
    if (ticks_per_second_ == 0.0) {
        calibrate_timer();
    }
    return static_cast<double>(ticks) / ticks_per_second_;
}

void CpuDevice::calibrate_timer() const {
    // Simple calibration using std::chrono
    auto start_chrono = std::chrono::high_resolution_clock::now();
    auto start_rdtsc = get_timestamp();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    auto end_rdtsc = get_timestamp();
    auto end_chrono = std::chrono::high_resolution_clock::now();

    auto elapsed = std::chrono::duration<double>(end_chrono - start_chrono).count();
    ticks_per_second_ = (end_rdtsc - start_rdtsc) / elapsed;
}

void CpuDevice::pin_thread(uint32_t core_id) {
#if defined(FORKBENCH_PLATFORM_LINUX)
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id % core_count(), &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
#elif defined(FORKBENCH_PLATFORM_WINDOWS)
    SetThreadAffinityMask(GetCurrentThread(), 1ULL << (core_id % core_count()));
#endif
}

void CpuDevice::set_high_priority() {
#if defined(FORKBENCH_PLATFORM_WINDOWS)
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
#endif
    // Linux: can be extended with sched_setscheduler if needed
}

} // namespace forkbench
