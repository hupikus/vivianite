#include <raylib.h>

#include <cstring>
#include <algorithm>

#include <string>
#include <string_view>
#include <array>

#include "field_renderer.h"
#include "raylib_type.h"
#include "editor.h"

#define RESERVE_WORD 31

#define FONT_SIZE 22.0f
#define VERTICAL_MARGIN 13.0f
#define HORIZONTAL_MARGIN 40.0f


#define LINE_COUNTER_SIZE 20.0f
#define LINE_COUNTER_SPACING 9.0f

#define TAB_SIZE 4



std::array<std::string, 21> keywords = {"and", "break", "do", "else", "elseif", "end", "false", "for", "function", "if", "in", "local", "nil", "not", "or", "repeat", "return", "then", "true", "until", "while"};
auto kwbegin = keywords.begin();
auto kwend = keywords.end();


FieldRenderer::~FieldRenderer()
{
    UnloadFont(counterFont);
}

FieldRenderer::FieldRenderer(Field& active_field, const char* code_font, iRect* surface) : field(active_field)
{
    posx = surface->pos_x;
    posy = surface->pos_y;
    width = surface->width;
    height = surface->height;


    font = LoadFontEx(code_font, FONT_SIZE, 0, 0);
    counterFont = LoadFontEx("fonts/Roboto_Condensed-Black.ttf", LINE_COUNTER_SIZE, 0, 0);

    SetTextLineSpacing(FONT_SIZE);
    fontSize = MeasureTextEx(font, " ", FONT_SIZE, FONT_SIZE);

    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(counterFont.texture, TEXTURE_FILTER_BILINEAR);


    colors = &Editor::colorTheme;
    highlight.reserve(180);

    render = new CodeState;

    palette = new CodePalette{
        .text     = colors->text,
        .comment  = Color{155, 155, 155, 255},
        .variable = Color{14, 181, 248, 255},
        .function = Color{243, 92, 65, 255},
        .keyword  = Color{223, 92, 189, 255},
        .string   = Color{230, 139, 49, 255},
        .number   = Color{233, 246, 76, 255},
        .brackets = {
                        Color{191, 2, 91, 255},
                        Color{78, 190, 24, 255},
                        Color{15, 101, 162, 255}
                    },
        .commas   = Color{0, 187, 184, 255},
        .lines    = Color{226, 164, 41, 255},
        .escape = Color{108, 137, 255, 255}
    };
    GenText(0);
}

void FieldRenderer::Render()
{
    //code
    size_t x = 0;
    size_t y = 0;
    for (const std::string_view& line : field.Text)
    {
        x = 0;
        for (auto codepoint = line.begin(); codepoint != line.end(); ++codepoint)
        {
            DrawTextCodepoint(font, *codepoint, Vector2{HORIZONTAL_MARGIN + fontSize.x * x, VERTICAL_MARGIN + FONT_SIZE * y}, FONT_SIZE, highlight[y][x]);
            ++x;
        }
        //Line counter
        x = 0;
        for (auto i : lines[y])
        {
            DrawTextCodepoint(counterFont, i, Vector2{5.0f + LINE_COUNTER_SPACING * x, VERTICAL_MARGIN + FONT_SIZE * y + ((FONT_SIZE - LINE_COUNTER_SIZE) / 2)}, LINE_COUNTER_SIZE, palette->lines);
            ++x;
        }
        ++y;
    }
    //cursor and selection

}

const Color* KeywordCheck(const std::string* buffer, const CodePalette* palette)
{
    if (std::find(kwbegin, kwend, *buffer) != kwend) { return &(palette->keyword); }

    return &(palette->text);

}

