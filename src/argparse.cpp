#include "argparse.h"

#include <string.h>

// We don't need advanced argument parsing until there's at least 2 arguments other than help
struct Args argparse(int argc, char *argv[])
{
    struct Args args = {0};

    if (argc < 2) { return args; }

    if (strcmp(argv[1], "--help") == 0 || (strcmp(argv[1], "-h") == 0)) {
        args.help = true;
        return args;
    }

    if (argc < 3) { return args; }

    if (strcmp(argv[1], "--root") == 0 || (strcmp(argv[1], "-r") == 0)) {
        args.root = strdup(argv[1]);
    }

    return args;
}
