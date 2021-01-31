#include "ColorPair.h"

ColorPair::ColorPair() {}

ColorPair::ColorPair(Color foreground, Color background, short tag) {
    this->foreground = foreground;
    this->background = background;
    this->tag = tag;
}