//Generate text highlight
void FieldRenderer::GenText(size_t start_line)
{
    size_t size = sizeof field.Text;
    const Color* current_color;

    highlight.clear();
    highlight.reserve(size);
    lines.reserve(size);

    bool check_keyword = false;

    std::string buffer = "";
    buffer.reserve(RESERVE_WORD);


    Text* text;

    size_t& x = render->x;
    size_t& y = render->y;

    y = start_line;

    //text props

    for (const std::string_view& line : field.Text)
    //for (const std::string& line : field.Text)
    {
        //add line
        highlight.push_back(std::vector<Color>());
        highlight[y].reserve(line.end() - line.begin());


        lines.resize(y + 1);
        lines[y] = std::to_string(y + 1);
        //for (int i = 0; i < 16; ++i) highlight[y].push_back(Color{191, 2, 91, 255});


        //disable comment mode
        if (!render->multiline_comment) render->comment = false;
        current_color = &(palette->text);


        x = 0;
        buffer.clear();
        //continue;
        for (auto it = line.begin(); it != line.end(); ++it)
        {
            check_keyword = true;
            if (render->string) { current_color = &(palette->string); }


            switch (*it)
            {
            //whitespaces
            case '\t':
            case ' ':
                if (render->comment) break;

                if (!render->comment and !render->string) { current_color = &(palette->text); }

                break;
            //comments, strings, numbers
            case '-':
                if (!render->string)
                {
                    if (*(it + 1) == '-')
                    {
                        render->comment = true;
                        current_color = &(palette->comment);
                    }
                    else if (!render->comment)
                    {
                        current_color = &(palette->commas);
                    }
                }
                break;
            case '"':
                if (!render->string)
                {
                    if (!render->comment)
                    {
                    render->string = true;
                    render->squote = false;
                    current_color = &(palette->string);

                    check_keyword = false;
                    UpdateKeywordCheck(&buffer, x, y);
                    }
                }
                else if (!render->squote) render->string = false;

                break;
            case '\'':
                if (!render->string)
                {
                    if (!render->comment)
                    {
                    render->string = true;
                    render->squote = true;
                    current_color = &(palette->string);

                    check_keyword = false;
                    UpdateKeywordCheck(&buffer, x, y);
                    }
                }
                else if (render->squote) render->string = false;

                break;
            case '(':
                if (!render->comment and !render->string)
                {
                    render->brackets[0] = (render->brackets[0] - 1) % 3;
                    current_color = &(palette->brackets[render->brackets[0]]);
                }
                break;
            case ')':
                if (!render->comment and !render->string)
                {
                    current_color = &(palette->brackets[render->brackets[0]]);
                    render->brackets[0] = (render->brackets[0] + 1) % 3;
                }
                break;
            case ',':
            case '.':
            case ':':
            case ';':
            case '^':
            case '=':
            case '+':
            case '*':
            case '/':
            case '!':
            case '%':
            case '&':
            case '?':
            case '>':
            case '<':
            case '|':
            case '@':
            case '#':
            case '$':
                if (!render->comment and !render->string) { current_color = &(palette->commas); }
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if (!render->comment and !render->string and buffer == "") { current_color = &(palette->number); }
                if (*(it + 1) == '.')
                {
                    highlight[y].push_back(palette->number);
                    highlight[y].push_back(palette->number);
                    ++it;
                    x += 2;
                    continue;
                }
                break;
            case '\\':
                if (render->string and it + 1 < line.end())
                {
                    switch (*(it + 1))
                    {
                    case 'a':
                    case 'b':
                    case 'f':
                    case 'n':
                    case 'r':
                    case 't':
                    case 'v':
                    case '?':
                    case '\'':
                    case '"':
                    case '\\':
                    case 'x':
                        highlight[y].push_back(palette->escape);
                        highlight[y].push_back(palette->escape);
                        ++it;
                        x += 2;
                        continue;
                    default:
                        current_color = &(palette->commas);
                        break;
                    }
                }
                current_color = &(palette->commas);
                break;
            default:
                check_keyword = false;
                if (!render->comment and !render->string) { current_color = &(palette->text); }
                buffer += *it;
                break;
            }

            //apply
            highlight[y].push_back(*current_color);

            //keywords
            if (check_keyword and !render->comment)
            {
                UpdateKeywordCheck(&buffer, x, y);
            }
            ++x;
        }
        UpdateKeywordCheck(&buffer, x, y);
        ++y;
    }
}


void FieldRenderer::UpdateKeywordCheck(std::string* buffer, size_t x, size_t y)
{
    const Color* kw_type = KeywordCheck(buffer, palette);

    if (kw_type != &(palette->text))
    {
        size_t blen = buffer->length();
        if (x >= blen)
        {
            for (size_t dx = x - blen; dx != x; ++dx)
            {
                highlight[y][dx] = *kw_type;
            }
        }
    }
    buffer->clear();
}
