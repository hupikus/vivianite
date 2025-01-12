#define RAYLIB_DEBUG
#include "raylib.hpp"
#include <iostream>

#include "window.h"


#define WIDTH 900
#define HEIGHT 600
#define MIN_WIDTH 350
#define MIN_HEIGHT 220

#define MOVEMENT_MINIMAL_DELTA 0.08f


Window::Window(int window_width, int window_height)
{
  //preprefs
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);

  InitWindow(WIDTH, HEIGHT, "Hello raylib!");

  //prefs
  SetWindowMinSize(MIN_WIDTH, MIN_HEIGHT);
  //SetTargetFPS(60);
}

Window::~Window()
{
    CloseWindow();
    //delete editor;
}


void Window::InitUI()
{
    //editor = new Editor(WIDTH, HEIGHT);
    //editor->Init();
}

void Window::Render()
{
  while (!WindowShouldClose())
  {
 	BeginDrawing();

 	//editor->Render();

 	EndDrawing();
  }

  CloseWindow();
}