#include "vivianite.h"
#include "fonts.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_MULTIPLE_MASTERS_H

FT_Library ft;

#define DEFAULT_FONT_PATH "assets/fonts/Menlo.ttf"
FT_Face default_font;


bool InitFonts()
{
    FT_Init_FreeType(&ft);

    // Default font
    if (FT_New_Face(ft, DEFAULT_FONT_PATH, 0, &default_font)) {
        printf("Could not find the default font.\n");
        return false;
    }

    return true;
}

FT_Face *GetFont(std::string name)
{
    return &default_font;
}

FT_Face *GetDefaultFont()
{
    return &default_font;
}

void DestroyFonts()
{
    FT_Done_FreeType(ft);
}
