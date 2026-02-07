#include <string>
#include <SDL3/SDL.h>

#ifndef VIVIANITE_H
#define VIVIANITE_H

void Alert(const std::string &msg);

// No Config!
//Config *read_config(Tile *tile, const std::string)


// Keys
bool isKeyPressed(int key);
bool IsKeyDown(int key);
bool isKeyReleased(int key);

#endif
