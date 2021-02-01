#ifndef Tile_h
#define Tile_h
#include "TileTypes.h"
#include "ViewNode.h"
#include "ColorManager.h"

struct Tile: public ViewNode {
    TileType type;
    bool is_passable, is_occupied;
    char symbol;
    
    Tile();
    Tile(int x, int y, std::string color, TileType type, double zPosition, char symbol);
    Tile(Point position, std::string color, TileType type, bool is_passable, double zPosition, char symbol);
    ~Tile();
    
    void render();
};

#endif
