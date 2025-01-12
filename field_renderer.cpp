#include "raylib.hpp"
#include <raylib.h>
#include <string_view>
#include <iostream>

#include "field_renderer.h"
#include "editor.h"

#define RESERVE_MORE 80
#define RESERVE_WIDTH 120

#define DEBUG_FONT_SIZE 22.0f
#define VERTICAL_MARGIN 13.0f
#define HORIZONTAL_MARGIN 13.0f
#define LINE_COUNTER_SIZE 10.0f

#define TAB_SIZE 4


FieldRenderer::FieldRenderer(Field& active_field, Font& code_font, iRect* surface) : field(active_field), font(code_font)
{
    posx = surface->pos_x;
    posy = surface->pos_y;
    width = surface->width;
    height = surface->height;

    SetTextLineSpacing(DEBUG_FONT_SIZE);

    fontSize = MeasureTextEx(font, " ", DEBUG_FONT_SIZE, DEBUG_FONT_SIZE);


    colors = &Editor::colorTheme;
    //Texts.reserve(field.Text.size() + RESERVE_MORE);

    //LineNumbers.reserve(field.Text.size() + RESERVE_MORE);
    //GenText(0);

}

void FieldRenderer::Render()
{
    for (size_t i = 0; i != actual_size; ++i)
    {
        //surface.draw(LineNumbers[i]);
        //surface.draw(Texts[i]);
    }
}
//
//void FieldRenderer::GenText(size_t start_line)
//{
//    actual_size = Texts.size();
//    size_t size = 0;
//    Color* current_color = &colors.text;
//    std::string buffer = "";
//    buffer.reserve(RESERVE_WIDTH);
//    sf::Text* text;
//
//    size_t x;
//    size_t y = start_line;
//
//    //text props
//
//    bool comment = false;
//    //for (const std::string_view& line : field.Text)
//    for (const std::string& line : field.Text)
//    {
//        x = 0;
//        text = GetText(size);
//        text->setPosition(HORIZONTAL_MARGIN, VERTICAL_MARGIN + DEBUG_FONT_SIZE * y);
//
//        text->setString(line);
//        ++y;
//        continue;
//        for (auto it = line.begin(); it != line.end(); ++it)
//        {
//                switch (*it)
//                {
//                case ' ':
//                    ++x;
//                    if (comment) { buffer += ' '; break; }
//                    std::cout << buffer << ' ';
//                    text->setString(std::move(buffer));
//                    //buffer.clear();
//                    text = GetText(size);
//                    text->setPosition(HORIZONTAL_MARGIN + spacing * x, VERTICAL_MARGIN + DEBUG_FONT_SIZE * y);
//                    break;
//                case '\t':
//                    buffer += std::string(TAB_SIZE, ' ');
//                    x += TAB_SIZE;
//                default:
//                    buffer += *it;
//                    ++x;
//                    break;
//                }
//        }
//
//        if (!buffer.empty())
//        {
//            text->setString(std::move(buffer));
//            buffer.clear();
//        }
//        std::cout << '\n';
//        y += 1;
//    }
//    if (!buffer.empty())
//    {
//        text->setString(std::move(buffer));
//        buffer.clear();
//    }
//}
//
//
//sf::Text* FieldRenderer::GetText(size_t& index)
//{
//    if (index >= actual_size)
//    {
//        Texts.push_back(std::move(sf::Text("", font, DEBUG_FONT_SIZE)));
//        Texts[index].setFillColor(colors.text);
//        ++actual_size;
//    }
//    return &Texts[index++];
//}