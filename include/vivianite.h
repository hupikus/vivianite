#ifndef VIVIANITE_H
#define VIVIANITE_H

#include <string>
#include <SDL3/SDL.h>
#include <ft2build.h>
#include FT_FREETYPE_H

void Alert(const std::string &msg);

// No Config!
//Config *read_config(Tile *tile, const std::string)


// Keys
bool IsKeyPressed(int key);
bool IsKeyDown(int key);
bool IsKeyReleased(int key);

// Resources
bool IsFontLoaded(const std::string &path, int size);

FT_Face *GetDefaultFont();
FT_Face *GetFont();

#endif
