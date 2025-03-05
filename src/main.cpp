#include <string>

#include "structure/ui_loop.h"
#include "window.h"
#include "globals/resources.h"

#define WIDTH 900
#define HEIGHT 600



//theme

int main(int argc, char* argv[])
{
    std::string path;
    if (argc > 2 && *argv[1] == '-' && *(argv[1] + 1) == 't') { path = argv[2]; }

    Window* window = new Window(WIDTH, HEIGHT);


    LoadResources();

    UILoop loop(WIDTH, HEIGHT);
    loop.Start();
    //std::thread editor_process(&Editor::process, &editor);
    //editor_process.detach();


    loop.Abort();
    delete window;

    return 0;
}
