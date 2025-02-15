#ifndef FIELD_RENDERER
#define FIELD_RENDERER

#include <raylib.h>
#include <utility>
#include <vector>

#include "shared_prefs.h"
#include "raylib_type.h"
#include "field.h"



class FieldRenderer
{
private:
    Field& field;

    Font font;
    Font counterFont;

    float scale = 100.0f;
    int posx, posy;
    size_t width, height;

    //customization
    ColorSet* colors;
    struct CodePalette* palette;
    Vector2 fontSize;

    //checkState
    struct CodeState* render;

    //cache
    std::vector<std::string> lines;
    std::vector<std::vector<Color>> highlight;

    void GenText(size_t start_line);
    void UpdateKeywordCheck(std::string* buffer, size_t x, size_t y);
    void DrawLine();

public:
    FieldRenderer(Field& field, const char* code_font, iRect* surface);
    ~FieldRenderer();


    void Render();
};

struct CodeState
{
    size_t x = 0;
    size_t y = 0;
    bool comment = false;
    bool string = false;
    bool squote = false;
    bool multiline_comment = false;

    //brackets
    int brackets[4] = {0, 0, 0, 0};
};

struct CodePalette
{
    Color text;
    Color comment;
    Color variable;
    Color function;
    Color keyword;
    Color string;
    Color number;
    Color brackets[3];
    Color commas;
    Color lines;
    Color escape;
};


#endif














