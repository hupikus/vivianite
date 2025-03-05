#include "resources.h"

Font baseFont;

void LoadResources()
{
    baseFont = LoadFontEx("assets/fonts/Menlo-Bold.ttf", 96, nullptr, 65535);
    SetTextureFilter(baseFont.texture, TEXTURE_FILTER_BILINEAR);
}
