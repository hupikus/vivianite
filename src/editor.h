#include <string>

#ifndef EDITOR
#define EDITOR

#include "shared_prefs.h"
#include "field_renderer.h"
#include "field.h"


class Editor
{
private:
    Field* field;
    FieldRenderer* renderer;

    bool active = false;

    //prefs
    Font font;
    std::string fontName;
    float fontSize;

public:
    Editor();
    ~Editor();

    static ColorSet colorTheme;

    void Init();

    void Abort();


    void Render(size_t width, size_t height);

};

#endif
