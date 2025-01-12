#include "raylib.hpp"
#include <raylib.h>
#include <vector>

#ifndef FIELD_RENDERER
#define FIELD_RENDERER

#include "shared_prefs.h"
#include "field.h"



class FieldRenderer
{
private:
    Field& field;
    Font& font;

    float scale = 100.0f;
    int posx, posy;
    size_t width, height;

    ColorSet* colors;
    Vector2 fontSize;

    //cached
    //std::vector<sf::Text> Texts;
    //std::vector<sf::Text> LineNumbers;
    size_t actual_size = 0;

    void GenText(size_t start_line);
    //sf::Text* GetText(size_t& index);

public:
    FieldRenderer(Field& field, Font& code_font, iRect* surface);


    void Render();
};

#endif
