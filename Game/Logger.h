//
//  Logger.hpp
//  Game
//
//  Created by Kyle Burns on 1/31/21.
//

#ifndef Logger_h
#define Logger_h

#include <stdio.h>
#include <string>

class Logger {
    FILE *fp;
    static Logger *shared_instance;
    Logger();
public:
    static Logger * shared();
    void print(std::string message, std::string string = "");
    void print(std::string message, int value);
    void print(std::string message, double value);
    void close();
};

#endif /* Logger_hpp */
