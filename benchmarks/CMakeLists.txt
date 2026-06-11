# benchmarks/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)

add_executable(forkbench_cpu_bench main.cpp)

target_link_libraries(forkbench_cpu_bench PRIVATE forkbench)

# === Vector Instruction Flags ===
target_compile_options(forkbench_cpu_bench PRIVATE -mavx2 -mfma -march=native)

# AVX512 only if explicitly enabled
if(FORKBENCH_ENABLE_AVX512)
    target_compile_options(forkbench_cpu_bench PRIVATE -mavx512f -mavx512dq)
    target_compile_definitions(forkbench_cpu_bench PRIVATE FORKBENCH_ENABLE_AVX512)
endif()
