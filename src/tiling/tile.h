#include <string>
#include <SDL3/SDL.h>

#ifndef TILE_H
#define TILE_H

class Tile
{
private:
    void _draw(SDL_Renderer *renderer, SDL_Rect surface);

public:

    Tile();
    Tile(std::string dlpath);
    Tile(std::string tileName, std::string tileDlPath);


    std::string name = "";

    bool dlLoaded = false;
    std::string dlPath;

    bool content = false;

    void (*drawFunc)(SDL_Renderer *renderer, const SDL_Rect&);

    void Draw(SDL_Renderer *renderer, SDL_Rect surface);

    Tile *first;
    Tile *second;

    double ratio = 0.5;
    bool horizontal = true;

    ~Tile();
};
#endif
