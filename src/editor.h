#include <string>

#include "elements/abstract/tab.h"
#include "shared_prefs.h"

#include "field_renderer.h"
#include "field.h"

#ifndef EDITOR
#define EDITOR

class Editor : public Tab
{
public:
    Editor();
    ~Editor() override;

    static ColorSet colorTheme;

    void Start() override;

    void Render(size_t width, size_t height, float deltatime) override;
    void Process(float deltatime) override;

    void Abort() override;

private:
    Field* field;
    FieldRenderer* renderer;

    bool active = false;

    //prefs
    Font font;
    std::string fontName;
    float fontSize;
};

#endif
