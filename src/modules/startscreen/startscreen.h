#include "raylib.h"

#include "../../structure/type/shared_prefs.h"
#include "../../elements/abstract/tab.h"
#include "../../globals/resources.h"
#include "../../globals/colors.h"


#ifndef STARTSCREEN
#define STARTSCREEN

class StartingScreen : public Tab
{
public:
    StartingScreen();
    ~StartingScreen() override;

    void Start() override;

    void Render(int pos_x, int pos_y, size_t width, size_t height, float deltatime) override;
    void Input(int pos_x, int pos_y, size_t width, size_t height, float deltatime) override;

    void Abort() override;

    std::unique_ptr<Tab> NewInstance() override;

private:
    Font& font = baseFont;
    ColorSet& colorTheme = vivianiteColorTheme;
};

#endif
