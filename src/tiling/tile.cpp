#include "tiling/tile.h"
#include <SDL3/SDL.h>

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


void Tile::Draw(SDL_Renderer *r, SDL_Rect& surface)
{
    //printf("draw %s.\n", name.c_str());
    if (content && drawFunc) {
        //DlLoad();
        drawFunc(r, surface);
    } else {
        int shift = 0;
        int size = 0;
        if (first) {
            if (second) {
                // Split tile if second exists
                if (horizontal) {
                    size = surface.w;
                    shift = size * ratio;
                    surface.w = shift;
                } else {
                    size = surface.h;
                    shift = size * ratio;
                    surface.h = shift;
                }
                SDL_SetRenderViewport(r, &surface);
            }

            first->Draw(r, surface);

            if (second) {
                if (horizontal) {
                    surface.x += shift;
                    surface.w = size - shift;
                } else {
                    surface.y += shift;
                    surface.h = size - shift;
                }
            }
        }
        if (second) {
            SDL_SetRenderViewport(r, &surface);
            second->Draw(r, surface);
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
        lib.reset();
    }
    if (first) delete first;
    if (second) delete second;
}

