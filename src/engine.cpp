#include "../headers/enginecm.h"

int maxfps = 24;

player haaland = {"haaland", 100};

// 1366,768

float waitTime = (1 / (float)maxfps);
int screenWidth = 700;  // 1366 / 2;
int screenHeight = 700; // 768 / 2;

int main() {
  glfwInit();
  GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "raycaster",
                                        nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 1, 0, 1, 0, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  haaland.position = {0.25, 0.25};
  haaland.direction = {0, 1};

  double delta = 0.0f;
  int frames = 0;
  if (glfwRawMouseMotionSupported())
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

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
    /////////////////////////////////

    // mouse
    static double prevX;
    double mousex, mousey;
    glfwGetCursorPos(window, &mousex, &mousey);
    glfwSetCursorPos(window, screenWidth / 2, screenHeight / 2);

    float changeX = ((((float)screenWidth / 2) - mousex) / 1000.0f);

    haaland.angle -= changeX;
    haaland.direction = {cosf(haaland.angle), sinf(haaland.angle)};
    haaland.direction.normalize();

    //////////////////////////////////////
    initRayacasts(haaland);
    render3D(&haaland);
    // render2D(&haaland);

    glfwSwapBuffers(window);
    glfwPollEvents();

    std::this_thread::sleep_for(
        std::chrono::milliseconds((int)(waitTime * 1000)));
    auto endClock = (std::chrono::high_resolution_clock::now() - startClock);

    delta = std::chrono::duration<double>(endClock).count();

    frames++;
    if (frames > maxfps) {
      frames = 0;
      // std::cout << "\r fps: " << 1.0f / delta << "\n";
    }
  }

  return 0;
}
