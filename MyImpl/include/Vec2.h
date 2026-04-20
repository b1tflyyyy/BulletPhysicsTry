#pragma once

#include <cmath>

namespace myimpl {

struct Vec2 {
  double x = 0.0;
  double y = 0.0;

  Vec2 operator+(const Vec2 &rhs) const { return {x + rhs.x, y + rhs.y}; }
  Vec2 operator-(const Vec2 &rhs) const { return {x - rhs.x, y - rhs.y}; }
  Vec2 operator*(double scalar) const { return {x * scalar, y * scalar}; }

  Vec2 &operator+=(const Vec2 &rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }
};

inline double degToRad(double degrees) {
  return degrees * std::acos(-1.0) / 180.0;
}

} // namespace myimpl
