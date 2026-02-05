#include "tiling/tile.h"

static Tile *root = nullptr;

SDL_Rect rootRect {0, 0, 1, 1};

void InitCompositor()
{
    root = new Tile("Vivianite", "modules/std/vivianite.so");
}

void DrawFrame(SDL_Renderer *renderer, int width, int height)
{
    rootRect.w = width;
    rootRect.h = height;
    SDL_SetRenderViewport(renderer, &rootRect);
    if (root) root->Draw(renderer, rootRect);
    SDL_SetRenderViewport(renderer, &rootRect);

    SDL_SetRenderDrawColor(renderer, 38, 35, 42, 255);
    SDL_RenderClear(renderer);

}

void DestroyCompositor()
{
    if (root) delete root;
}
