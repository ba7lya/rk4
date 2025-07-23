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
    /// @brief 默认构造函数
    /// @details 默认微分方程为 dy/dx = x + y
    /// @warning 默认微分方程仅供测试使用，实际使用时请提供自定义微分方程
    rk4()
        : ode_(std::bind(&rk4::ode, this, std::placeholders::_1, std::placeholders::_2)) {
        std::cerr << "warning: no differential equation assigned." << std::endl;
    }

    /// @brief 构造函数
    /// @param de 外部提供的微分方程 dy/dx = f(x,y)
    explicit rk4(std::function<double(double, double)> ode)
        : ode_(ode) {}

    /// @brief 析构函数
    ~rk4() {}

    /// @param x0 初始x值
    /// @param y0 初始y值
    /// @param h 步长
    /// @param steps 迭代次数
    /// @return
    double operator()(const double x0, const double y0, const double h, const int steps) {
        x = x0;
        y = y0;
        for (int i = 0; i < steps; i++) {
            k1 = h * f(x, y);
            k2 = h * f(x + h / 2, y + k1 / 2);
            k3 = h * f(x + h / 2, y + k2 / 2);
            k4 = h * f(x + h, y + k3);
            y += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
            x += h;
        }
        return y;
    }

private:
    // 示例微分方程 dy/dx = f(x,y)
    const double ode(const double x, const double y) const { return x + y; }

    double x, y;
    double k1, k2, k3, k4;
    std::function<double(double, double)> ode_; // dy/dx = f(x,y)
};

} // namespace ba7lya::rk4
