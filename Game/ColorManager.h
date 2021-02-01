#ifndef ColorManager_h
#define ColorManager_h

#include <map>
#include <stack>
#include "ColorPair.h"
#include "Logger.h"

#define MAX_COLORS 16
#define MAX_COLOR_PAIRS 256

class ColorManager {
    static ColorManager *shared_instance;
    std::map<std::string, Color> color_values;
    std::map<std::string, ColorPair> color_pairs;
    std::stack<short> pair_pool; // holds available ids for color pairs
    std::stack<short> color_pool; // holds available ids for colors
    std::string active_pair;
    WINDOW *active_window;
    ColorManager();
public:
    static ColorManager * shared();
    // provide values 0-255
    void create_color(std::string name, short r, short g, short b);
    void delete_color(std::string name);
    chtype color_id(std::string name);
    void create_pair(std::string name, std::string foreground_name, std::string background_name);
    void toggle_pair(std::string name);
    void delete_pair(std::string name);
    chtype pair_id(std::string name);
    void set_background(std::string name);
    void set_window(WINDOW *win);
    void move_putc(std::string name, int x, int y, char c);
};

#endif
