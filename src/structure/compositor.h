#include <raylib.h>

#include <string>
#include <vector>
#include <array>
#include <memory>

#include "../globals/colors.h"
#include "../modules/code/editor.h"

#ifndef COMPOSITOR
#define COMPOSITOR

struct Tile
{
    bool tab = false;

    bool vertical = false;
    float ratio = 0.5f;

    std::string name;
    //Tab* instance;
    std::unique_ptr<Tab> instance;
    std::array<std::unique_ptr<Tile>, 2> layout = {nullptr, nullptr};
};


class Compositor
{
public:

    Compositor();

    void Init();

    void Render();
    void Process();

    void Abort();

private:

    bool active = true;

    size_t width;
    size_t height;
    float deltatime;

    //Panel system
    std::unique_ptr<Tile> root_tile;
    std::unique_ptr<Tile>* focus_tile;


    void chroot(std::unique_ptr<Tile>& root, int posx, int posy, size_t width, size_t height, bool start);

    void keys();



    int click_x, click_y = 0;
    bool click = false;


    //Render special
    Color& inactive = inactiveColor;

};

#endif
