#ifndef Map_h
#define Map_h

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>

#include "Tile.h"
#include "ColorManager.h"
#include "SimplexNoise.h"

class Map: public Node {
    std::vector<Tile *> world;
    WINDOW *game_window;
public:
    Map(int width, int height);
    void configure_window();
    void configure_colors();
    void generate_terrain(float zoom = 1.0f, float offset = 0.0f);
    void render();
    void deconfigure();
    // variables
    int width, height;
    int get_char();
};

#endif

