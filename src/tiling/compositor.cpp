#include "tiling/tile.h"


static Tile *root = nullptr;

SDL_Rect rootRect {0, 0, 1, 1};

static bool update_dll = false;

void InitCompositor()
{
    root = new Tile("Vivianite", "src/modules/builtin/startscreen/module.so", true);
}

void DrawFrame(SDL_Renderer *renderer, int width, int height)
{
    SDL_SetRenderDrawColor(renderer, 38, 35, 42, 255);
    SDL_RenderClear(renderer);


    rootRect.w = width;
    rootRect.h = height;
    SDL_SetRenderViewport(renderer, &rootRect);
    if (root) root->Draw(renderer, rootRect);
    SDL_SetRenderViewport(renderer, &rootRect);

    update_dll = false;
}

void DestroyCompositor()
{
    if (root) delete root;
}
