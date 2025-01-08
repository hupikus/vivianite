#include <string>

#ifndef EDITOR
#define EDITOR

#include "field_renderer.h"
#include "field.h"


class Editor
{
private:
    std::string path;
    sf::RenderWindow* window;
    Field* field;
    FieldRenderer* renderer;

    bool active = false;

    //prefs
    sf::Font mainFont;

    //composite rendering
    sf::View* view;
    sf::RenderTexture codeTexture;
    sf::Sprite codeSprite;

public:
    Editor(std::string workpath);
    ~Editor();

    static ColorSet colorTheme;

    void init();

    void abort();

    void process();

    void Render();

};

#endif
