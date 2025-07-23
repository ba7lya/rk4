///
/// @file rk4.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2025-07-24
/// @copyright Copyright (c) 2025
///

#pragma once

#include <cmath>
#include <functional>
#include <iostream>
#include <vector>

namespace ba7lya::rk4 {

///
/// @brief 4阶龙格库塔算法
///
class rk4 {
public:
    /// @brief 构造函数
    /// @param[in] ode 外部提供的微分方程 dy/dx = f(x,y)
    explicit rk4(std::function<double(double, double)> ode)
        : ode_(ode) {}

    /// @brief 析构函数
    ~rk4() {}

    /// @param[in,out] x x值
    /// @param[in,out] y y值
    /// @param[in] h 步长
    /// @param[in] steps 迭代次数
    /// @return
    void operator()(double& x, double& y, const double h, const int steps) {
        double k1 { 0.0 }, k2 { 0.0 }, k3 { 0.0 }, k4 { 0.0 };
        for (int i = 0; i < steps; i++) {
            k1 = ode_(x, y);
            k2 = ode_(x + h / 2, y + k1 / 2);
            k3 = ode_(x + h / 2, y + k2 / 2);
            k4 = ode_(x + h, y + h * k3);
            y += (k1 + 2 * k2 + 2 * k3 + k4) * h / 6;
            x += h;
        }
    }

private:
    std::function<double(double, double)> ode_; // dy/dx = f(x,y)
};

} // namespace ba7lya::rk4
