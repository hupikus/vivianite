#include <raylib.h>

#include <memory>

#include "compositor.h"

Compositor::Compositor()
{

}

void Compositor::Init()
{
    layout.emplace_back(
    new Tile
    {
        .tab = true,
        .vertical = false,
        .ratio = 0.5f,
        .name = "Editor",
        //.instance = (Tab*)(new Editor())
        .instance = std::make_unique<Editor>()
    });

    for (const auto& tab : layout)
    {
        if (tab->tab)
        {
            tab->instance->Start();
        }
    }
}


void Compositor::Render()
{
    width = GetRenderWidth();
    height = GetRenderHeight();

    deltatime = GetFrameTime();


    for (const auto& tab : layout)
    {
        if (tab->tab)
        {
            const auto& instance = tab->instance;
            instance->Render(0, 0, width, height, deltatime);
            instance->Process(deltatime);
        }
    }
}

void Compositor::Process()
{

}
