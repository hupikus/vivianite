#include <raylib.h>

#include <cstddef>

#include "ui_loop.h"
#include "compositor.h"

UILoop::UILoop(size_t width, size_t height)
{
    compositor = std::make_unique<Compositor>();

    process_pause = std::chrono::milliseconds(10);
}

void UILoop::Start()
{
    compositor->Init();

    double frametime = 1.0 / 60.0;

    // DisableEventWaiting();

    while (!WindowShouldClose() and active)
    {
        BeginDrawing();

        Process();

        Render();

        // PollInputEvents();

        // SwapScreenBuffer();
        // ClearBackground(BLACK);

        // WaitTime(frametime);

        EndDrawing();
    }

    active = false;
}

void UILoop::Render()
{
    compositor->Render();
}

void UILoop::Process()
{
    compositor->Process();
}

void UILoop::Abort()
{
}
