#include <cstddef>
#include <string>

#include <sstream>
#include <vector>
#include <memory>

#include <SDL3/SDL.h>

#include "input/keys.h"
#include "tiling/compositor.h"

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
    if (!SDL_Init(SDL_INIT_VIDEO))
        throw std::runtime_error("Failed to initialize video.\n");

    window = SDL_CreateWindow("Vivianite", width, height, SDL_WINDOW_RESIZABLE);
    if (!window)
        throw std::runtime_error("Failed to create a window.\n");

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
        throw std::runtime_error("Failed to create a renderer.\n");

    SDL_SetRenderVSync(renderer, 1);

    SDL_Surface *icon = SDL_LoadPNG("assets/icon/vivianite.png");
    if (!icon) {
        SDL_Log("Failed to set icon.\n");
    } else {
        SDL_SetWindowIcon(window, icon);
        SDL_DestroySurface(icon);
    }

    SDL_ShowWindow(window);


    InitCompositor();

    return 0;
}

void Window::Loop()
{
    SDL_Event event;
    bool running = true;


    while (running) {
        updateKeys();
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                width = event.window.data1;
                height = event.window.data2;
                break;
            case SDL_EVENT_KEY_DOWN:
                  setKey(event.key.scancode, 1);
                  break;
            case SDL_EVENT_KEY_UP:
                  setKey(event.key.scancode, -1);
                  break;
            }
        }

        DrawFrame(renderer, width, height);

        SDL_RenderPresent(renderer);
    }
}

Window::~Window()
{
    DestroyCompositor();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

