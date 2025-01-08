#include <SFML/Graphics/RenderWindow.hpp>
class Window
{
private:
    int width, height;
    sf::RenderWindow window;
public:
    Window(int window_width, int window_height);

    sf::RenderWindow* GetSFWin();

};
