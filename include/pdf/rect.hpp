#pragma once
#include <pdf/config.hpp>
#include <pdf/units.hpp>
#include <pdf/point.hpp>
#include <pdf/size.hpp>
#include <algorithm>

namespace pdf {

struct rect {
public:
  units x;
  units y;
  units cx;
  units cy;

  constexpr rect() noexcept = default;

  constexpr rect(units x, units y, units cx, units cy) noexcept : x(x), y(y), cx(cx), cy(cy) {
  }

  constexpr rect(point a, point b) noexcept :
    x(std::min(a.x, b.x)), y(std::min(a.y, b.y)), cx(a.x > b.x ? a.x - b.x : b.x - a.x),
    cy(a.y > b.y ? a.y - b.y : b.y - a.y) {
  }

  constexpr rect(point point, size size) noexcept : x(point.x), y(point.y), cx(size.cx), cy(size.cy) {
  }
};

}  // namespace pdf
