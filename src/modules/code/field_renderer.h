#include <raylib.h>
#include <utility>
#include <vector>

#include "../../globals/colors.h"
#include "../../globals/resources.h"

#include "../../structure/type/shared_prefs.h"
#include "../../structure/type/raylib_type.h"
#include "field.h"

#ifndef FIELD_RENDERER
#define FIELD_RENDERER

class FieldRenderer
{
private:
    Field& field;

    Font& font = baseFont;
    Font counterFont;

    float scale = 100.0f;

    //customization
    //ColorSet* colors;
    ColorSet& colors = vivianiteColorTheme;
    struct CodePalette* palette;
    Vector2 fontSize;

    //checkState
    struct CodeState* render;

    //cache
    std::vector<std::string> lines;
    std::vector<std::vector<Color>> highlight;

    void GenText(size_t start_line);
    void UpdateKeywordCheck(std::string* buffer, size_t x, size_t y, bool check_mode);
    void DrawLine();

public:
    FieldRenderer(Field& field);


    void Render(int pos_x, int pos_y, size_t width, size_t height);
    void Update(size_t FromLine);
};

//Structure representing code state at given cursor position
struct CodeState
{
    size_t x = 0;
    size_t y = 0;
    bool comment = false;
    bool string = false;
    bool squote = false;
    bool multiline_comment = false;

    //brackets () [] {}
    int brackets = 0;

    //definitions
    size_t awaits_name = 0;
    //analyser
    std::vector<std::string> vars = std::vector<std::string>(24); //1
    std::vector<std::string> functions = std::vector<std::string>(24); //2
    std::vector<std::string> classes = std::vector<std::string>(24); //3

    //functions
    int awaits_namespace_member = 0;
};

struct CodePalette
{
    Color text;
    Color comment;
    Color variable;
    Color function;
    Color keyword;
    Color logic;
    Color branch;
    Color definition;
    Color namspace;
    Color string;
    Color number;
    Color brackets[5];
    Color commas;
    Color lines;
    Color escape;
    Color error;
};


#endif














