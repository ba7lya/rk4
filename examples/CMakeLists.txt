# --- CMake Version ---
cmake_minimum_required(VERSION 3.21)

# --- example_1 ---
add_executable(${PROJECT_NAME}_example_1)
target_sources(${PROJECT_NAME}_example_1 PRIVATE example_1.cxx)
target_link_libraries(${PROJECT_NAME}_example_1 PRIVATE ${PROJECT_NAME})
target_compile_features(${PROJECT_NAME}_example_1 PRIVATE cxx_std_23)
