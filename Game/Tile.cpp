#include "Tile.h"

Tile::Tile() {
    this->is_passable = true;
    this->type = DEFAULT;
    this->zPosition = 0;
    this->color = "default";
    this->symbol = 'K';
    this->is_occupied = false;
}

Tile::Tile(int x, int y, std::string color, TileType type, double zPosition, char symbol) {
    this->position = Point(x, y);
    this->color = color;
    this->zPosition = zPosition;
    this->symbol = symbol;
    this->is_passable = true;
    this->is_occupied = false;
}

Tile::Tile(Point position, std::string color, TileType type, bool is_passable, double zPosition, char symbol) {
    this->position = position;
    this->color = color;
    this->is_passable = is_passable;
    this->type = type;
    this->zPosition = zPosition;
    this->symbol = symbol;
    this->is_occupied = false;
}

Tile::~Tile() {}

void Tile::render() {
    if (is_occupied) {
        attron(A_BLINK);
        ColorManager::shared()->move_putc(color, position.x, position.y, symbol);
        attroff(A_BLINK);
    } else {
        ColorManager::shared()->move_putc(color, position.x, position.y, symbol);
    }
}
