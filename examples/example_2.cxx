///
/// @file example_2.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief Compare RK4 results against analytic solutions, and sweep the step size.
/// @version 0.1
/// @date 2025-10-24
/// @copyright Copyright (c) 2023-2025
/// SPDX-License-Identifier: MIT
///

#include <cmath>
#include <cstddef>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "ba7lya/rk4/rk4.hxx"

namespace rk4 = ba7lya::rk4;

namespace {

using exact_function = std::function<double(double)>;

// Integrate one ODE to x = x0 + h * steps and print numerical, analytic and
// absolute-error columns along the way.
template<rk4::ode_function f_t>
void run_test_case(
    const std::string& name,
    const f_t& ode,
    double x0,
    double y0,
    double h,
    std::size_t total_steps,
    const exact_function& exact_solution
) {
    std::cout << "\n=== " << name << " ===" << '\n';
    std::cout << "step: " << h << ", total_steps: " << total_steps << '\n';
    std::cout << std::setw(10) << "x" << std::setw(15) << "numerical sol." << std::setw(15)
              << "analytic sol." << std::setw(15) << "abs err" << '\n';
    std::cout << std::string(55, '-') << '\n';

    rk4::rk4 solver { ode };
    double x = x0;
    double y = y0;

    for (std::size_t i = 0; i <= total_steps; ++i) {
        const double exact = exact_solution(x);
        const double error = std::abs(y - exact);

        std::cout << std::setw(10) << std::fixed << std::setprecision(3) << x << std::setw(15)
                  << std::setprecision(6) << y << std::setw(15) << exact << std::setw(15) << error
                  << '\n';

        if (i < total_steps) { solver.step(x, y, h); }
    }
}

} // namespace

int main() {
    std::cout << "example_2" << '\n';
    std::cout << "==========================================" << '\n';

    // Test case 1: dy/dx = cos(x), analytic solution y = sin(x)
    run_test_case(
        "example 1: dy/dx = cos(x)",
        [](double x, double) noexcept { return std::cos(x); },
        0.0,
        0.0,
        0.1,
        20,
        [](double x) { return std::sin(x); }
    );

    // Test case 2: dy/dx = -y, analytic solution y = e^(-x)
    run_test_case(
        "example 2: dy/dx = -y",
        [](double, double y) noexcept { return -y; },
        0.0,
        1.0,
        0.1,
        20,
        [](double x) { return std::exp(-x); }
    );

    // Test case 3: dy/dx = x + y, analytic solution y = 2·eˣ − x − 1
    run_test_case(
        "example 3: dy/dx = x + y",
        [](double x, double y) noexcept { return x + y; },
        0.0,
        1.0,
        0.05,
        20,
        [](double x) { return 2 * std::exp(x) - x - 1; }
    );

    // Accuracy comparison across different step sizes.
    std::cout << "\n=== step size accuracy comparison ===" << '\n';
    const std::vector<double> step_sizes = { 0.2, 0.1, 0.05, 0.01 };

    for (const double h : step_sizes) {
        rk4::rk4 solver { [](double x, double) noexcept { return std::cos(x); } };
        double x = 0.0;
        double y = 0.0;
        const auto steps = static_cast<std::size_t>(2.0 / h); // integrate to x = 2.0

        solver(x, y, h, steps);
        const double exact = std::sin(2.0);

        std::cout << "step " << h << ": numerical sol.=" << y << ", analytic sol.=" << exact
                  << ", err=" << std::abs(y - exact) << '\n';
    }

    return 0;
}
