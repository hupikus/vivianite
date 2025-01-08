#include <iostream>
#include <string>
//#include <thread>
#ifndef MAIN_CPP
#define MAIN_CPP

#include "editor.h"


//theme

int main(int argc, char* argv[])
{
    std::string path;
    if (argc > 2 && *argv[1] == '-' && *(argv[1] + 1) == 't') { path = argv[2]; }

    Editor editor(path);
    editor.init();
    std::cout << "\033[0;32mSuccesful\033[0m\n";
    //std::thread editor_process(&Editor::process, &editor);
    //editor_process.detach();
    editor.process();

    return 0;
}
#endif
