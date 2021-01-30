#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <ncurses.h>

#include "Tile.h"

class Map {
    std::vector<Tile *> world;
public:
    Map(int width, int height);
    void configure_window();

    // variables
    int width, height;
};



