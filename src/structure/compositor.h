#include <string>
#include <vector>
#include <memory>

#include "../elements/abstract/tab.h"

#include "../editor.h"

#ifndef COMPOSITOR
#define COMPOSITOR

typedef struct CompositorTab
{
    size_t id;
    std::string name;
    std::unique_ptr<Tab> instance;

    float posx = 0.0f;
    float posy = 0.0f;
    float width = 1.0f;
    float height = 1.0f;

} CompositorTab;


class Compositor
{
public:
    std::vector<CompositorTab> tabs;

    Compositor();

    void Init();

    void Render();

private:
    bool active = true;

    size_t width;
    size_t height;

    float deltatime;
};

#endif
