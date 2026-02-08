#ifndef VIVIANITE_H
#define VIVIANITE_H

#include <string>
#include <SDL3/SDL.h>

void Alert(const std::string &msg);

// No Config!
//Config *read_config(Tile *tile, const std::string)


// Keys
bool IsKeyPressed(int key);
bool IsKeyDown(int key);
bool IsKeyReleased(int key);

#endif
