#pragma once
#include <pdf/config.hpp>
#include <pdf/units.hpp>

namespace pdf {

struct size {
public:
  units cx;
  units cy;

  constexpr size() noexcept = default;
  constexpr size(units cx, units cy) noexcept : cx(cx), cy(cy) {
  }
};

}  // namespace pdf
