#include "Tile.h"

Tile::Tile(Point *position, Color *background_color, Color *foreground_color, TileType type = DEFAULT, bool is_passable = true) {
    this->position = position;
    this->background_color = background_color;
    this->foreground_color = foreground_color;
    this->is_passable = is_passable;
    this->type = type;
}

Tile::~Tile() {
    delete position;
    delete background_color;
    delete foreground_color;
}
