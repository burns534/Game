#include "Point.h"
#include "TileTypes.h"
#include "Color.h"
#include <string>

struct Tile {
    Point *position;
    TileType type;
    Color *background_color, *foreground_color;
    bool is_passable;

    Tile(Point *position, Color *background_color, Color *foreground_color, TileType type = DEFAULT, bool is_passable = true);
    ~Tile();
};