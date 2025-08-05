#pragma once

#include <string>
#include <memory>

#include "../../globals/colors.h"
#include "../../globals/resources.h"
#include "../../elements/abstract/tab.h"
#include "../../structure/type/shared_prefs.h"

#include "field_renderer.h"
#include "field.h"

#ifndef EDITOR
    #define EDITOR

class Editor: public Tab {
public:
    Editor();
    ~Editor() override;

    void Start() override;

    void Render(int pos_x, int pos_y, size_t width, size_t height, float deltatime) override;
    void Input(int pos_x, int pos_y, size_t width, size_t height, float deltatime) override;

    void Abort() override;

    std::unique_ptr<Tab> NewInstance() override;

private:
    Field* field;
    FieldRenderer* renderer;

    bool active = false;

    // prefs
    Font& font = baseFont;
    ColorSet& colorTheme = vivianiteColorTheme;
    std::string fontName;
    float fontSize;
};

#endif
