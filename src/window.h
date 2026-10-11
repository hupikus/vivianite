#ifndef WINDOW_H
#define WINDOW_H

#include <stdbool.h>
#include <SDL3/SDL.h>

class Window
{
private:
    size_t width, height;

    SDL_Window *window;
    SDL_Renderer *renderer;
public:
    const char *window_root = NULL;
    bool cli_render_mode = false;

    Window(int window_width, int window_height);

    int Init();

    void Resize(int width, int height);

    void Loop();

    ~Window();
};

#endif
