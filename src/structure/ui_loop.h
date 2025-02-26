#include <memory>
#include <chrono>

#include "compositor.h"



#ifndef UILOOP
#define UILOOP

class UILoop
{
public:
    UILoop(size_t width, size_t height);

    void Init();

private:
    std::unique_ptr<Compositor> compositor;

    std::chrono::duration<long, std::ratio<1, 1000>> process_pause;
    bool active = true;

    void Render();
    void Process();
};
#endif
