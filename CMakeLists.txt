cmake_minimum_required(VERSION 3.10.0)

project(Raycast VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(imgui REQUIRED)
find_package(GLEW REQUIRED)

include_directories(headers)



add_executable(Raycast src/engine.cpp src/raycaster.cpp src/render.cpp)



target_link_libraries(Raycast 
    OpenGL::GL
    GLEW::GLEW
    glfw
    imgui::imgui
)
