#include "vivianite.h"
#include "fonts.h"

FT_Library ft;
FT_Face *default_font;
FT_Face *render_font;

bool InitFonts()
{
    FT_Init_FreeType(&ft);

    // Default font
    if (FT_New_Face(ft, DEFAULT_FONT_PATH, 0, default_font)) {
        printf("Could not load the default font.\n");
        return false;
    }

    render_font = default_font;

    return true;
}

FT_Face *GetFont(std::string name) { return render_font; } // Yet

FT_Face *GetDefaultFont(void) { return default_font; }

void SetRenderFont(FT_Face *font)
{
    if (!font) return;
    render_font = font;
}

void DestroyFonts(void)
{
    FT_Done_FreeType(ft);
}
