#pragma once

// ===================================================================
// Forkbench Configuration
// Modern C++ CPU Benchmarking Library
// ===================================================================

// Version
#define FORKBENCH_VERSION_MAJOR 0
#define FORKBENCH_VERSION_MINOR 1
#define FORKBENCH_VERSION_PATCH 0

// Platform detection
#if defined(FORKBENCH_WINDOWS)
    #define FORKBENCH_PLATFORM_WINDOWS
#elif defined(FORKBENCH_LINUX)
    #define FORKBENCH_PLATFORM_LINUX
#else
    #error "Unsupported platform. Only Windows and Linux are supported."
#endif

// C++ standard
#if __cplusplus < 202002L
    #warning "Forkbench recommends C++20 or later"
#endif

// Compile-time tunable options
#ifndef FORKBENCH_DEFAULT_REPETITIONS
    #define FORKBENCH_DEFAULT_REPETITIONS 1000
#endif

#ifndef FORKBENCH_DEFAULT_WARMUP_ITERATIONS
    #define FORKBENCH_DEFAULT_WARMUP_ITERATIONS 100
#endif

#ifndef FORKBENCH_ENABLE_STATISTICS
    #define FORKBENCH_ENABLE_STATISTICS 1
#endif

// Helper macros
#define FORKBENCH_INLINE inline __attribute__((always_inline))
#define FORKBENCH_NOINLINE __attribute__((noinline))
#define FORKBENCH_FORCE_INLINE __attribute__((always_inline))

namespace forkbench {

// Forward declarations
class Benchmark;
class Result;
class Runner;

} // namespace forkbench
