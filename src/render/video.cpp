#include "vivianite.h"

#include <stdint.h>

typedef struct frames {
    size_t count;
    size_t capacity;
    void **data;
} frames;

typedef struct video {
    videoSpecs specs = {0};

    float time = 0.0;
    frames frames;
} video;

