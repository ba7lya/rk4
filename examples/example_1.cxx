///
/// @file example_1.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2025-07-24
/// @copyright Copyright (c) 2025
///

#include <cmath>
#include <iostream>

#include "ba7lya/rk4/rk4.hxx"

double ode(const double x, const double y) { return sin(x) * sin(x) * y; }

using namespace ba7lya::rk4;

int main(int argc, const char* argv[]) {
    constexpr double x0 = 0.0; // 初始x值
    constexpr double y0 = 1.0; // 初始y值
    constexpr double h = 0.1;  // 步长
    constexpr int steps = 10;  // 迭代次数

    rk4 rk(ode);
    std::cout << "4阶龙格库塔方法计算结果:" << rk(x0, y0, h, steps) << std::endl;

    return 0;
}
