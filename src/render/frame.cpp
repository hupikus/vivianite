#include "vivianite.h"

SDL_Texture *CreateTexture(SDL_Renderer *r, size_t width, size_t height)
{
    SDL_Texture *t = SDL_CreateTexture(
        r,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        width,
        height);

    return t;
}
