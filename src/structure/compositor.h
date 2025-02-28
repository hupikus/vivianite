#include <string>
#include <vector>
#include <memory>

#include "../modules/code/editor.h"

#ifndef COMPOSITOR
#define COMPOSITOR

struct Tile
{
    bool tab = true;

    bool vertical = false;
    float ratio = 0.5f;

    std::string name;
    //Tab* instance;
    std::unique_ptr<Tab> instance;
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
    std::vector<std::unique_ptr<Tile>> layout;

};

#endif
