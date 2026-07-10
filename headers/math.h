#pragma once
#define PI 3.141529f

#include <cmath>
#include <iostream>

struct vec2 {
  float x;
  float y;

  vec2 operator+(const vec2 &n) const { return {x + n.x, y + n.y}; };

  vec2 operator-(const vec2 &n) const { return {x - n.x, y - n.y}; };

  vec2 operator*(const float n) const { return {x * n, y * n}; };

  float dot(const vec2 &n) const { return (n.x * x) + (n.y * y); };
  float magnitude() const { return std::sqrt(x * x + y * y); };

  vec2 normalize() const {
    float mag = magnitude();

    return {x / mag, y / mag};
  }

  void print() const { std::cout << "{" << x << ", " << y << "} \n"; }
};
