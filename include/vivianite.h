#ifndef VIVIANITE_H
#define VIVIANITE_H

#include <string>
#include <SDL3/SDL.h>
#include <ft2build.h>
#include FT_FREETYPE_H

extern void Alert(const std::string &msg);

// No Config!
//extern Config *read_config(Tile *tile, const std::string)


// Keys
extern bool IsKeyPressed(int key);
extern bool IsKeyDown(int key);
extern bool IsKeyReleased(int key);

// Resources
extern bool IsFontLoaded(const std::string &path, int size);

extern FT_Face *GetDefaultFont(void);
extern FT_Face *GetFont(void);

extern void SetRenderFont(const FT_Face);

#endif // VIVIANITE_H
