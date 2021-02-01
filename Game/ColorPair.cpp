#include "ColorPair.h"

ColorPair::ColorPair() {}

ColorPair::ColorPair(Color foreground, Color background, short id) {
    this->foreground = foreground;
    this->background = background;
    this->id = id;
}
