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

Editor::Editor()
{
    fontName = "assets/fonts/Menlo-Bold.ttf";
    font = LoadFontEx("assets/fonts/Menlo-Bold.ttf", 96, 0, 0);
    GenTextureMipmaps(&font.texture);
    fontSize = (float)font.baseSize;
}

void Editor::Init()
{
    field = new Field("");
    renderer = new FieldRenderer(*field, "assets/fonts/Menlo-Bold.ttf");

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



void Editor::Render(size_t width, size_t height)
{
    //TODO: separate threads
    if (field->InputLoop())
    {
        renderer->Update(field->cursor_y);
    }
    ClearBackground(colorTheme.background);
    BeginScissorMode(0, 0, width, height);
    renderer->Render(0, 0, width, height);
    EndScissorMode();


}



