# ba7lya.rk4

[![CI](https://github.com/BA7LYA/rk4/actions/workflows/ci.yml/badge.svg)](https://github.com/BA7LYA/rk4/actions/workflows/ci.yml)

4th order Runge-Kutta | 4阶龙格库塔算法

Header-only C++23 library, MIT licensed.

## Usage

```cpp
#include "ba7lya/rk4/rk4.hxx"

// dy/dx = -y, y(0) = 1  →  y(x) = e^(-x)
ba7lya::rk4::rk4 solver{[](double /*x*/, double y) noexcept { return -y; }};

double x = 0.0;
double y = 1.0;
solver(x, y, /*h = */0.1, /*steps = */20); // integrate to x = 2.0

solver.step(x, y, 0.1); // or take a single step
```

The ODE is stored directly (no `std::function` type erasure), so the
integrator is reentrant, thread-safe on its own copies, and usable in
`constexpr` contexts.

## Building (vcpkg manifest mode)

The repo vendors [vcpkg](https://github.com/microsoft/vcpkg) as a submodule
at `thirdparty/vcpkg`:

```bash
git submodule update --init --recursive
cmake --preset MSVC-2022-x64-Ninja   # or GCC-linux-x64-Ninja on Linux
cmake --build --preset MSVC-2022-x64-Debug
ctest --preset MSVC-2022-x64-Test
```

Options: `ba7lya.rk4_BUILD_DEMO` (ON), `ba7lya.rk4_BUILD_TEST` (ON when
top-level), `ba7lya.rk4_BUILD_BENCHMARK` (OFF).

Consuming as an installed dependency:

```cmake
find_package(ba7lya.rk4 CONFIG REQUIRED)
target_link_libraries(main PRIVATE ba7lya::rk4)
```

## References

[Runge-Kutta methods @wikipedia](https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods)

## License

MIT — see [LICENSE](LICENSE).
