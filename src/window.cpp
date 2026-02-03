#include <cstddef>
//#include <string>
#include <cstring>

#include <sstream>
#include <vector>
#include <memory>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

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

    return 0;
}

#include <unistd.h>
void Window::Loop()
{
    SDL_Event event;
    bool running = true;


    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 38, 35, 42, 255);
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
    }
}

Window::~Window()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

