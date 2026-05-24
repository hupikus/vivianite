#ifndef FONT_ATLAS_H
#define FONT_ATLAS_H

#include <vector>

typedef struct GlyphMap
{
    size_t atlas;
    size_t x, y;
    size_t width, height;
} GlyphMap;

typedef struct Glyph
{
    uint32_t ch;

    size_t height;
    FT_Face *font;

    GlyphMap map;
} Glyph;

typedef struct GlyphAtlas
{
    size_t width, height;
    unsigned char *data;
} GlyphAtlas;

#endif // FONT_ATLAS_H
