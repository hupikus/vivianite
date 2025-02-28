#include <raylib.h>

#include <cstring>
#include <algorithm>

#include <string>
#include <string_view>
#include <array>

#include "field_renderer.h"
#include "../../structure/type/raylib_type.h"
#include "editor.h"

#define RESERVE_WORD 31

#define FONT_SIZE 22.0f
#define VERTICAL_MARGIN 13.0f
#define HORIZONTAL_MARGIN 40.0f


#define LINE_COUNTER_SIZE 20.0f
#define LINE_COUNTER_SPACING 9.0f



std::array<std::string, 13> keywords = {"break", "do", "else", "elseif", "for", "if", "repeat", "then", "until", "while", "goto", "end", "return"};
auto kwbegin = keywords.begin();
auto kwend = keywords.end();

std::array<std::string, 7> logic_keywords = {"and", "false", "in", "nil", "not", "or", "true"};
auto lgbegin = logic_keywords.begin();
auto lgend = logic_keywords.end();

std::array<std::string, 8> namespaces = {"string", "table", "math", "io", "os", "debug", "coroutine", "utf8"};
auto nsbegin = namespaces.begin();
auto nsend = namespaces.end();

std::array<std::array<std::string, 25>, 9> builtins = {{

    //base
    {"assert", "collectgarbage", "dofile", "error", "_G", "getmetatable", "ipairs", "load", "loadfile", "next", "pairs", "pcall",
    "print", "rawequal", "rawget", "rawlen", "rawset", "require", "select", "setmetatable", "tonumber", "tostring", "type", "warn",
    "xpcall"},

    //string
    {"byte", "char", "dump", "find", "format", "gmatch", "gsub", "len", "lower", "match", "pack", "packsize", "rep", "reverse",
    "sub", "unpack", "upper"},

    //table
    {"concat", "insert", "move", "pack", "remove", "sort", "unpack"},

    //math
    {"abs", "acos", "asin", "atan", "ceil", "cos", "deg", "exp", "floor", "fmod", "huge", "log", "max", "min", "modf", "pi", "rad",
    "random", "randomseed", "sin", "sqrt", "tan", "tointeger", "type", "ult"},

    //io
    {"close", "flush", "input", "lines", "open", "output", "popen", "read", "stderr", "stdin", "stdout", "tmpfile", "type", "write"},

    //os
    {"clock", "date", "difftime", "execute", "exit", "getenv", "remove", "rename", "setlocale", "time", "tmpname"},

    //debug
    {"debug", "gethook", "getinfo", "getlocal", "getmetatable", "getregistry", "getupvalue", "getuservalue", "sethook", "setlocal",
    "setmetatable", "setupvalue", "setuservalue", "traceback", "upvalueid", "upvaluejoin"},

    //coroutine
    {"close", "create", "isyieldable", "resume", "running", "status", "wrap", "yield"},

    //utf8
    {"char", "charpattern", "codepoint", "codes", "len", "offset"}

}};


// FieldRenderer::~FieldRenderer()
// {
//     //Why segfault
//     UnloadFont(font);
//     UnloadFont(counterFont);
// }

void FieldRenderer::Update(size_t FromLine)
{
    delete render;
    render = new CodeState;
    if (FromLine > 0) { --FromLine; }
    GenText(0);
}

FieldRenderer::FieldRenderer(Field& active_field, const char* code_font) : field(active_field)
{
    font = LoadFontEx(code_font, FONT_SIZE, 0, 0);
    counterFont = LoadFontEx("assets/fonts/Roboto_Condensed-Black.ttf", LINE_COUNTER_SIZE, 0, 0);

    SetTextLineSpacing(FONT_SIZE);
    fontSize = MeasureTextEx(font, " ", FONT_SIZE, FONT_SIZE);

    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(counterFont.texture, TEXTURE_FILTER_BILINEAR);


    colors = &Editor::colorTheme;
    highlight.reserve(180);

    render = new CodeState; //look header

    palette = new CodePalette{
        .text       = colors->text,
        .comment    = Color{155, 155, 155, 255},
        .variable   = Color{138, 219, 251, 255},
        .function   = Color{249, 216, 109, 255},
        .keyword    = Color{223, 92, 189, 255},
        .logic      = Color{230, 83, 68, 255},
        .branch     = Color{205, 161, 241, 255},
        .definition = Color{129, 234, 181, 255},
        .namspace   = Color{139, 238, 88, 255},
        .string     = Color{230, 139, 49, 255},
        .number     = Color{183, 226, 130, 255},
        .brackets   = {
                        Color{7, 230, 142, 255},
                        Color{52, 239, 255, 255},
                        Color{255, 235, 60, 255},
                        Color{212, 93, 255, 255},
                        Color{63, 246, 80, 255},
                    },
        .commas     = Color{0, 187, 184, 255},
        .lines      = Color{226, 164, 41, 255},
        .escape     = Color{108, 137, 255, 255},
        .error      = Color{247, 75, 51, 255}
    };
    GenText(0);
}

