#pragma once

#include <cstddef>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
private:
    size_t width, height;

    GLFWwindow* window;
public:
    Window(int window_width, int window_height);

    int Init();

    int Loop();

    ~Window();
};
