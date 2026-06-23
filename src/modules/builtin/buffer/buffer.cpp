#include "vivianite.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>

extern "C" void tile_init()
{
}


extern "C" void tile_draw(SDL_Renderer *r, int w, int h)
{
    // Draw a current frame
    // SDL_SetRenderDrawColor(r, 175, 35, 42, 255);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
    SDL_RenderFillRect(r, NULL);

}

extern "C" void tile_destroy()
{


}