void FieldRenderer::Render(int pos_x, int pos_y, size_t width, size_t height)
{
    //code
    size_t x = 0;
    size_t y = 0;
    size_t index = 0;
    float check_y = 0.0f;


    size_t wrap_offset = 0;
    size_t wrap_howmany_fit = (width - HORIZONTAL_MARGIN) / fontSize.x - 2;

    for (const std::string_view& line : field.Text)
    {
        //Code
        x = 0;
        index = 0;

        check_y = VERTICAL_MARGIN + FONT_SIZE * (y + wrap_offset) + field.scrollpos;
        if (check_y > height) { break; }
        check_y += pos_y;


        //line counter
        for (auto i : lines[y])
        {
            DrawTextCodepoint(counterFont, i, Vector2{pos_x + 5.0f + LINE_COUNTER_SPACING * x, check_y + ((FONT_SIZE - LINE_COUNTER_SIZE) / 2)}, LINE_COUNTER_SIZE, palette->lines);
            ++x;
        }

        //text
        x = 0;
        for (auto codepoint = line.begin(); codepoint != line.end(); ++codepoint)
        {
            DrawTextCodepoint(font, *codepoint, Vector2{pos_x + HORIZONTAL_MARGIN + fontSize.x * x, check_y}, FONT_SIZE, highlight[y][index]);


            //cursor
            if ( y == field.cursor_y and index == field.cursor_x) { DrawRectangle(HORIZONTAL_MARGIN + fontSize.x * x, check_y, 2, FONT_SIZE, WHITE); }


            ++x;
            ++index;

            //wrap
            if (x > wrap_howmany_fit) {
                check_y = VERTICAL_MARGIN + FONT_SIZE * (y + ++wrap_offset) + field.scrollpos;
                x = 0;
            }
        }
        if ( y == field.cursor_y and index == field.cursor_x) { DrawRectangle(HORIZONTAL_MARGIN + fontSize.x * x, check_y, 2, FONT_SIZE, WHITE); }

        ++y;
    }

    //cursor and selection
    // x = field.cursor_x;
    // y = field.cursor_y;
    // while (x > wrap_howmany_fit)
    // {
    //     ++y;
    //     x -= wrap_howmany_fit + 1;
    // }
    // DrawRectangle(HORIZONTAL_MARGIN + fontSize.x * x, VERTICAL_MARGIN + y * FONT_SIZE, 2, FONT_SIZE, WHITE);

}

//total keyword, namespace, variable searcher
const Color* KeywordCheck(const std::string* buffer, const CodePalette* palette, CodeState* state)
{
    //keywords
    if (*buffer == "local")
    {
        state->awaits_name = 1;
        return &(palette->keyword);
    }
    if (*buffer == "function")
    {
        state->awaits_name = 2;
        return &(palette->keyword);
    }
    if (*buffer == "class")
    {
        state->awaits_name = 3;
        return &(palette->keyword);
    }

    //branch keywords
    if (std::find(kwbegin, kwend, *buffer) != kwend) { return &(palette->branch); }

    //logic keywords
    if (std::find(lgbegin, lgend, *buffer) != lgend) { return &(palette->logic); }


    //variable, function names
    if (state->awaits_name > 0)
    {
        switch (state->awaits_name)
        {
            case 1: //variable
                state->vars.push_back(*buffer);
                break;
            case 2: //function
                state->functions.push_back(*buffer);
                break;
        }
        state->awaits_name = 0;
        return &(palette->definition);
    }

    //namespace mentions
    auto find = std::find(nsbegin, nsend, *buffer);
    if (find != nsend)
    {
        state->awaits_namespace_member = (find - nsbegin) + 1;
        return &(palette->namspace);
    }


    //namespace builtins
    auto nmend = builtins[state->awaits_namespace_member].end();
    if (std::find(builtins[state->awaits_namespace_member].begin(), nmend, *buffer) != nmend)
    {
        return &(palette->function);
    }
    else if (state->awaits_namespace_member != 0)
    {
        return &(palette->text);
    }

    //use of defined variables
    auto vend = state->vars.end();
    if (std::find(state->vars.begin(), vend, *buffer) != vend) { return &(palette->variable); }

    //use of defined functions
    vend = state->functions.end();
    if (std::find(state->functions.begin(), vend, *buffer) != vend) { return &(palette->function); }

    //unknown, but check for global var definition in future
    return &(palette->text);

}

