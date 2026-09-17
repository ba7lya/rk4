///
/// @file rk4_test.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief Unit tests for ba7lya::rk4::rk4 against analytic ODE solutions.
/// @version 0.1
/// @date 2026-09-16
/// @copyright Copyright (c) 2023-2025
/// SPDX-License-Identifier: MIT
///

#include <cmath>
#include <cstddef>
#include <gtest/gtest.h>

#include "rk4.hxx"

namespace {

using ba7lya::rk4::rk4;

constexpr double x_end = 2.0;                // end of the integration interval
constexpr std::size_t steps_hundredth = 200; // steps for h = 0.01
constexpr std::size_t steps_tenth = 20;      // steps for h = 0.1
constexpr std::size_t steps_twentieth = 40;  // steps for h = 0.05

} // namespace

// The ODEs under test are small lambdas so the integrator can be used
// in constexpr contexts and without type erasure.

TEST(rk4_test, integrates_cos_to_sin) {
    auto f = [](double x, double) noexcept { return std::cos(x); };
    double x = 0.0;
    double y = 0.0;

    rk4 { f }(x, y, x_end / static_cast<double>(steps_hundredth), steps_hundredth);

    EXPECT_NEAR(x, x_end, 1e-12);
    EXPECT_NEAR(y, std::sin(x_end), 1e-6);
}

TEST(rk4_test, integrates_exponential_decay) {
    auto f = [](double, double y) noexcept { return -y; };
    double x = 0.0;
    double y = 1.0;

    rk4 { f }(x, y, x_end / static_cast<double>(steps_hundredth), steps_hundredth);

    EXPECT_NEAR(x, x_end, 1e-12);
    EXPECT_NEAR(y, std::exp(-x_end), 1e-6);
}

TEST(rk4_test, integrates_linear_ode) {
    // dy/dx = x + y with y(0) = 1 has the solution y = 2·eˣ − x − 1
    auto f = [](double x, double y) noexcept { return x + y; };
    double x = 0.0;
    double y = 1.0;
    constexpr double x_stop = 1.0;
    constexpr std::size_t n = 100; // h = 0.01

    rk4 { f }(x, y, x_stop / static_cast<double>(n), n);

    EXPECT_NEAR(x, x_stop, 1e-12);
    EXPECT_NEAR(y, 2 * std::exp(x_stop) - x_stop - 1, 1e-6);
}

TEST(rk4_test, converges_at_fourth_order) {
    auto f = [](double, double y) noexcept { return -y; };
    constexpr double x_stop = 1.0;

    auto integrate = [&](double h, std::size_t n)
    {
        double x = 0.0;
        double y = 1.0;
        rk4 { f }(x, y, h, n);
        return std::abs(y - std::exp(-x_stop));
    };

    const double coarse = integrate(x_stop / steps_tenth, steps_tenth);
    const double fine = integrate(x_stop / steps_twentieth, steps_twentieth);

    ASSERT_GT(coarse, 0.0);
    ASSERT_GT(fine, 0.0);
    // Halving the step size must divide the error by 2⁴ = 16.
    EXPECT_NEAR(coarse / fine, 16.0, 4.0);
}

TEST(rk4_test, zero_steps_and_zero_step_size_leave_state_untouched) {
    auto f = [](double, double y) noexcept { return -y; };
    double x = 0.5;
    double y = 0.25;

    rk4 solver { f };
    solver(x, y, 0.1, 0);
    EXPECT_DOUBLE_EQ(x, 0.5);
    EXPECT_DOUBLE_EQ(y, 0.25);

    solver(x, y, 0.0, 10);
    EXPECT_DOUBLE_EQ(x, 0.5);
    EXPECT_DOUBLE_EQ(y, 0.25);
}

TEST(rk4_test, integrates_backwards_with_negative_step_size) {
    auto f = [](double x, double) noexcept { return std::cos(x); };
    double x = 0.0;
    double y = 0.0;

    rk4 solver { f };
    solver(x, y, x_end / steps_tenth, steps_tenth);  // forward to x = 2
    solver(x, y, -x_end / steps_tenth, steps_tenth); // back to x = 0

    EXPECT_NEAR(x, 0.0, 1e-12);
    EXPECT_NEAR(y, 0.0, 1e-6);
}

TEST(rk4_test, works_in_constexpr_context) {
    // Proves the template (not std::function) integration is constexpr-capable.
    constexpr double decayed = []
    {
        double x = 0.0;
        double y = 1.0;
        rk4 { [](double, double y) noexcept { return -y; } }(x, y, 0.1, 10);
        return y;
    }();

    static_assert(decayed > 0.0 && decayed < 1.0);
    EXPECT_NEAR(decayed, std::exp(-1.0), 1e-6);
}

TEST(rk4_test, single_step_matches_manual_formula) {
    // One step of dy/dx = x + y from (0, 1) with h = 0.1, computed by hand:
    // k1 = 1, k2 = 1.1, k3 = 1.105, k4 = 1.2105
    // y += (1 + 2·1.1 + 2·1.105 + 1.2105) · 0.1 / 6 = 1.1103416666...
    auto f = [](double x, double y) noexcept { return x + y; };
    double x = 0.0;
    double y = 1.0;

    rk4 { f }.step(x, y, 0.1);

    EXPECT_DOUBLE_EQ(x, 0.1);
    EXPECT_NEAR(y, 1.1103416666666667, 1e-12);
}
