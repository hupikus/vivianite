#include <cstddef>
class Window
{
private:
    size_t width, height;
    //float pos_x, pos_y;
    //float velocity_x = 100.0f, velocity_y = 0.0f;
public:
    Window(int window_width, int window_height);
    ~Window();
};
