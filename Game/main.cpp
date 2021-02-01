//
//  main.cpp
//  Game
//
//  Created by Kyle Burns on 1/31/21.
//

#include "Game.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cout << "Incorrect argument count\n";
        exit(EXIT_FAILURE);
    }
    int width = atoi(argv[1]), height = atoi(argv[2]);
    
    Game game(width, height);
    game.run();
    Logger::shared()->close();
    return 0;
}
