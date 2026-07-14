#pragma once

#include "map.h"
#include "math.h"

struct ray {
  vec2 origin;
  vec2 direction;
};

struct raycastResult {
  float distance;
  vec2 normal;
  vec2 hit;
  bool casted = false;
  float r, g, b;
};

raycastResult StartRaycast(ray r);
