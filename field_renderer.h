#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#ifndef FIELD_RENDERER
#define FIELD_RENDERER

#include "field.h"
#include "shared_prefs.h"



class FieldRenderer
{
private:
    Field& field;
    sf::RenderTexture& surface;

    float scale = 100.0f;
    int width, height;

    ColorSet colors;
    sf::Font font;



public:
    FieldRenderer(Field& field, sf::RenderTexture& surface, int field_width, int field_height);

    void Render();
};

#endif
