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
#include <iostream>

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

    std::cout << "4th-order Runge-Kutta results:" << '\n';
    std::cout << "x, f, y" << '\n';
    double x = x0;
    double y = y0;
    for (std::size_t i = 0; i < rounds; ++i) {
        solver(x, y, h, steps);
        std::cout << x << "," << std::sin(x) + y0 << "," << y << '\n';
    }

    return 0;
}
