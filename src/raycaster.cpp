#include "../headers/raycaster.h"
#include "../headers/map.h"
#include "../headers/math.h"
#include "../headers/raycaster.h"
#include <iostream>
#include <random>
#include <system_error>

raycastResult StartRaycast(ray r) {
  // horizontal detection//boot/EFIUEFI-Shell/
  float maxHeight = 1;
  float maxWidth = 1;
  float resf = (float)mapX.size;
  float cellSize = 1.0f / resf;

  float currentPx = r.origin.y;
  float div = std::fmod(currentPx, cellSize);
  float remainings = cellSize - div;
  float slop = r.direction.x / r.direction.y;

  vec2 hitPosition = {0, 0};
  bool wallhit = false;

  raycastResult Result;
  float normalX = 0;
  float nornalY = 0;

  for (int i = 0; i < mapX.size * 2 && !wallhit; i++) {
    float tostore = (currentPx + remainings) + (i * cellSize);
    float tostorenegative =
        (currentPx - (cellSize - remainings)) - (i * cellSize);
    if (r.direction.y < 0.0f) {
      tostore = tostorenegative;
    }

    //   listhorizontal.push_back(tostore);
    vec2 cp = {r.origin.x + ((tostore - r.origin.y) * slop), tostore};

    int cellPos = (int)(tostore / cellSize);
    if (r.direction.y < 0.0f) {
      cellPos = cellPos - 1;
      nornalY = -1;

    } else {
      nornalY = 1;
    }

    // std::cout << cellPos << std::endl;

    for (int z = 0; z < mapX.size; z++) {
      if (cp.x > (cellSize * z) && cp.x < (cellSize * (z + 1)) &&
          mapX.mapinfo[cellPos * mapX.size + z] != 0 && wallhit == false) {
        // std::cout << "potential hit detecteon at " << z << std::endl;
        wallhit = true;
        break;
      }
    }

    if (wallhit) {
      //     glPointSize(5.0f); // 5 pixels wide
      //    glBegin(GL_POINTS);

      hitPosition = cp;
      //   glVertex2f(cp.x, cp.y);
      //  glEnd();
    }

    // cp.print();
  }
  float startX = r.origin.x;                      // NEW: was currentPx
  float remainderX = std::fmod(startX, cellSize); // NEW: was div
  float distanceToNextX = cellSize - remainderX;  // NEW: was remainings

  float slopeXY = r.direction.y / r.direction.x; // NEW: was slop (dy/dx)

  vec2 wallHitPos = {0, 0}; // NEW: was hitPosition
  bool wallFound = false;   // NEW: was wallhit

  for (int step = 0; step < mapX.size * 2 && !wallFound; step++) { // NEW: was i
    float gridX =
        (startX + distanceToNextX) + (step * cellSize); // NEW: was tostore
    float gridXNegative = (startX - (cellSize - distanceToNextX)) -
                          (step * cellSize); // NEW: was tostorenegative

    if (r.direction.x < 0.0f) {
      gridX = gridXNegative;
    }

    // Calculate Y from X
    vec2 intersectPoint = {
        gridX, r.origin.y + ((gridX - r.origin.x) * slopeXY)}; // NEW: was cp

    int cellIndexX = (int)(gridX / cellSize); // NEW: was cellPos
    if (r.direction.x < 0.0f) {
      cellIndexX = cellIndexX - 1;
      normalX = -1;

    } else {
      normalX = 1;
    }

    for (int cellY = 0; cellY < mapX.size; cellY++) { // NEW: was z
      // Check if intersectPoint.y is in cell cellY
      if (intersectPoint.y > (cellSize * cellY) &&
          intersectPoint.y < (cellSize * (cellY + 1)) &&
          mapX.mapinfo[cellY * mapX.size + cellIndexX] != 0 &&
          wallFound == false) {
        wallFound = true;
        break;
      }
    }

    if (wallFound) {
      // verticalIntersections.push_back(gridX);
      //   glPointSize(5.0f);
      //   glBegin(GL_POINTS);
      wallHitPos = intersectPoint;
      //   glVertex2f(intersectPoint.x, intersectPoint.y);
      // glEnd();
    }
  }

  Result.hit = {0, 0};
  float mag1 = (hitPosition - r.origin).magnitude();
  float mag2 = (wallHitPos - r.origin).magnitude();

  // hitPosition.print();
  // wallHitPos.print();
  // std::cout << mag1 << " " << mag2 << "\n";

  // FIRST: Check if either is zero

  bool hValid = (hitPosition.magnitude() != 0);
  bool vValid = (wallHitPos.magnitude() != 0);

  // Result.normal = {-normalX, -nornalY};

  vec2 yN = {0, -nornalY};
  vec2 xN = {-normalX, 0};

  if (!hValid || !vValid) {
    Result.casted = false;
  }

  if (hValid && vValid) {
    Result.hit = (mag1 < mag2) ? hitPosition : wallHitPos;

    Result.normal = (mag1 < mag2) ? yN : xN;
    Result.casted = true;

  } else if (hValid) {

    Result.hit = hitPosition;
    Result.casted = true;
    Result.normal = yN;

  } else if (vValid) {
    Result.hit = wallHitPos;
    Result.casted = true;
    Result.normal = xN;
  }

  if (Result.casted) {
    Result.distance = (Result.hit - r.origin).magnitude();

    if (Result.distance > 0.0f) {
      Result.r = 1.0f;
      Result.g = 1.0f;
      Result.b = 1.0f;
    }
  }

  return Result;
};
