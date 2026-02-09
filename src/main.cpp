#include "window.h"
#include <filesystem>

#define WIDTH 900
#define HEIGHT 600

const char *start_path;

int main(int argc, char* argv[])
{
    Window* window = new Window(WIDTH, HEIGHT);
    if (window->Init()) {
        return 1;
    }
    window->Loop();

    delete window;

    return 0;
}
