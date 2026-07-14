#pragma once

#include <vector>
struct map {
  int size;
  std::vector<int> mapinfo;

  map(int s, std::vector<int> data) {
    size = s;
    mapinfo = std::move(data);
  }
};

inline map mapX =
    map(8, {
               1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0,
               0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1,
               0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,

           });

// inline int res = 8;

// inline float resf = (float)res;

// inline float cellSize = 1.0f / resf;
