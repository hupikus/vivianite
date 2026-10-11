#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <unistd.h>
#include <limits.h>
#include <string.h>

#include "window.h"
#include "tiling/compositor.h"

#include "argparse.h"

#define WIDTH 900
#define HEIGHT 600


void exe_path(void)
{
    #ifdef _WIN32
        #include <windows.h>
        wchar_t path[MAX_PATH];
        DWORD len = GetModuleFileNameW(NULL, path, MAX_PATH);

        #define cd _wchdir
    #else
        char path[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", path, PATH_MAX);

        #define cd chdir
    #endif
    if (len <= 0) return;
    path[len] = 0;
    std::cout << path << std::endl;

    std::filesystem::path dir(path);
    cd(dir.parent_path().c_str());
}

int main(int argc, char* argv[])
{
    exe_path();

    struct Args args = argparse(argc, argv);
    if (args.help) {
        printf(HelpMessage);
        return 0;
    }


    Window* main_window = new Window(WIDTH, HEIGHT);

    if (args.root) {
        main_window->window_root = strdup(argv[2]);
        main_window->cli_render_mode = true;
    } else {
        main_window->window_root = strdup(STARTSCREEN_MODULE);
        main_window->cli_render_mode = false;
    }

    if (main_window->Init()) { return 1; }
    main_window->Loop();

    delete main_window;

    return 0;
}
