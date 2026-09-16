///
/// @file example_1.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief Integrate dy/dx = cos(x) and compare against y = sin(x) + C.
/// @version 0.1
/// @date 2025-07-24
/// @copyright Copyright (c) 2023-2025
/// SPDX-License-Identifier: MIT
///

#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string>

#include "ba7lya/rk4/rk4.hxx"

namespace rk4 = ba7lya::rk4;

int main() {
    constexpr double x0 = 0.0;          // initial x
    constexpr double y0 = 1.0;          // initial y
    constexpr double h = 0.1;           // step size
    constexpr std::size_t steps = 3;    // steps per round
    constexpr std::size_t rounds = 100; // number of rounds

    // dy/dx = cos(x), exact solution y = sin(x) + y0
    auto f = [](double x, double) noexcept { return std::cos(x); };
    rk4::rk4 solver { f };

    std::cout << "=== dy/dx = cos(x), y(0) = " << y0 << " ===" << '\n';
    std::cout << "step: " << h << ", rounds: " << rounds << ", steps per round: " << steps
              << std::endl;
    std::cout << std::setw(10) << "x" << std::setw(15) << "numerical sol." << std::setw(15)
              << "analytic sol." << std::setw(15) << "abs err" << '\n';
    std::cout << std::string(55, '-') << '\n';

    double x = x0;
    double y = y0;
    std::cout << std::setw(10) << std::fixed << std::setprecision(3) << x << std::setw(15)
              << std::setprecision(6) << y << std::setw(15) << std::sin(x) + y0 << std::setw(15)
              << std::abs(y - (std::sin(x) + y0)) << '\n';

    for (std::size_t i = 0; i < rounds; ++i) {
        solver(x, y, h, steps);
        const double exact = std::sin(x) + y0;
        const double error = std::abs(y - exact);
        std::cout << std::setw(10) << std::fixed << std::setprecision(3) << x << std::setw(15)
                  << std::setprecision(6) << y << std::setw(15) << exact << std::setw(15) << error
                  << '\n';
    }

    return 0;
}
