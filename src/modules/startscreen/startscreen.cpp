#include "startscreen.h"
#include <raylib.h>

StartingScreen::StartingScreen()
{
}

void StartingScreen::Start()
{
}

void StartingScreen::Render(int pos_x, int pos_y, size_t width, size_t height, float deltatime)
{
    DrawRectangle(pos_x, pos_y, width, height, colorTheme.background);
    DrawTextEx(font, "Vivanite", {pos_x + 15.0f, pos_y + 0.0f}, 96, 0, WHITE);
    DrawRectangle(width * 0.375f, height * 0.375f, width >> 2, width >> 2, colorTheme.inactive);
}

void StartingScreen::Input(int pos_x, int pos_y, size_t width, size_t height, float deltatime)
{
    // InputLoop(pos_x, pos_y, width, height, deltatime);
}

void StartingScreen::Abort()
{
}

StartingScreen::~StartingScreen()
{
}

std::unique_ptr<Tab> StartingScreen::NewInstance()
{
    return std::make_unique<StartingScreen>();
}
