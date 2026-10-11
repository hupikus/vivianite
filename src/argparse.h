#ifndef ARGPARSE_H
#define ARGPARSE_H

#include <stdbool.h>

struct Args {
    bool help;

    const char *root;
};

struct Args argparse(int argc, char *argv[]);

const char *const HelpMessage =
"-h, --help                 Print this message\n"
"-r, --root {PATH}          Use given .so plugin as the window root (search for system plugins in src/modules)\n";

#endif // ARGPARSE_H
