#include <iostream>
#include <cstddef>
//#include <string>
#include <cstring>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <OpenImageIO/imageio.h>

#include "window.h"

#define MIN_WIDTH 350
#define MIN_HEIGHT 220

Window::Window(int window_width, int window_height)
{
    width = window_width;
    height = window_height;
}

int Window::Init()
{
    if (!glfwInit()) { return -1; }

    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);


    window = glfwCreateWindow(width, height, "OPENGLOPENGLOPENGLOPENGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Unable to init GLAD" << std::endl;
        return -1;
    }


    //Platform
    int platform = glfwGetPlatform();
    //GLFW_PLATFORM_WIN32
    //GLFW_PLATFORM_COCOA
    //GLFW_PLATFORM_WAYLAND
    //GLFW_PLATFORM_X11
    //GLFW_PLATFORM_NULL

    std::cout << "Platform: " << platform << std::endl;

    // Props setup
    #if defined(WINDOWS)
    #include <windows.h>
    #endif

    #if defined(LINUX)
    #if defined(X11)
    #define GLFW_EXPOSE_NATIVE_X11
    #include <X11/Xlib.h>
    #include <X11/cursorfont.h>
    #endif
    #endif




    // Icon

    GLFWimage images[2];


    return 0;

}

int Window::Loop()
{
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.85f, 0.85f, 0.85f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

Window::~Window()
{

}
