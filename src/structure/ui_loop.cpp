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


    while (!WindowShouldClose() and active)
    {
        Render();
        //Process();
    }

    active = false;
}


void UILoop::Render()
{
        BeginDrawing();

        compositor->Render();

        EndDrawing();
}

//empty
void UILoop::Process()
{
    compositor->Process();
}


void UILoop::Abort()
{

}
