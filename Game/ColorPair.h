#include "Color.h"

struct ColorPair {
    Color foreground, background;
    short id;
    ColorPair();
    ColorPair(Color foreground, Color background, short id);
};
