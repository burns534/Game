//
//  Game.hpp
//  Game
//
//  Created by Kyle Burns on 2/1/21.
//

#ifndef Game_h
#define Game_h
#include "Map.h"

class Game {
    Map *map;
public:
    Game(int width, int height);
    ~Game();
    void run();
};

#endif /* Game_hpp */
