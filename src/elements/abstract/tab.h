#pragma once

#include <cstddef>
#include <memory>

#ifndef ANY_TAB
    #define ANY_TAB

class Tab {
public:
    Tab() = default;
    virtual void Start() = 0;

    virtual void Render(int pos_x, int pos_y, size_t width, size_t height, float deltatime) = 0;
    virtual void Input(int pos_x, int pos_y, size_t width, size_t height, float deltatime) = 0;

    virtual void Abort() = 0;

    virtual ~Tab() = default;

    virtual std::unique_ptr<Tab> NewInstance() = 0;
};

#endif
