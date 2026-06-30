#include "vivianite.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>

extern "C" void tile_init()
{
}


extern "C" void tile_draw(SDL_Renderer *r, int w, int h)
{
    // Draw a current frame
    // SDL_SetRenderDrawColor(r, 175, 35, 42, 255);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
    SDL_RenderFillRect(r, NULL);

    videoCache *video = GetActiveJob();
    if (video == NULL) return;
    SDL_Texture *t = GetRenderPreview(video);
    if (t == NULL) return;

    float video_w = (float)video->width;
    float video_h = (float)video->height;

    float scale_x = (float)w / video_w;
    float scale_y = (float)h / video_h;
    float scale = scale_x < scale_y ? scale_x : scale_y;

    SDL_FRect dst = {
        .w = video_w * scale,
        .h = video_h * scale,
    };

    dst.x = (w - dst.w) * 0.5f;
    dst.y = (h - dst.h) * 0.5f;

    SDL_RenderTexture(r, t, NULL, &dst);
}

extern "C" void tile_destroy()
{


}
