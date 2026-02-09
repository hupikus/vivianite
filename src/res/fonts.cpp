#include "fonts.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_MULTIPLE_MASTERS_H

FT_Library ft;

bool InitFonts()
{
    FT_Init_FreeType(&ft);
    return true;
}
