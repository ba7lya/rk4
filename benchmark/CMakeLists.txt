# --- CMake Version ---
cmake_minimum_required(VERSION 3.21)

# --- rk4_benchmark ---
add_executable(${PROJECT_NAME}_benchmark)
target_sources(${PROJECT_NAME}_benchmark PRIVATE rk4_benchmark.cxx)
target_link_libraries(
    ${PROJECT_NAME}_benchmark
    PRIVATE ${PROJECT_NAME} benchmark::benchmark benchmark::benchmark_main
)
target_compile_features(${PROJECT_NAME}_benchmark PRIVATE cxx_std_23)
