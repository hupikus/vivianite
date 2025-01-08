#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>



#include <SFML/Window/WindowBase.hpp>
#include <string>

#include "editor.h"

#include "shared_prefs.h"
#include "field.h"
#include "field_renderer.h"



#define WIDTH 900
#define HEIGHT 600

#define MIN_WIDTH 75
#define MIN_HEIGHT 40

ColorSet Editor::colorTheme {
    sf::Color(207, 255, 231), // text
    sf::Color(43, 35, 53), // background
    sf::Color(46, 105, 72), // acccent
    sf::Color(42, 28, 55), // shadow
    sf::Color(91, 97, 105), // inactive
    "Hello"
};


    Editor::Editor(std::string workpath)
    {
        path = workpath;
    }

    Editor::~Editor()
    {
        active = false;
        delete window;
        delete field;
        delete renderer;
    }

    void Editor::init()
    {
        //windowBase->setMinimumSize = (MIN_WIDTH, MIN_HEIGHT);
        window = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Vivianite");

        try
        {
            sf::Image icon;
            icon.loadFromFile("vivianite.png");
            sf::Vector2u icon_size = icon.getSize();
            window->setIcon(icon_size.x, icon_size.y, icon.getPixelsPtr());
        }
        catch(...) {}

        codeTexture.create(WIDTH, HEIGHT);
        codeSprite = sf::Sprite();
        codeSprite.setScale(1.f, -1.f);
        codeSprite.setPosition(0.f, HEIGHT);

        codeSprite.setTexture(codeTexture.getTexture());



        //view
        view = new sf::View(sf::FloatRect(0, 0, WIDTH, HEIGHT));

        //prefs
        window->setFramerateLimit(60);
        window->setView(*view);

        //cistomization
        mainFont.loadFromFile("fonts/Menlo.ttf");



        field = new Field();
        renderer = new FieldRenderer(*field, codeTexture, WIDTH, HEIGHT);
    }

    void Editor::abort() { active = false; }

    void Editor::process()
    {
        sf::Event event;

        int dx = 0, dy = 0;

        active = true;

        while (active && window->isOpen())
        {
            while (window->pollEvent(event))
            {
                //close
                if (event.type == sf::Event::Closed) { window->close(); }

                else if (event.type == sf::Event::KeyPressed)
                {
                    window->close();
                }
                else if (event.type == sf::Event::Resized)
                {
                    view->setSize(event.size.width, event.size.height);
                    view->setCenter((event.size.width >> 1) + 1, (event.size.height >> 1) + 1);
                    codeTexture.create(event.size.width + 1, event.size.height + 1);
                    codeSprite.setTexture(codeTexture.getTexture(), true);
                    codeSprite.setPosition(0, event.size.height + 1);

                    window->setView(*view);
                }
            }

            Render();
            renderer->Render();

            window->draw(codeSprite);
            window->display();
        }
        if (!active) { delete window; }
        active = false;


    }

    void Editor::Render()
    {

    }




