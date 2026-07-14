#include "../headers/render.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

float resf = (float)mapX.size;
float cellSize = 1.0f / resf;

void initRayacasts(player &plr) {
  for (int i = 0; i < resolution; i++) {
    float ii = ((float)i / (float)resolution) - 0.5f;
    float reqAng = ii * (PI / 6.0f);
    vec2 dir = {cosf(reqAng + plr.angle), sinf(reqAng + plr.angle)};

    ray newRay = {plr.position, dir};
    raycastResult hitResult = StartRaycast(newRay);

    // hitResult.normal.print();
    farmap[i].dist = hitResult.distance;
    farmap[i].noormal = hitResult.normal;
  };
}

void render2D(player *plr) {
  float margin = 0.03f / resf;

  for (int y = 0; y < mapX.size; y++) {
    for (int x = 0; x < mapX.size; x++) {
      glBegin(GL_QUADS);
      glColor3f(0.0f, 0.0f, 0.0f);
      if (mapX.mapinfo[mapX.size * y + x] == 1) {
        glColor3f(1.0f, 1.0f, 1.0f);
      }
      glVertex2f(0 + x * cellSize + margin, 0 + y * cellSize + margin);
      glVertex2f(cellSize + x * cellSize - margin, 0 + y * cellSize + margin);
      glVertex2f(cellSize + x * cellSize - margin,
                 cellSize + y * cellSize - margin);
      glVertex2f(0 + x * cellSize + margin, cellSize + y * cellSize - margin);
      glEnd();
    }
  }
  for (int i = 0; i < resolution; i++) {
    float ii = ((float)i / (float)resolution) - 0.5f;
    float reqAng = ii * (PI / 6.0f);

    vec2 dir = {cosf(reqAng + plr->angle), sinf(reqAng + plr->angle)};

    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 1.0f);

    glVertex2f(plr->position.x, plr->position.y);
    float dist = farmap[i].dist;
    glVertex2f(plr->position.x + dir.x * dist, plr->position.y + dir.y * dist);

    glEnd();
  }
};

void render3D(player *plr) {
  float cellsize = 1.0f / (float)resolution;

  // ==========================================
  // 1. DRAW CEILING (Full screen top half)
  // ==========================================
  glBegin(GL_QUADS);
  glColor3f(83.0f / 255.0f, 128.0f / 255.0f, 0.6f); // Dark blue
  glVertex2f(0, 1.0f);
  glVertex2f(1, 1.0f);
  glVertex2f(1, 0.5f);
  glVertex2f(0, 0.5f);
  glEnd();

  // ==========================================
  // 2. DRAW FLOOR (Full screen bottom half)
  // ==========================================
  glBegin(GL_QUADS);
  glColor3f(0.2f, 0.3f, 0.2f); // Dark brown
  glVertex2f(0, 0.0f);
  glVertex2f(1, 0.0f);
  glVertex2f(1, 0.5f);
  glVertex2f(0, 0.5f);
  glEnd();

  // ==========================================
  // 3. DRAW WALLS (ON TOP of ceiling/floor)
  // ==========================================

  for (int i = 0; i < resolution; i++) {
    float ii = ((float)i / (float)resolution) - 0.5f;
    float reqAng = ii * (PI / 6.0f);
    vec2 dirXX = {cosf(reqAng + plr->angle), sinf(reqAng + plr->angle)};

    float dist = farmap[i].dist * 4 * cosf(reqAng);

    if (dist < 0.01f)
      continue;

    float wallHeight = 0.3f / dist;

    if (wallHeight > 1.0f)
      wallHeight = 1.0f;

    float top = 0.5f + wallHeight / 2.0f;
    float bottom = 0.5f - wallHeight / 2.0f;

    float x1 = i * cellsize;
    float x2 = (i + 1) * cellsize;

    // DISTANCE SHADING (closer = brighter, farther = darker)
    float shade = 1.0f / (1.0f + dist * 0.3f);
    float dot = farmap[i].noormal.normalize().dot(dirXX.normalize());

    dot = dot * -1;

    float shadesssss;

    shadesssss = std::clamp(dot, 0.5f, 1.0f);
    shade = shade * shadesssss;

    glBegin(GL_QUADS);
    glColor3f(0.9f * shade, 0.5f * shade, 0.5f * shade);
    glVertex2f(x1, bottom);
    glVertex2f(x2, bottom);
    glVertex2f(x2, top);
    glVertex2f(x1, top);
    glEnd();
  }
}
