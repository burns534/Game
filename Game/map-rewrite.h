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
public:
    Map(int width, int height);
    void configure_window();
    void deconfigure();
    // variables
    int width, height;
};

#endif

