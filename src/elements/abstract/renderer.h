#ifndef ANY_RENDERER
#define ANY_RENDERER

#include <cstddef>

class Renderer
{
public:
    virtual void Render(int x, int y, size_t width, size_t height) = 0;

    virtual ~Renderer() = default;
protected:
    Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
};

#endif