//Generate text highlight and analyse code through quickly
void FieldRenderer::GenText(size_t start_line)
{
    size_t size = field.Text.size();
    const Color* current_color;

    if (start_line == 0) { highlight.clear(); }
    highlight.reserve(size);
    lines.reserve(size);

    bool check_keyword = false;

    std::string buffer = "";
    buffer.reserve(RESERVE_WORD);


    Text* text;
    bool check_mode;

    size_t& x = render->x;
    size_t& y = render->y;

    y = 0;

    //text props

    for (const std::string_view& line : field.Text)
    //for (const std::string& line : field.Text)
    {
        check_mode = start_line > y + 1;
        // !check_mode == highlight edit mode

        if (!check_mode)
        {
            //add line
            highlight.push_back(std::vector<Color>());
            highlight[y].reserve(line.end() - line.begin());

            lines.resize(y + 1);
            lines[y] = std::to_string(y + 1);
        }

        //disable comment, string mode, var mode
        if (!render->multiline_comment)
        {
            render->comment = false;
            render->string = false;
        }
        current_color = &(palette->text);
        render->awaits_name = 0;


        x = 0;
        buffer.clear();
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
                     UpdateKeywordCheck(&buffer, x, y, check_mode);
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
                    UpdateKeywordCheck(&buffer, x, y, check_mode);
                    }
                }
                else if (render->squote) render->string = false;

                break;
            case '(':
            case '[':
                if (!render->comment and !render->string)
                {
                    ++render->brackets;
                    current_color = &(palette->brackets[render->brackets % 5]);
                }
                break;
            case ')':
            case ']':
                if (!render->comment and !render->string)
                {
                    current_color = &(palette->brackets[render->brackets % 5]);
                    if (--render->brackets == -1) { current_color = new Color{255, 0, 0, 255}; }
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
            case '~':
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
                if (!render->comment and !render->string)
                {
                    if (buffer.empty())
                    {
                        current_color = &(palette->number);
                        if (*(it + 1) == '.' and !check_mode)
                        {
                            highlight[y].push_back(palette->number);
                            highlight[y].push_back(palette->number);
                            ++it;
                            x += 2;
                            continue;
                        }

                        if (render->awaits_namespace_member > 0 and *it != '.')
                        {
                            render->awaits_namespace_member = 0;
                        }
                    }
                    else
                    {
                        buffer += *it;
                        current_color = &(palette->text);
                        check_keyword = false;
                    }
                }
                break;
            case '\\':
                if (render->string and it + 1 < line.end() and !check_mode)
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
                    case 'z':
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
                if (!render->comment and !render->string)
                {
                    current_color = &(palette->text);
                    check_keyword = false;
                    buffer += *it;
                }
                break;
            }

            if (!check_mode)
            {
                //apply
                highlight[y].push_back(*current_color);
            }

            //keywords
            if (check_keyword and !render->comment)
            {
                UpdateKeywordCheck(&buffer, x, y, check_mode);
                if (*it != ' ')
                {
                    render->awaits_name = 0;
                }
            }
            ++x;
        }
        UpdateKeywordCheck(&buffer, x, y, check_mode);
        ++y;
    }
}


void FieldRenderer::UpdateKeywordCheck(std::string* buffer, size_t x, size_t y, bool check_mode)
{
    const Color* kw_type = KeywordCheck(buffer, palette, render);

    size_t blen = buffer->length();

    if (kw_type != &(palette->text))
    {

        if (x >= blen and !check_mode)
        {
            for (size_t dx = x - blen; dx != x; ++dx)
            {
                highlight[y][dx] = *kw_type;
            }
        }
    }
    else if (x == blen and !check_mode)
    {
        for (size_t dx = x - blen; dx != x; ++dx)
        {
            highlight[y][dx] = *kw_type;
        }
    }
    buffer->clear();
}
