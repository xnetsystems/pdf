#pragma once
#include <pdf/config.hpp>
#include <cstdint>

namespace pdf {

struct color {
  std::uint8_t r = 0;
  std::uint8_t g = 0;
  std::uint8_t b = 0;

  constexpr color() noexcept = default;
  constexpr color(std::uint32_t v) noexcept :
    r(static_cast<std::uint8_t>(v >> 16 & 0xFF)), g(static_cast<std::uint8_t>(v >> 8 & 0xFF)), b(v & 0xFF) {
  }
  constexpr color(std::uint8_t r, std::uint8_t g, std::uint8_t b) noexcept : r(r), g(g), b(b) {
  }
};

}  // namespace pdf
