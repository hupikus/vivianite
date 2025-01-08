#include <SFML/Graphics/RenderWindow.hpp>

#include "window.h"



sf::RenderWindow* Window::GetSFWin() { return &window; }

Window::Window(int window_width, int window_height)
{
    width = window_width;
    height = window_height;
}
