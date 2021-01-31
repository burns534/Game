#ifndef ColorManager_h
#define ColorManager_h

#include <ncurses.h>
#include <map>
#include <stack>
#include "ColorPair.h"

class ColorManager {
    static ColorManager *shared_instance;
    std::map<std::string, Color> color_values;
    std::map<std::string, ColorPair> color_pairs;
    std::stack<char> pair_pool; // holds available ids for color pairs
    std::stack<char> color_pool; // holds available ids for colors
    std::string active_pair;
    ColorManager();
public:
    static ColorManager * shared();
    // provide values 0-255
    void create_color(std::string name, short r, short g, short b);
    void delete_color(std::string name);
    void create_pair(std::string name, std::string foreground_name, std::string background_name);
    void toggle_pair(std::string name);
    void delete_pair(std::string name);
};

#endif