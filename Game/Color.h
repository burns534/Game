#include <string>
#include <ncurses.h>

struct Color {
    std::string name;
    short r, g, b, id;
    Color();
    Color(short r, short g, short b, short id);
};
