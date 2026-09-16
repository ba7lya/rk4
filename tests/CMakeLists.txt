# SPDX-License-Identifier: MIT

# --- CMake Version ---
cmake_minimum_required(VERSION 3.21)

include(GoogleTest)

# --- rk4_tests ---
add_executable(${PROJECT_NAME}_tests)
target_sources(${PROJECT_NAME}_tests PRIVATE rk4_test.cxx)
target_link_libraries(
    ${PROJECT_NAME}_tests
    PRIVATE ${PROJECT_NAME} GTest::gtest GTest::gtest_main
)
target_compile_features(${PROJECT_NAME}_tests PRIVATE cxx_std_23)
gtest_discover_tests(${PROJECT_NAME}_tests)
