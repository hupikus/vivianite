#include <string>
#ifndef MAIN_CPP
#define MAIN_CPP

#include "editor.h"
#include "window.h"


#define WIDTH 900
#define HEIGHT 600



//theme

int main(int argc, char* argv[])
{
    std::string path;
    if (argc > 2 && *argv[1] == '-' && *(argv[1] + 1) == 't') { path = argv[2]; }

    Window window(WIDTH, HEIGHT);
    //std::thread editor_process(&Editor::process, &editor);
    //editor_process.detach();

    return 0;
}
#endif
