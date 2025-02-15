#define RAYLIB_DEBUG
#include <raylib.h>
//#include <iostream>

#include "window.h"

#include "editor.h"


#define WIDTH 900
#define HEIGHT 600
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
    SetWindowIcon(LoadImage("vivianite.png"));
    SetWindowMinSize(MIN_WIDTH, MIN_HEIGHT);
    SetTargetFPS(60);
    SetExitKey(0);

}

Window::~Window()
{
    if (IsWindowReady()) { CloseWindow(); }
    delete editor;
}


void Window::InitUI()
{
    editor = new Editor(WIDTH, HEIGHT);
    editor->Init();
}

void Window::Render()
{
    while (!WindowShouldClose())
    {
        BeginDrawing();

        editor->Render();

        EndDrawing();
    }

  CloseWindow();
}
