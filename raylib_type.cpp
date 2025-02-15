#include <raylib.h>
#include "raylib_type.h"

Text* NewText(Font font, const char* text, Vector2 position, float fontSize, float spacing, Color tint)
{
    Text* f = new Text{font, text, position, fontSize, spacing, tint};
    return f;
}
