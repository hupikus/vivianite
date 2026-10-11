#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#define STARTSCREEN_MODULE "modules/builtin/startscreen/module.so"
#define BUFFER_VIEW_MODULE "modules/builtin/buffer/module.so"

#include "tiling/tile.h"
#include <SDL3/SDL.h>

void  InitCompositor(const char *root_path, bool cli_render_mode);
void  DrawFrame(SDL_Renderer *renderer, int width, int height);
void  DestroyCompositor();

#endif // COMPOSITOR_H
