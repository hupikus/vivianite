#include "vivianite.h"

#include <unistd.h>
#include <dlfcn.h>

#include "tiling/tile.h"

static const char *init_symbol = "tile_init";
static const char *draw_symbol = "tile_draw";
static const char *destroy_symbol = "tile_destroy";

typedef void (*draw_func_t) (
    SDL_Renderer*,
    int width,
    int height
);

void dllfail(SDL_Renderer *r, int width, int height);

void Tile::DlLoad(void)
{
    if (dlPath.empty()) { return; }

    if (handle != NULL) {
        content = false;
        if (destroyFunc != NULL) { destroyFunc(); }
        dlclose(handle);
        dlLoaded = false;
    }

    if (access(dlPath.c_str(), F_OK) == -1) {
        printf("Library's file doesn't exist or has a bad descriptor.\n");
        drawFunc = dllfail;
        content = true;
        return;
    }

    void *new_handle = dlopen(dlPath.c_str(), RTLD_NOW);
    if (new_handle == NULL) {
        printf("failed to open %s: %s\n", dlPath.c_str(), dlerror());
        drawFunc = dllfail;
        content = true;
        return;
    }

    draw_func_t new_func = (draw_func_t)dlsym(new_handle, draw_symbol);
    if (new_func == NULL) {
        printf("Failed to open %s from %s: %s.\n", draw_symbol, dlPath.c_str(), dlerror());
        drawFunc = dllfail;
        content = true;
        dlLoaded = false;
        return;
    }

    // Accept library after draw function was found
    handle = std::move(new_handle);
    dlLoaded = true;
    drawFunc = new_func;
    content = true;

    void (*new_init_func)(void) = NULL;
    void (*new_destroy_func)(void) = NULL;

    // _init
    new_init_func = (void(*)(void))dlsym(handle, init_symbol);
    if (new_init_func == NULL) {
        printf("Failed to open %s from %s: %s.\n", init_symbol, dlPath.c_str(), dlerror());
    } else {
        initFunc = new_init_func;
    }

    // _destroy
    new_destroy_func = (void(*)(void))dlsym(handle, destroy_symbol);
    if (new_destroy_func == NULL) {
        printf("Failed to open %s from %s: %s.\n", destroy_symbol, dlPath.c_str(), dlerror());
    } else {
        destroyFunc = new_destroy_func;
    }

    printf("Module %s's draw function loaded as %p\n", name.c_str(), drawFunc);
    initFunc();

}
