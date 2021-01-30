#include "Color.h"

struct ColorPair {
    Color *first_color;
    Color *second_color;
    short tag;
    ColorPair(Color *first, Color *second, short tag);
    ~ColorPair();
};