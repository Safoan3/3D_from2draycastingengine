#include <cmath>
#include <cstdlib>
#include <list>
#include <ratio>
#define GLEW_STATIC
#define GLEW_NO_GLU
#define GLEW_USE_GL_CORE_PROFILE // Force core profile
#include <GL/glew.h>

#include "../headers/map.h"
#include "../headers/math.h"
#include "../headers/player.h"
#include "../headers/raycaster.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <thread>
#include <vector>

int resolution = 256;
float farmap[256];

float resf = (float)res;
float cellSize = 1.0f / resf;
float margin = 0.03f / resf;

void render2D() {
  for (int y = 0; y < res; y++) {
    for (int x = 0; x < res; x++) {
      glBegin(GL_QUADS);
      glColor3f(0.0f, 0.0f, 0.0f);
      if (map[res * y + x] == 1) {
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
};

void render3D() {
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
    float dist = farmap[i] * 4;
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
    float shade = 1.0f / (1.0f + dist * 0.6f);

    glBegin(GL_QUADS);
    glColor3f(0.8f * shade, 0.8f * shade, 0.8f * shade);
    glVertex2f(x1, bottom);
    glVertex2f(x2, bottom);
    glVertex2f(x2, top);
    glVertex2f(x1, top);
    glEnd();
  }
}

player haaland = {"haaland", 100};

// 1366,768
//

int maxfps = 60;
float waitTime = (1 / (float)maxfps);
int screenWidth = 1366;
int screenHeight = 768;

int main() {
  glfwInit();
  GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "raycaster",
                                        nullptr, nullptr);
  glfwMakeContextCurrent(window);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 1, 0, 1, 0, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  haaland.position = {0.25, 0.25};
  haaland.direction = {0, 1};

  double delta = 0.0f;

  while (!glfwWindowShouldClose(window)) {
    auto startClock = std::chrono::high_resolution_clock::now();

    glClear(GL_COLOR_BUFFER_BIT);

    // player input handing
    float walkSpeed = (float)haaland.walkspeed / 1500.0f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      haaland.position =
          haaland.position + (haaland.direction * walkSpeed * delta * 30);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      haaland.position =
          haaland.position - (haaland.direction * walkSpeed * delta * 30);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      vec2 modDir = haaland.direction;
      modDir.x = -haaland.direction.y;
      modDir.y = haaland.direction.x;

      haaland.position = haaland.position + (modDir * walkSpeed * delta * 30);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      vec2 modDir = haaland.direction;
      modDir.x = -haaland.direction.y;
      modDir.y = haaland.direction.x;

      haaland.position = haaland.position - (modDir * walkSpeed * delta * 30);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
      haaland.angle = haaland.angle - walkSpeed * 6.0f * delta * 30.0f;
      haaland.direction = {cosf(haaland.angle), sinf(haaland.angle)};
      haaland.direction.normalize();
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
      haaland.angle = haaland.angle + walkSpeed * 6.0f * delta * 30.0f;
      haaland.direction = {cosf(haaland.angle), sinf(haaland.angle)};
      haaland.direction.normalize();
    };
    //////////////////////////////////////

    render3D();
    // render2D();
    int max = resolution / 2;
    int index = 0;

    for (int i = -max; i <= max; i++) {
      float ii = (float)i / (float)max;
      float reqAng = ii * (PI / 6.0f);
      vec2 dir = {cosf(reqAng + haaland.angle), sinf(reqAng + haaland.angle)};

      ray newRay = {haaland.position, dir};
      raycastResult hitResult = StartRaycast(newRay);

      farmap[index] = hitResult.distance * cosf(reqAng);
      /*
            glBegin(GL_LINES);
            glColor3f(0.0f, 1.0f, 1.0f);
            glVertex2f(newRay.origin.x, newRay.origin.y);
            glVertex2f(hitResult.hit.x, hitResult.hit.y);
            glEnd();
      */
      index++;
      if (index > resolution) {
        index = resolution;
      }
    };

    glfwSwapBuffers(window);
    glfwPollEvents();

    std::this_thread::sleep_for(
        std::chrono::milliseconds((int)(waitTime * 1000)));
    auto endClock = (std::chrono::high_resolution_clock::now() - startClock);

    delta = std::chrono::duration<double>(endClock).count();
  }

  return 0;
}
