//
//  Logger.cpp
//  Game
//
//  Created by Kyle Burns on 1/31/21.
//

#include "Logger.h"

Logger * Logger::shared_instance = nullptr;

Logger::Logger() {
    fp = fopen("log.txt", "w+");
}

Logger * Logger::shared() {
    if (!shared_instance) {
        shared_instance = new Logger();
    }
    return shared_instance;
}

void Logger::print(std::string message, std::string string) {
    fprintf(fp, "%s%s\n", message.c_str(), string.c_str());
}

void Logger::print(std::string message, int value) {
    fprintf(fp, "%s%d\n", message.c_str(), value);
}

void Logger::print(std::string message, double value) {
    fprintf(fp, "%s%f\n", message.c_str(), value);
}

void Logger::close() {
    fclose(fp);
}
