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
    active = true;
}


void Editor::Start()
{
    font = LoadFontEx("assets/fonts/Menlo-Bold.ttf", 96, 0, 0);

    GenTextureMipmaps(&font.texture);
    fontSize = (float)font.baseSize;

    field = new Field("");


    renderer = new FieldRenderer(*field, "assets/fonts/Menlo-Bold.ttf");
}


void Editor::Render(size_t width, size_t height, float deltatime)
{
    if (field->InputLoop(deltatime)) //returns if need to update highlight
    {
        renderer->Update(field->cursor_y);
    }
    ClearBackground(colorTheme.background);
    BeginScissorMode(0, 0, width, height);
    renderer->Render(0, 0, width, height);
    EndScissorMode();


}

void Editor::Process(float deltatime)
{
}


void Editor::Abort()
{

}

Editor::~Editor()
{
    delete field;
    delete renderer;
}




