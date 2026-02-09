#include "vivianite.h"
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

TTF_Font *font = TTF_OpenFont("assets/fonts/Menlo.ttf", 48);
void dllfail(SDL_Renderer *r, int w, int h)
{
    float scale = (float)w * 0.9f / (float)(22*8);
    SDL_SetRenderScale(r, scale, scale);
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    SDL_RenderDebugText(
        r,
        (w * 0.05f) / scale,
        (h / 2.0f - 4.0f * scale) / scale,
        "Failed to load plugin."
    );
}
