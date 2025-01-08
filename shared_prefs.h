#include <SFML/Graphics.hpp>
#include <string>
#ifndef SHARED_PREFS
#define SHARED_PREFS
struct ColorSet
{
    sf::Color text;
    sf::Color background;
    sf::Color acccent;
    sf::Color shadow;
    sf::Color inactive;
    std::string name;
};

#endif
