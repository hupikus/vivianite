#include <raylib.h>

#include <cstddef>
//#include <thread>

#include "ui_loop.h"
#include "compositor.h"


UILoop::UILoop(size_t width, size_t height)
{
    compositor = std::make_unique<Compositor>();

    process_pause = std::chrono::milliseconds(10);
}

void UILoop::Init()
{
    compositor->Init();

    //std::thread draw(&UILoop::Render, this);
    //std::thread process(&UILoop::Process, this);

    //draw.join();
    //process.detach();
    Render();

    //CloseWindow();
}


void UILoop::Render()
{
    while (!WindowShouldClose() and active)
    {
        BeginDrawing();

        compositor->Render();

        EndDrawing();
    }
    active = false;
}


void UILoop::Process()
{
    // while (active)
    // {
    //     std::this_thread::sleep_for(process_pause);
    // }
    //active = false;
}
