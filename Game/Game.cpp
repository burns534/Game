//
//  Game.cpp
//  Game
//
//  Created by Kyle Burns on 2/1/21.
//

#include "Game.h"

Game::Game(int width, int height) {
    this->map = new Map(width, height);
}

Game::~Game() {
    delete map;
}

void Game::run() {
    char ch;
    map->configure_window();
    map->configure_colors();
    map->generate_terrain(2.0, (float) rand() / RAND_MAX);
    map->render();
    while((ch = map->get_char()) != 'q') {
        
    }
    map->deconfigure();
}
