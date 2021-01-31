#ifndef Map_h
#define Map_h

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <random>

#include "Tile.h"
#include "ColorManager.h"

class Map: public Node {
    std::vector<Tile *> world;
    WINDOW *game_window;
public:
    Map(int width, int height);
    void configure_window();
    void configure_colors();
    void deconfigure();
    // variables
    int width, height;
};

#endif

