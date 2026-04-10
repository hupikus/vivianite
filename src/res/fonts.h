#ifndef FONTS_H
#define FONTS_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_MULTIPLE_MASTERS_H

#define DEFAULT_FONT_PATH "assets/fonts/Menlo.ttf"

bool InitFonts(void);
void DestroyFonts(void);

#endif // FONTS_H
