#pragma once
#include <pdf/config.hpp>

namespace pdf {

inline constexpr double mm2in(double mm) noexcept {
  return mm / 25.4;
}

inline constexpr double cm2in(double cm) noexcept {
  return cm / 2.54;
}

inline constexpr double in2mm(double in) noexcept {
  return in * 25.4;
}

inline constexpr double in2cm(double in) noexcept {
  return in * 2.54;
}

class units {
public:
  constexpr static double dpi = 72.0;

  constexpr units() noexcept = default;
  constexpr units(double pt) noexcept : pt_(pt) {
  }

  constexpr double pt() const noexcept {
    return pt_;
  }

  constexpr double in() const noexcept {
    return pt_ / dpi;
  }

  constexpr double mm() const noexcept {
    return in2mm(in());
  }

  constexpr double cm() const noexcept {
    return in2cm(in());
  }

  constexpr operator double() const noexcept {
    return pt_;
  }

  constexpr units& operator+=(const units& other) noexcept {
    pt_ += other.pt_;
    return *this;
  }

  constexpr units& operator-=(const units& other) noexcept {
    pt_ -= other.pt_;
    return *this;
  }

  static constexpr units in(double in) noexcept {
    return units(in * dpi);
  }

  static constexpr units mm(double mm) noexcept {
    return units(mm2in(mm) * dpi);
  }

  static constexpr units cm(double cm) noexcept {
    return units(cm2in(cm) * dpi);
  }

  friend constexpr units operator+(const units& lhs, const units& rhs) noexcept {
    return { lhs.pt_ + rhs.pt_ };
  }

  friend constexpr units operator-(const units& lhs, const units& rhs) noexcept {
    return { lhs.pt_ - rhs.pt_ };
  }

  template <typename T>
  friend units operator*(const units& u, T factor) noexcept {
    return { u.pt_ * factor };
  }

  template <typename T>
  friend units operator/(const units& u, T denominator) noexcept {
    return { u.pt_ / denominator };
  }

  friend constexpr bool operator==(const units& lhs, const units& rhs) noexcept {
    return lhs.pt_ == rhs.pt_;
  }

  friend constexpr bool operator!=(const units& lhs, const units& rhs) noexcept {
    return lhs.pt_ != rhs.pt_;
  }

  friend constexpr bool operator<=(const units& lhs, const units& rhs) noexcept {
    return lhs.pt_ <= rhs.pt_;
  }

  friend constexpr bool operator>=(const units& lhs, const units& rhs) noexcept {
    return lhs.pt_ >= rhs.pt_;
  }

  friend constexpr bool operator<(const units& lhs, const units& rhs) noexcept {
    return lhs.pt_ < rhs.pt_;
  }

  friend constexpr bool operator>(const units& lhs, const units& rhs) noexcept {
    return lhs.pt_ > rhs.pt_;
  }

private:
  double pt_ = 0.0;
};

inline namespace literals {

inline constexpr units operator"" _pt(unsigned long long pt) noexcept {
  return units(static_cast<double>(pt));
}

inline constexpr units operator"" _pt(long double pt) noexcept {
  return units(static_cast<double>(pt));
}

inline constexpr units operator"" _in(unsigned long long in) noexcept {
  return units::in(static_cast<double>(in));
}

inline constexpr units operator"" _in(long double in) noexcept {
  return units::in(static_cast<double>(in));
}

inline constexpr units operator"" _mm(unsigned long long mm) noexcept {
  return units::mm(static_cast<double>(mm));
}

inline constexpr units operator"" _mm(long double mm) noexcept {
  return units::mm(static_cast<double>(mm));
}

inline constexpr units operator"" _cm(unsigned long long cm) noexcept {
  return units::cm(static_cast<double>(cm));
}

inline constexpr units operator"" _cm(long double cm) noexcept {
  return units::cm(static_cast<double>(cm));
}

}  // namespace literals
}  // namespace pdf
