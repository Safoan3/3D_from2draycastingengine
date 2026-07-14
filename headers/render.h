#pragma once
#include "map.h"
#include "player.h"
#include "raycaster.h"

struct farmapEntry {
  vec2 noormal;
  float dist;
};
inline int resolution = 64;
inline farmapEntry farmap[64];

void initRayacasts(player &plr);
void render2D(player *plr);
void render3D(player *plr);
