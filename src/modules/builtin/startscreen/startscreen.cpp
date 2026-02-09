#include "vivianite.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>
#include <boost/dll/alias.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_MULTIPLE_MASTERS_H

void render_text(SDL_Renderer *renderer);

FT_Library ft;
void init()
{
	FT_Init_FreeType(&ft);
}

void draw(SDL_Renderer *r, int w, int h)
{
	int size = 32;
	int x = 0;
	int y = 0;
	const char *fontfile = "/home/sipuchiy/cyth/vivianite/assets/fonts/Menlo.ttf";
	const char *text = "HIIIIIIIIIIIIIII:3";
	SDL_RenderFillRect(r, new (const SDL_FRect){w*0.25f, h*0.25f, w*0.75f, h*0.75f});

	float scale = (float)w * 0.9f / (float)(26*8);
	SDL_SetRenderScale(r, scale, scale);
	SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
	SDL_RenderDebugText(
		r,
		(w * 0.05f) / scale,
		(h / 2.0f - 4.0f * scale) / scale,
		"Not failed to load plugin."
	);

	// render_text(r);

 }

struct character_info {
    float ax, ay; // advance
    float bw, bh; // bitmap width/height
    float bl, bt; // bitmap left/top
    float tx;     // x offset in atlas
};

void render_text(SDL_Renderer *renderer) {
    const char *font_path = "/home/sipuchiy/cyth/vivianite/assets/fonts/Menlo.ttf"; // path to variable font
    const char *text = "Hello Variable Font!";
    int font_size = 32;

    if (FT_Init_FreeType(&ft)) {
        SDL_Log("Failed to init FreeType");
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, font_path, 0, &face)) {
        SDL_Log("Failed to load font");
        FT_Done_FreeType(ft);
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, font_size);

    // Example: set variable weight axis if available
    if (FT_HAS_MULTIPLE_MASTERS(face)) {
        FT_MM_Var *mmvar;
        if (FT_Get_MM_Var(face, &mmvar) == 0) {
            FT_Fixed coords[mmvar->num_axis];
            for (int i = 0; i < mmvar->num_axis; i++)
                coords[i] = mmvar->axis[i].def;
            // Set weight axis to bold-ish (700)
            coords[0] = 400 * 65536;
            FT_Set_Var_Design_Coordinates(face, mmvar->num_axis, coords);
        }
    }

    struct character_info c[128] = {0};

    // Compute atlas size
    int atlas_width = 0;
    int atlas_height = 0;
    for (int i = 0; i < 128; i++) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) continue;
        c[i].ax = face->glyph->advance.x / 64.0f;
        c[i].ay = 0;
        c[i].bw = face->glyph->bitmap.width;
        c[i].bh = face->glyph->bitmap.rows;
        c[i].bl = face->glyph->bitmap_left;
        c[i].bt = face->glyph->bitmap_top;
        atlas_width += face->glyph->bitmap.width;
        if (face->glyph->bitmap.rows > atlas_height)
            atlas_height = face->glyph->bitmap.rows;
    }

    if (atlas_width == 0) {
        SDL_Log("No glyphs to render");
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        return;
    }

    // Create atlas texture
    SDL_Texture *atlas = SDL_CreateTexture(renderer,
                                           SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_TARGET,
                                           atlas_width, atlas_height);
    SDL_SetRenderTarget(renderer, atlas);
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderClear(renderer);

    int x_offset = 0;
    for (int i = 0; i < 128; i++) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) continue;
        FT_Bitmap *bmp = &face->glyph->bitmap;
        if (bmp->width == 0 || bmp->rows == 0) continue;

        SDL_Surface *surf = SDL_CreateSurface(bmp->width, bmp->rows, SDL_PIXELFORMAT_RGBA8888);
        Uint32 *pixels = (Uint32*)surf->pixels;
        for (int row = 0; row < bmp->rows; row++)
            for (int col = 0; col < bmp->width; col++) {
                Uint8 val = bmp->buffer[row * bmp->pitch + col];
                pixels[row * bmp->width + col] = (val << 24) | 0xFFFFFF; // white with alpha
            }

        SDL_Texture *glyph_tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_DestroySurface(surf);

        SDL_FRect dst = {
            (float)x_offset,
            (float)(atlas_height - bmp->rows),
            (float)bmp->width,
            (float)bmp->rows
        };
        SDL_RenderTexture(renderer, glyph_tex, NULL, &dst);
        SDL_DestroyTexture(glyph_tex);

        c[i].tx = (float)x_offset;
        x_offset += bmp->width;
    }

    SDL_SetRenderTarget(renderer, NULL);

    // Draw text
    float pen_x = 10, pen_y = 200; // arbitrary start
    for (const char *p = text; *p; p++) {
        unsigned char ch = *p;
        if (ch >= 128) continue;

        float w = c[ch].bw;
        float h = c[ch].bh;
        float x2 = pen_x + c[ch].bl;
        float y2 = pen_y - c[ch].bt;

        if (w > 0 && h > 0) {
            SDL_FRect src = { c[ch].tx, (float)(atlas_height - c[ch].bh), c[ch].bw, c[ch].bh };
            SDL_FRect dst = { x2, y2, w, h };
            SDL_RenderTexture(renderer, atlas, &src, &dst);
        }

        pen_x += c[ch].ax;
        pen_y += c[ch].ay;
    }

    SDL_DestroyTexture(atlas);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void destroy()
{
}

// Required
BOOST_DLL_ALIAS(init, _init)
BOOST_DLL_ALIAS(draw, _draw)
BOOST_DLL_ALIAS(destroy, _destroy)
