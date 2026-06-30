#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include "window.h"


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

    if (argc >= 2 && (strcmp(argv[1], "--help") == 0 || (strcmp(argv[1], "-h") == 0))) {
        printf(
            "-h, --help                 Print this message\n"
            "-r, --root {PATH}          Use given .so plugin as window root\n"
        );
        return 0;
    }

    Window* window = new Window(WIDTH, HEIGHT);
    if (argc >= 3 && (strcmp(argv[1], "--root") == 0 || (strcmp(argv[1], "-r") == 0))) {
        window->window_root = strdup(argv[2]);
    }

    if (window->Init()) {
        return 1;
    }
    window->Loop();

    delete window;

    return 0;
}
