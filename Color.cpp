#include "Color.h"

Color::Color() {
    r = g = b = id = 0;
}

Color::Color(short r, short g, short b, short id) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->id = id;
}