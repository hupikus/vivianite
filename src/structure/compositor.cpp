#include <raylib.h>

#include <memory>

#include "compositor.h"

Compositor::Compositor()
{

}

void Compositor::Init()
{
    tabs.push_back(CompositorTab{
        .id = 0,
        .name = "Editor",
        .instance = std::unique_ptr<Tab>(new Editor())
        });

    tabs[0].instance->Start();
}


void Compositor::Render()
{
    width = GetRenderWidth();
    height = GetRenderHeight();

    deltatime = GetFrameTime();


    for (CompositorTab& tab : tabs)
    {
        tab.instance->Render(width, height, deltatime);
    }
}
