#include "vivianite.h"
#include <stdbool.h>
#include <string.h>
#include <memory.h>

#ifndef TILE_H
#define TILE_H

class Tile
{
private:
    void *handle = NULL;
public:

    Tile();
    Tile(std::string dlpath);
    Tile(std::string tileName, std::string tileDlPath, bool load);


    std::string name = "";

    bool dlLoaded = false;
    std::string dlPath;

    void DlLoad();

    void (*initFunc)(void) = NULL;
    void (*drawFunc)(SDL_Renderer*, const int width, const int height) = NULL;
    void (*destroyFunc)(void) = NULL;

    bool content = false;
    void Draw(SDL_Renderer* r, SDL_Rect& surface);

    Tile *first = NULL;
    Tile *second = NULL;
    std::vector<Tile *> children = {0};

    double ratio = 0.5;
    bool horizontal = true;

    ~Tile();
};
#endif // TILE_H
