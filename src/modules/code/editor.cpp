#include <raylib.h>

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
    active = true;
}


void Editor::Start()
{

    GenTextureMipmaps(&font.texture);
    fontSize = (float)font.baseSize;

    field = new Field("");


    renderer = new FieldRenderer(*field);
}


void Editor::Render(int pos_x, int pos_y, size_t width, size_t height, float deltatime)
{
    DrawRectangle(pos_x, pos_y, width, height, colorTheme.background);
    renderer->Render(pos_x, pos_y, width, height);
}

void Editor::Input(int pos_x, int pos_y, size_t width, size_t height, float deltatime)
{
    if (field->InputLoop(pos_x, pos_y, width, height, deltatime)) //returns if need to update highlight
    {
        renderer->Update(field->cursor_y);
    }
}


void Editor::Abort()
{

}

Editor::~Editor()
{
    delete field;
    delete renderer;
}




