///
/// @file rk4_benchmark.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief Google-Benchmark timings for the RK4 integrator.
/// @version 0.1
/// @date 2026-09-16
/// @copyright Copyright (c) 2023-2025
/// SPDX-License-Identifier: MIT
///

#include <benchmark/benchmark.h>
#include <cstddef>

#include "ba7lya/rk4/rk4.hxx"

namespace { using ba7lya::rk4::rk4; } // namespace

// Time one operator() call integrating a fixed number of uniform steps
// of the exponential decay problem dy/dx = -y (cheap RHS, so the timing
// is dominated by the four ODE evaluations per step).
static void BM_rk4_steps(benchmark::State& state) {
    const auto steps = static_cast<std::size_t>(state.range(0));
    rk4 solver { [](double, double y) noexcept { return -y; } };

    for (auto _ : state) {
        double x = 0.0;
        double y = 1.0;
        solver(x, y, 1e-4, steps);
        benchmark::DoNotOptimize(y);
    }

    state.SetItemsProcessed(static_cast<std::int64_t>(steps) * state.iterations());
}

BENCHMARK(BM_rk4_steps)->Arg(100)->Arg(1000)->Arg(100000)->ArgNames({ "steps" });

// Time the single-step entry point with a stateful-looking lambda to make
// sure the templated ODE call is not type-erased.
static void BM_rk4_single_step(benchmark::State& state) {
    rk4 solver { [](double x, double y) noexcept { return -y + 0.0 * x; } };
    double x = 0.0;
    double y = 1.0;

    for (auto _ : state) {
        solver.step(x, y, 1e-6);
        benchmark::DoNotOptimize(y);
        if (x > 1.0) { // re-anchor so the benchmark stays numerically bounded
            x = 0.0;
            y = 1.0;
        }
    }
}

BENCHMARK(BM_rk4_single_step);
