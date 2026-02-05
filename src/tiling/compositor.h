#include "tiling/tile.h"
#include <SDL3/SDL.h>

#ifndef COMPOSITOR_H
#define COMPOSITOR_H

void  InitCompositor();
void  DrawFrame(SDL_Renderer *renderer, int width, int height);
void  DestroyCompositor();

#endif
