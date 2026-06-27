#include "vivianite.h"
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

TTF_Font *font = TTF_OpenFont("assets/fonts/Menlo.ttf", 48);
void dllfail(SDL_Renderer *r, int w, int h)
{
    SDL_SetRenderDrawColor(r, 42, 35, 36, 255);
    SDL_RenderFillRect(r, NULL);

    SDL_SetRenderDrawColor(r, 255, 23, 25, 255);
    SDL_RenderDebugText(
        r,
        0, 0,
        "Failed to load plugin."
    );
}
