#include "editor.h"
class Window
{
private:
    int width, height;
    //float pos_x, pos_y;
    //float velocity_x = 100.0f, velocity_y = 0.0f;

    //Editor* editor;

public:
    Window(int window_width, int window_height);
    ~Window();

    void InitUI();
    void Render();

};
