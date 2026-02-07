#include "vivianite.h"
#include <string>
#include <boost/dll/shared_library.hpp>
#include <memory>

#ifndef TILE_H
#define TILE_H

class Tile
{
private:
    std::unique_ptr<boost::dll::shared_library> lib;
public:

    Tile();
    Tile(std::string dlpath);
    Tile(std::string tileName, std::string tileDlPath, bool load);


    std::string name = "";

    bool dlLoaded = false;
    std::string dlPath;

    void DlLoad();

    void (*initFunc)(void) = nullptr;
    void (*drawFunc)(SDL_Renderer*,   SDL_Rect&) = nullptr;
    void (*destroyFunc)(void) = nullptr;

    bool content = false;
    void Draw(SDL_Renderer* r, SDL_Rect& surface);

    Tile *first = nullptr;
    Tile *second = nullptr;

    double ratio = 0.5;
    bool horizontal = true;

    ~Tile();
};
#endif
