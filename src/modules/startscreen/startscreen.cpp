#include "startscreen.h"


StartingScreen::StartingScreen()
{

}

void StartingScreen::Start()
{

}

void StartingScreen::Render(int pos_x, int pos_y, size_t width, size_t height, float deltatime)
{
    DrawRectangle(pos_x, pos_y, width, height, colorTheme.background);
}

void StartingScreen::Input(int pos_x, int pos_y, size_t width, size_t height, float deltatime)
{
    //InputLoop(pos_x, pos_y, width, height, deltatime);
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
