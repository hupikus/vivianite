#include <raylib.h>

#include <memory>


#include "compositor.h"

Compositor::Compositor()
{
    // root_tile = std::unique_ptr<Tile>(
    //     new Tile
    //     {
    //         .vertical = false,
    //         .ratio = 0.5f,
    //     });
}

void Compositor::Init()
{
    root_tile = std::unique_ptr<Tile>(
    new Tile
    {
        .tab = true,
        .name = "Editor",
        .instance = std::make_unique<Editor>()
    });

    chroot(root_tile, 0, 0, 0, 0, true);

    focus_tile = &root_tile;
}

void Compositor::Process()
{
    width = GetRenderWidth();
    height = GetRenderHeight();

    deltatime = GetFrameTime();

    click = IsMouseButtonPressed(0);
    if (click)
    {
        click_x = GetMouseX();
        click_y = GetMouseY();
    }

    //key process
    keys();
}

void Compositor::keys()
{

    bool ctrl = IsKeyDown(KEY_LEFT_CONTROL) or IsKeyDown(KEY_RIGHT_CONTROL);
    bool shift = IsKeyDown(KEY_LEFT_SHIFT) or IsKeyDown(KEY_RIGHT_SHIFT);
    bool alt = IsKeyDown(KEY_LEFT_ALT) or IsKeyDown(KEY_RIGHT_ALT);


    //Tiling
    if(IsKeyPressed(KEY_N) and ctrl)
    {
        //1. copy pointer
        std::unique_ptr<Tile>* moved_tile = focus_tile;
        //2.create new tab (with the same type) and start it
        std::unique_ptr<Tile> created_tile = std::unique_ptr<Tile>(
            new Tile
            {
                .tab = true,
                .name = (*moved_tile)->name,
                .instance = (*moved_tile)->instance->NewInstance()
            });

        created_tile->instance->Start();

        //mess
        //using TileType = std::remove_reference_t<decltype(*(created_tile->instance))>;
        //std::unique_ptr<Tab> tab = std::make_unique<TileType>();
        //end of the mess

        //3. create new tile and fill layout
        *focus_tile = std::unique_ptr<Tile>(
            new Tile
            {
                .vertical = shift,
                .ratio = 0.5f,
                .layout = alt ?
                std::array<std::unique_ptr<Tile>, 2> {std::move(created_tile), std::move(*moved_tile)} :
                std::array<std::unique_ptr<Tile>, 2> {std::move(*moved_tile), std::move(created_tile)}
            });
        //4.set focus tab to the initial one
        focus_tile = &(*focus_tile)->layout[alt];
    }

    //focus
    if(IsKeyPressed(KEY_ESCAPE)) { focus_tile = nullptr; }
}


void Compositor::Render()
{
    BeginScissorMode(0, 0, width, height);

    chroot(root_tile, 0, 0, width, height, false);

    EndScissorMode();
}



void Compositor::chroot(std::unique_ptr<Tile>& root, int posx, int posy, size_t width, size_t height, bool start)
{
    if (root->tab)
    {
        const auto& instance = root->instance;

        if (start) { instance->Start(); }
        else
        {

            BeginScissorMode(posx, posy, width, height);
            instance->Render(posx, posy, width, height, deltatime);

            if (focus_tile != &root)
            {
                 DrawRectangle(posx, posy, width, height, inactive);

                 if (click)
                 {
                     if (click_x > posx and click_x < posx + width and click_y > posy and click_y < posy + height)
                     {
                         focus_tile = &root;
                         SetMouseCursor(0);
                     }
                     click = false;
                 }
            }
            else
            {
                instance->Input(posx, posy, width, height, deltatime);
            }
        }
    }
    else //tile
    {
        int new_posx = posx;
        int new_posy = posy;
        size_t new_width;
        size_t new_height;


        //count width and height
        if (root->vertical)
        {
            new_height = root->ratio * height;
            new_width = width;
        }
        else
        {
            new_width = root->ratio * width;
            new_height = height;
        }


        if (root->layout[0] != nullptr and root->ratio > 0.0f)
        {
            chroot(root->layout[0], new_posx, new_posy, new_width, new_height, start);
        }

        if ( !(root->ratio < 1.0f) ) return;

        auto& tab = root->layout[1];

        if (tab != nullptr)
        {
            //substract remains
            if (root->vertical)
            {
                new_posy += new_height;
                new_height = height - new_height;
            }
            else
            {
                new_posx += new_width;
                new_width = width - new_width;
            }

            chroot(tab, new_posx, new_posy, new_width, new_height, start);
        }
    }
}

