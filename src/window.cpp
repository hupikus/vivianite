#define RAYLIB_DEBUG
#include <raylib.h>

#include "window.h"

#define MIN_WIDTH 350
#define MIN_HEIGHT 220



Window::Window(int window_width, int window_height)
{
    //preprefs
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    width = window_width;
    height = window_height;
    InitWindow(window_width, window_height, "Vivianite");

    //prefs
    SetWindowIcon(LoadImage("assets/vivianite.png"));
    SetWindowMinSize(MIN_WIDTH, MIN_HEIGHT);
    SetTargetFPS(240);
    SetExitKey(0);

}

Window::~Window()
{
    if (IsWindowReady()) { CloseWindow(); }
}

