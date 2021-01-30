#include "Color.h"

struct ColorPair {
    Color foreground, background;
    short tag;
    ColorPair();
    ColorPair(Color foreground, Color background, short tag);
};