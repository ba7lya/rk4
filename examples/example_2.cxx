///
/// @file example_2.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2025-10-24
/// @copyright Copyright (c) 2025
///

#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <vector>

#include "ba7lya/rk4/rk4.hxx"

using ba7lya::rk4::rk4;

// 测试用例1: dy/dx = cos(x), 解析解 y = sin(x) + C
double test_ode1(double x, double y) { return cos(x); }

// 测试用例2: dy/dx = -y, 解析解 y = C * exp(-x)
double test_ode2(double x, double y) { return -y; }

// 测试用例3: dy/dx = x + y, 线性微分方程
double test_ode3(double x, double y) { return x + y; }

void run_test_case(
    const std::string& name,
    std::function<double(double, double)> ode,
    double x0,
    double y0,
    double h,
    int total_steps,
    std::function<double(double)> exact_solution
) {
    std::cout << "\n=== " << name << " ===" << std::endl;
    std::cout << "step: " << h << ", total_steps: " << total_steps << std::endl;
    std::cout << std::setw(10) << "x" << std::setw(15) << "numerical sol." << std::setw(15)
              << "analytic sol." << std::setw(15) << "abs err" << std::endl;
    std::cout << std::string(55, '-') << std::endl;

    rk4 solver(ode);
    double x = x0;
    double y = y0;

    for (int i = 0; i <= total_steps; i++) {
        double exact = exact_solution(x);
        double error = std::abs(y - exact);

        std::cout << std::setw(10) << std::fixed << std::setprecision(3) << x << std::setw(15)
                  << std::setprecision(6) << y << std::setw(15) << exact << std::setw(15) << error
                  << std::endl;

        if (i < total_steps) { solver(x, y, h, 1); }
    }
}

int main() {
    std::cout << "example_2" << std::endl;
    std::cout << "==========================================" << std::endl;

    // 测试用例1: dy/dx = cos(x)
    run_test_case(
        "example 1: dy/dx = cos(x)",
        test_ode1,
        0.0,
        0.0,
        0.1,
        20,
        [](double x) { return sin(x); }
    );

    // 测试用例2: dy/dx = -y
    run_test_case(
        "example 2: dy/dx = -y",
        test_ode2,
        0.0,
        1.0,
        0.1,
        20,
        [](double x) { return exp(-x); }
    );

    // 测试用例3: dy/dx = x + y
    run_test_case(
        "example 3: dy/dx = x + y",
        test_ode3,
        0.0,
        1.0,
        0.05,
        20,
        [](double x) { return 2 * exp(x) - x - 1; }
    );

    // 性能测试：比较不同步长下的精度
    std::cout << "\n=== step length accuracy comparison test ===" << std::endl;
    std::vector<double> step_sizes = { 0.2, 0.1, 0.05, 0.01 };

    for (double h : step_sizes) {
        rk4 solver(test_ode1);
        double x = 0.0;
        double y = 0.0;
        int steps = static_cast<int>(2.0 / h); // 积分到x=2.0

        solver(x, y, h, steps);
        double exact = sin(2.0);
        double error = std::abs(y - exact);

        std::cout << "step " << h << ": numerical sol.=" << y << ", analytic sol.=" << exact
                  << ", err=" << error << std::endl;
    }

    return 0;
}
