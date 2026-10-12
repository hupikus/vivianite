#include "tiling/tile.h"
#include "compositor.h"

static Tile *root = nullptr;

SDL_Rect rootRect {0, 0, 1, 1};

static bool update_dll = false;


void InitCompositor(const char *root_path, bool cli_render_mode)
{
    if (root != NULL) { delete root; }
    root = new Tile();
    root->horizontal = false;

    size_t path_len = strlen(root_path) + 11;
    char root_dl_path[path_len];
    snprintf(root_dl_path, path_len, "%s/module.so", root_path);

    Tile *startscreen = new Tile("Vivianite", root_dl_path, true);
    root->first = startscreen;

    if (cli_render_mode) {
        path_len = strlen(BUFFER_VIEW_MODULE) + 11;
        char buffer_path[path_len];
        snprintf(buffer_path, path_len, "%s/module.so", BUFFER_VIEW_MODULE);

        Tile *buffer = new Tile("Buffer view", buffer_path, true);
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
    if (root != NULL) { delete root; }
}
