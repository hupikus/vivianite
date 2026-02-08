#include "vivianite.h"
#include "keys.h"
#include <SDL3/SDL.h>

static int keys[SDL_SCANCODE_COUNT] = {0};

bool IsKeyDown(int key)
{
    return (key >= 0 && key < SDL_SCANCODE_COUNT &&
            keys[key] > 0);
}

bool IsKeyPressed(int key)
{
    return (key >= 0 && key < SDL_SCANCODE_COUNT &&
            keys[key] == 1);
}
bool IsKeyReleased(int key)
{
    return (key >= 0 && key < SDL_SCANCODE_COUNT &&
            keys[key] == -1);
}

// Private
void setKey(int key, int value)
{
    keys[key] = value;
}

void updateKeys()
{
    for (int i = 0; i < SDL_SCANCODE_COUNT; ++i) {
        if (keys[i] == 1) keys[i] = 2;
        else if (keys[i] == -1) keys[i] = 0;
    }
}
