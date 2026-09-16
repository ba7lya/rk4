# SPDX-License-Identifier: MIT

# --- cmake version ---
cmake_minimum_required(VERSION 3.21)

# --- project ---
project(
    ba7lya.rk4
    VERSION 0.1.0
    DESCRIPTION "Header-only 4th-order Runge-Kutta ODE integrator"
    LANGUAGES CXX
)

# --- includes ---
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# --- build options ---
if(PROJECT_IS_TOP_LEVEL)
    set(_ba7lya_rk4_test_default ON)
else()
    set(_ba7lya_rk4_test_default OFF)
endif()

option(${PROJECT_NAME}_BUILD_BENCHMARK "Build the benchmark" OFF)
option(${PROJECT_NAME}_BUILD_DEMO "Build demo programs" ON)
option(${PROJECT_NAME}_BUILD_DOC "Build Documentation" OFF)
option(${PROJECT_NAME}_BUILD_TEST "Build tests" ${_ba7lya_rk4_test_default})

# --- target ---
add_library(${PROJECT_NAME} INTERFACE)
add_library(ba7lya::rk4 ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES EXPORT_NAME rk4)

# --- include directory ---
target_include_directories(
    ${PROJECT_NAME}
    INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# --- compile features ---
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_23)

# --- compile definitions ---
target_compile_definitions(
    ${PROJECT_NAME}
    INTERFACE $<$<PLATFORM_ID:Windows>:NOMINMAX>
              $<$<PLATFORM_ID:Windows>:WINVER=0x0A00>
              $<$<PLATFORM_ID:Windows>:_WIN32_WINNT=0x0A00>
)

# --- compile options ---
target_compile_options(
    ${PROJECT_NAME}
    INTERFACE "$<$<C_COMPILER_ID:MSVC>:/utf-8>" "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
)

# --- modules ---
add_subdirectory(thirdparty)

# --- benchmark ---
if(${PROJECT_NAME}_BUILD_BENCHMARK)
    find_package(benchmark CONFIG REQUIRED)
    add_subdirectory(benchmark)
endif()

# --- demo ---
if(${PROJECT_NAME}_BUILD_DEMO)
    add_subdirectory(examples)
endif()

# --- unit tests ---
if(${PROJECT_NAME}_BUILD_TEST)
    find_package(GTest CONFIG REQUIRED)
    enable_testing()
    add_subdirectory(tests)
endif()

# --- package config ---
configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
write_basic_package_version_file(
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# --- install ---
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
    EXPORT ${PROJECT_NAME}Targets
    NAMESPACE ba7lya::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
install(
    FILES ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
          ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
install(FILES ${PROJECT_SOURCE_DIR}/LICENSE DESTINATION ${CMAKE_INSTALL_DOCDIR} OPTIONAL)
