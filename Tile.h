#ifndef Tile_h
#define Tile_h
#include "TileTypes.h"
#include "ViewNode.h"

struct Tile: public ViewNode {
    TileType type;
    bool is_passable;

    Tile();
    Tile(Point position, std::string background_color, std::string foreground_color, TileType type = DEFAULT, bool is_passable = true);
    ~Tile();
};

#endif