#include "raylib.hpp"
#include <string>

#include "editor.h"



ColorSet Editor::colorTheme
{
    Color{207, 255, 231}, // text
    Color{43, 35, 53}, // background
    Color{46, 105, 72}, // acccent
    Color{42, 28, 55}, // shadow
    Color{91, 97, 105}, // inactive
};

Editor::Editor(size_t f_width, size_t f_height)
{
    width = f_width;
    height = f_height;
    font = LoadFont("fonts/Menlo.ttf");

    //areas, customizable
    codeArea = {0, 0, width, height};
}

void Editor::Init()
{
    field = new Field("");
    renderer = new FieldRenderer(*field, font, &codeArea);
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

    BeginScissorMode(0, 0, width, height);
    renderer->Render();
    EndScissorMode();

    DrawFPS(0, 0);

}



