#include "ColorPair.h"

ColorPair::ColorPair(Color *first, Color *second, short tag) {
    this->first_color = first;
    this->second_color = second;
    this->tag = tag;
}

ColorPair::~ColorPair() {
    delete first_color;
    delete second_color;
}