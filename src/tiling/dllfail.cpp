#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

TTF_Font *font = TTF_OpenFont("assets/fonts/Menlo.ttf", 48);
void dllfail(SDL_Renderer *r, SDL_Rect &rect)
{
    // TTF_TextEngine *engine = TTF_CreateRendererTextEngine(r);
    // TTF_Text *text = TTF_CreateText(engine, font, "Failed to load plugin.", 22);
    // TTF_SetTextColor(text, 255, 255, 255, 255);

    // int w, h;
    // TTF_GetTextSize(text, &w, &h);
    // TTF_DrawRendererText(text, (surface.w - w) / 2.0f, (surface.h - h) / 2.0f);

    // TTF_DestroyText(text);
    // TTF_DestroyRendererTextEngine(engine);

    float scale = (float)rect.w * 0.9f / (float)(22*8);
    SDL_SetRenderScale(r, scale, scale);
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    SDL_RenderDebugText(
            r,
            (rect.w * 0.05f) / scale,
            (rect.h / 2.0f - 4.0f * scale) / scale,
            "Failed to load plugin."
    );
}
