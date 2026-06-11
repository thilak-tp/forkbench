#pragma once

#include "../device.h"
#include "../../config.h"
#include <string_view>
#include <cstdint>

#if defined(FORKBENCH_PLATFORM_LINUX)
    #include <unistd.h>
    #include <sched.h>
#elif defined(FORKBENCH_PLATFORM_WINDOWS)
    #include <windows.h>
    #include <processthreadsapi.h>
#endif

namespace forkbench {

// =============================================
// CPU Device - Layer 1: Platform Abstraction
// =============================================

class CpuDevice final : public Device {
public:
    static CpuDevice& instance();

    std::string_view name() const override { return "CPU"; }
    std::string_view type() const override { return "CPU"; }

    uint32_t core_count() const override;

    // High-resolution timestamp (RDTSC on x86, fallback to chrono)
    uint64_t get_timestamp() const override;

    double timestamp_to_seconds(uint64_t ticks) const override;

    // CPU-specific utilities
    void pin_thread(uint32_t core_id) override;
    void set_high_priority() override;

private:
    CpuDevice() = default;
    ~CpuDevice() = default;

    mutable double ticks_per_second_ = 0.0;  // Calibration cache
    void calibrate_timer() const;
};

} // namespace forkbench
