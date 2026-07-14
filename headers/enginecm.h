#pragma once

#include <algorithm>
#include <iostream>

#include <cmath>
#include <cstdlib>
#include <list>
#include <ratio>
#define GLEW_STATIC
#define GLEW_NO_GLU
#define GLEW_USE_GL_CORE_PROFILE // Force core profile
#include <GL/glew.h>

#include "map.h"
#include "math.h"
#include "player.h"
#include "raycaster.h"
#include "render.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <thread>
#include <vector>
