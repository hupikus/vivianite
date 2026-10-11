#include "tiling/tile.h"
#include "compositor.h"

static Tile *root = nullptr;

SDL_Rect rootRect {0, 0, 1, 1};

static bool update_dll = false;


void InitCompositor(const char *root_path, bool cli_render_mode)
{
    if (root) { delete root; }
    root = new Tile();
    root->horizontal = false;

    Tile *startscreen = new Tile("Vivianite", root_path, true);
    root->first = startscreen;

    if (cli_render_mode) {
        Tile *buffer = new Tile("Buffer view", BUFFER_VIEW_MODULE, true);
        root->second = buffer;

        root->ratio = 0.5;
    }
}

void DrawFrame(SDL_Renderer *renderer, int width, int height)
{
    SDL_SetRenderDrawColor(renderer, 38, 35, 42, 255);
    SDL_RenderClear(renderer);


    rootRect.w = width;
    rootRect.h = height;
    if (root) { root->Draw(renderer, rootRect); }

    update_dll = false;

}

void DestroyCompositor()
{
    if (root) { delete root; }
}
