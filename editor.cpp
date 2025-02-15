#include <raylib.h>
#include <string>

#include "editor.h"

#define DEBUG_FONT_SIZE 52

ColorSet Editor::colorTheme
{
    Color{207, 255, 231, 255}, // text
    Color{43,  35,  53,  255}, // background
    Color{46,  105, 72,  255}, // acccent
    Color{42,  28,  55,  255}, // shadow
    Color{91,  97,  105, 255}, // inactive
};

Editor::Editor(size_t f_width, size_t f_height)
{
    width = f_width;
    height = f_height;

    //areas, customizable
    codeArea = {0, 0, width, height};

    fontName = "fonts/Menlo-Bold.ttf";
    font = LoadFontEx("fonts/Menlo-Bold.ttf", 96, 0, 0);
    GenTextureMipmaps(&font.texture);


    fontSize = (float)font.baseSize;
}

void Editor::Init()
{
    field = new Field("");
    renderer = new FieldRenderer(*field, "fonts/Menlo-Bold.ttf", &codeArea);

}

void Editor::Abort()
{
    active = false;
}

Editor::~Editor()
{
    delete field;
    delete renderer;
}



void Editor::Render()
{
    ClearBackground(colorTheme.background);
    //DrawTextEx(font, "Nu suka", (Vector2){200, 50}, 96, 22.0f, WHITE);
    BeginScissorMode(0, 0, width, height);
    renderer->Render();
    EndScissorMode();

}



