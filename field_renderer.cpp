#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "field_renderer.h"
#include "editor.h"


FieldRenderer::FieldRenderer(Field& active_field, sf::RenderTexture& texture, int field_width, int field_height) : field(active_field), surface(texture)
{
    width = field_width;
    height = field_height;
    font.loadFromFile("fonts/Menlo.ttf");
    colors = Editor::colorTheme;
}

void FieldRenderer::Render()
{
    sf::Text text(field.Text[0], font, 52);

    surface.draw(text);
    surface.clear(colors.background);
}
