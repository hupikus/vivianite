#pragma once

#include <cstddef>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

class Window
{
private:
    size_t width, height;

    SDL_Window *window;
    SDL_Renderer *renderer;
public:
    Window(int window_width, int window_height);

    int Init();

    void Loop();

    ~Window();
};
