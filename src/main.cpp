#include "window.h"

#define WIDTH 900
#define HEIGHT 600

// theme

int main(int argc, char* argv[])
{
    Window* window = new Window(WIDTH, HEIGHT);
    window->Init();
    return window->Loop();
}
