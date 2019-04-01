#pragma once
#include <pdf/config.hpp>
#include <pdf/units.hpp>

namespace pdf {

struct point {
public:
  units x;
  units y;

  constexpr point() noexcept = default;
  constexpr point(units x, units y) noexcept : x(x), y(y) {
  }
};

}  // namespace pdf
