#include <unistd.h>
#include <dlfcn.h>
//#include <boost/dll/import.hpp>
//#include <boost/dll/shared_library.hpp>

#include "vivianite.h"
#include "tiling/tile.h"

#define SKIP_BOOST

static const char *init_symbol = "tile_init";
static const char *draw_symbol = "tile_draw";
static const char *destroy_symbol = "tile_destroy";

typedef void (*draw_func_t) (
    SDL_Renderer*,
    int width,
    int height
);

void dllfail(SDL_Renderer *r, int width, int height);

#ifdef SKIP_BOOST
void Tile::DlLoad()
{
    if (dlPath.empty()) return;

    if (handle != NULL) {
        content = false;
        if (destroyFunc != NULL) destroyFunc();
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
    if (new_handle == NULL)
    {
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
    if (new_init_func == NULL)
    {
        printf("Failed to open %s from %s: %s.\n", init_symbol, dlPath.c_str(), dlerror());
    } else {
        initFunc = new_init_func;
    }

    // _destroy
    new_destroy_func = (void(*)(void))dlsym(handle, destroy_symbol);
    if (new_destroy_func == NULL)
    {
        printf("Failed to open %s from %s: %s.\n", destroy_symbol, dlPath.c_str(), dlerror());
    } else {
        destroyFunc = new_destroy_func;
    }

    printf("Module %s's draw function loaded as %p\n", name.c_str(), drawFunc);

}
#else
void Tile::DlLoad()
{
    if (dlPath.empty()) return;

    if (lib) {
        content = false;
        if (destroyFunc) destroyFunc();
        dlLoaded = false;
        lib.reset();
    }

    if (access(dlPath.c_str(), F_OK) == -1) {
        printf("Library's file doesn't exist or has a bad descriptor.\n");
        drawFunc = dllfail;
        content = true;
        return;
    }

    std::unique_ptr<boost::dll::shared_library> new_lib;
    try {
        new_lib = std::make_unique<boost::dll::shared_library>(
            dlPath,
            boost::dll::load_mode::rtld_lazy
        );
    } catch (const std::exception& e)
    {
        printf("failed to open %s: %s\n", dlPath.c_str(), e.what());
        drawFunc = dllfail;
        content = true;
        return;
    }

    if (!new_lib->has(draw_symbol)) {
        printf("No %s in %s.\n", draw_symbol, dlPath.c_str());
        drawFunc = dllfail;
        content = true;
        return;
    }
    draw_func_t new_func = new_lib->get<draw_func_t>(draw_symbol);
    if (!new_func) {
        dlLoaded = false;
        return;
    }
    // Accept library after draw function was found
    lib = std::move(new_lib);
    dlLoaded = true;
    drawFunc = new_func;
    content = true;

    void (*init_func)(void) = nullptr;
    void (*destroy_func)(void) = nullptr;

    // _init
    if (lib->has(init_symbol)) {
        try {
            init_func = lib->get<void (*)(void)>(init_symbol);
        } catch (const std::exception& e)
        {
            init_func = nullptr;
            printf("%s", e.what());
        }
    }
    if (init_func) {
        initFunc = init_func;
    } else {
        printf("%s symbol is missing or is invalid, ignoring\n", init_symbol);
    }

    // _destroy
    if (lib->has(destroy_symbol)) {
        try {
            destroy_func = lib->get<void (*)(void)>(destroy_symbol);
        } catch (const std::exception& e)
        {
            destroy_func = nullptr;
            printf("%s", e.what());
        }
    }
    if (destroy_func) {
        destroyFunc = destroy_func;
    } else {
        printf("%s symbol is missing or is invalid, ignoring\n", destroy_symbol);
    }

    printf("Module %s's draw function loaded as %p\n", name.c_str(), drawFunc);

}
#endif

