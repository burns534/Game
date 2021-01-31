#include "Tile.h"

Tile::Tile() {
    this->is_passable = true;
    this->type = DEFAULT;
}

Tile::Tile(Point position, std::string background_color, std::string foreground_color, TileType type, bool is_passable) {
    this->position = position;
    this->background_color = background_color;
    this->foreground_color = foreground_color;
    this->is_passable = is_passable;
    this->type = type;
}

Tile::~Tile() {}
