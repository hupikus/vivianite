#include "tiling/tile.h"

#include <dlfcn.h>
#include <SDL3/SDL.h>

Tile::Tile() {}

Tile::Tile(std::string tileDlpath)
{
    dlPath = tileDlpath;
}

Tile::Tile(std::string tileName, std::string tileDlpath, bool load)
{
    name = tileName;
    dlPath = tileDlpath;
    if (load) DlLoad();
}


void Tile::Draw(SDL_Renderer *r, SDL_Rect &surface)
{
    //printf("draw %s: %i, %i, %i, %i\n", name.c_str(), surface.w, surface.h, surface.x, surface.y);
    if (IsKeyPressed(SDL_SCANCODE_R)) { DlLoad(); }
    if (content && drawFunc) {
        drawFunc(r, surface.w, surface.h);
    } else {
        SDL_Rect firstRect = surface;

        if (first) {
            SDL_SetRenderClipRect(r, NULL);
            SDL_SetRenderViewport(r, NULL);

            // Set viewport rect;
            // In its coordinates, clip the viewport.
            // Order matters.

            SDL_SetRenderViewport(r, &firstRect);
            SDL_SetRenderClipRect(r, &firstRect);

            if (horizontal) {
                firstRect.w *= ratio;
            } else {
                firstRect.h *= ratio;
            }

            firstRect.x = 0;
            firstRect.y = 0;
            

            first->Draw(r, firstRect);
        }


        if (second) {
            SDL_Rect secondRect = surface;

            SDL_SetRenderClipRect(r, NULL);
            SDL_SetRenderViewport(r, NULL);


            if (horizontal) {
                int shift = secondRect.w * ratio;
                secondRect.w -= shift;
                secondRect.x = shift;
            } else {
                int shift = secondRect.h * ratio;
                secondRect.h -= shift;
                secondRect.y = shift;
            }

            SDL_SetRenderViewport(r, &secondRect);

            // We need to preserve rect's global position
            // Because we set clip rect in global coordinates.
            // Instead of trying to guess tile draw order.

            int old_x = secondRect.x;
            int old_y = secondRect.y;
            secondRect.x = 0;
            secondRect.y = 0;

            SDL_SetRenderClipRect(r, &secondRect);

            secondRect.x = old_x;
            secondRect.y = old_y;

            second->Draw(r, secondRect);
        }
    }
}



Tile::~Tile()
{
    if (destroyFunc) {
        content = false;
        destroyFunc();
    }
    if (dlLoaded) {
        dlclose(handle);
    }
    if (first) delete first;
    if (second) delete second;
    printf("destroying %s.\n", name.c_str());
}

