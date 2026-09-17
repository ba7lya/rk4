///
/// @file rk4.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief 4th-order Runge-Kutta ODE integrator for dy/dx = f(x, y).
/// @version 0.1
/// @date 2025-07-24
/// @copyright Copyright (c) 2023-2025
/// SPDX-License-Identifier: MIT
///

#pragma once

#include <concepts>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace ba7lya::rk4 {

///
/// @brief Concept for a scalar ODE right-hand side f(x, y) -> double.
///
/// The call is made through a const reference, so the callable must not
/// mutate itself while being evaluated by the integrator.
///
template<typename f_t>
concept ode_function = std::is_copy_constructible_v<std::remove_cvref_t<f_t>>
                    && std::invocable<const std::remove_cvref_t<f_t>&, double, double>;

///
/// @brief Classical 4th-order Runge-Kutta integrator for dy/dx = f(x, y).
///
/// The ODE is stored directly (no type erasure), so the class is reentrant,
/// holds no per-step state, and is usable in constexpr contexts whenever
/// the callable is.
///
template<ode_function f_t>
class rk4 {
public:
    ///
    /// @brief Construct the integrator from an ODE right-hand side.
    /// @param[in] ode f(x, y) such that dy/dx = f(x, y)
    ///
    constexpr explicit rk4(f_t ode)
        : ode_ { std::move(ode) } {}

    ///
    /// @brief Advance (x, y) by one step of size h.
    /// @param[in,out] x current point, incremented by h
    /// @param[in,out] y current value, updated by the RK4 formula
    /// @param[in] h step size (may be negative to integrate backwards)
    ///
    constexpr void step(double& x, double& y, double h) const noexcept {
        const double k1 = ode_(x, y);
        const double k2 = ode_(x + h / 2, y + h * k1 / 2);
        const double k3 = ode_(x + h / 2, y + h * k2 / 2);
        const double k4 = ode_(x + h, y + h * k3);
        y += (k1 + 2 * k2 + 2 * k3 + k4) * h / 6;
        x += h;
    }

    ///
    /// @brief Advance (x, y) by a fixed number of uniform steps.
    /// @param[in,out] x current point, incremented by steps * h
    /// @param[in,out] y current value, updated in place
    /// @param[in] h step size (may be negative to integrate backwards)
    /// @param[in] steps number of steps to take
    ///
    constexpr void operator()(double& x, double& y, double h, std::size_t steps) const noexcept {
        for (std::size_t i = 0; i < steps; ++i) { step(x, y, h); }
    }

private:
    f_t ode_; // dy/dx = f(x, y)
};

template<typename f_t>
rk4(f_t) -> rk4<f_t>; // CTAD: deduce the callable type from the ODE

} // namespace ba7lya::rk4
