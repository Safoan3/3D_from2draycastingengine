#pragma once
#include "../headers/math.h"

struct player {
  std::string name;
  int health = 100;

  vec2 direction = {1, 0};
  vec2 position = {0, 0};
  int walkspeed = 5;
  float size = 0.02f;
  float angle = PI / 2.0f;

  void takeDamage(int num) { health = health - num; }
};
