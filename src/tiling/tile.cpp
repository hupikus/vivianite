#include "tiling/tile.h"
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
    // printf("draw %s: %i, %i, %i, %i\n", name.c_str(), surface.w, surface.h, surface.x, surface.y);
    if (IsKeyPressed(SDL_SCANCODE_R)) { DlLoad(); }
    if (content && drawFunc) {
        drawFunc(r, surface.w, surface.h);
    } else {
        SDL_Rect firstRect = surface;

        if (first) {
            SDL_SetRenderClipRect(r, NULL);
            SDL_SetRenderViewport(r, NULL);


            if (horizontal) {
                firstRect.w *= ratio;
            } else {
                firstRect.h *= ratio;
            }
            
            // Set viewport rect
            // In its coordinates set clip
            // Order matters

            SDL_SetRenderViewport(r, &firstRect);
            SDL_SetRenderClipRect(r, &firstRect);

            first->Draw(r, firstRect);
        }


        if (second) {
            SDL_Rect secondRect = surface;

            SDL_SetRenderClipRect(r, NULL);
            SDL_SetRenderViewport(r, NULL);


            if (horizontal) {
                int shift = secondRect.w * ratio;
                secondRect.w -= shift;
                secondRect.x += shift;
            } else {
                int shift = secondRect.h * ratio;
                secondRect.h -= shift;
                secondRect.y += shift;
            }

            // Have position 0 rect, becuase SetRenderClip works in Viewport coordinates
            firstRect = secondRect;
            firstRect.x = 0;
            firstRect.y = 0;
            SDL_SetRenderViewport(r, &secondRect);
            SDL_SetRenderClipRect(r, &firstRect);

            second->Draw(r, secondRect);
        }
    }
}



Tile::~Tile()
{
    printf("destroying %s.\n", name.c_str());
    if (destroyFunc) {
        content = false;
        destroyFunc();
    }
    if (dlLoaded) {
        dlclose(handle);
    }
    if (first) delete first;
    if (second) delete second;
}

