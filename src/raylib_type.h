#include <raylib.h>

#ifndef RAYLIB_TYPE
#define RAYLIB_TYPE

typedef struct Text
{
    Font font;
    const char* text;
    Vector2 position;
    float fontSize;
    float spacing;
    Color tint;

    float rotation = 0.0f;
    void draw() { DrawTextEx(font, text, position, fontSize, spacing, tint); }
    //void drawRotated() { DrawTextPro(font, text, position, origin, rotation, fontSize, spacing, tint); }
} Text;

Text* NewText(Font font, const char* text, Vector2 position, float fontSize, float spacing, Color tint);

#endif
