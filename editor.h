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

    size_t width, height;

    bool active = false;

    //prefs
    Font font;


    //composite rendering
    iRect codeArea;


public:
    Editor(size_t f_width, size_t f_height);
    ~Editor();

    static ColorSet colorTheme;

    void Init();

    void Abort();


    void Render();

};

#endif
