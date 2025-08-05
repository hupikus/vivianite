#pragma once

#include <raylib.h>
#include <cstddef>

#ifndef SHARED_PREFS
    #define SHARED_PREFS

typedef struct ColorSet {
    Color text;
    Color background;
    Color acccent;
    Color shadow;
    Color inactive;
} ColorSet;

typedef struct iRect {
    int pos_x, pos_y;
    size_t width, height;
} iRect;

typedef struct fRect {
    float pos_x, pos_y;
    float width, height;
} fRect;

#endif
