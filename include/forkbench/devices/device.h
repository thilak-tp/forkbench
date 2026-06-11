#pragma once

#include "../config.h"
#include <string_view>
#include <cstdint>

namespace forkbench {

// =============================================
// Device Abstraction - Core for Extensibility
// Allows clean addition of GPU / other devices later
// =============================================

class Device {
public:
    virtual ~Device() = default;

    // Device identification
    virtual std::string_view name() const = 0;
    virtual std::string_view type() const = 0;        // "CPU", "GPU", "NPU", etc.

    // Basic capabilities
    virtual uint32_t core_count() const = 0;
    virtual uint64_t get_timestamp() const = 0;       // High-resolution timer
    virtual double timestamp_to_seconds(uint64_t ticks) const = 0;

    // Optional: CPU-specific (overridable)
    virtual void pin_thread(uint32_t core_id) {}
    virtual void set_high_priority() {}
};

// Concrete CPU Device (forward declared here)
class CpuDevice;

} // namespace forkbench
