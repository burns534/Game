#include <ncurses.h>
#include <map>
#include "ColorPair.h"

class ColorManager {
    static ColorManager *shared_instance;
    std::map<std::string, Color *> color_values;
    std::map<std::string, ColorPair *> color_pairs;
    short *pair_pool;
    ColorPair *active_pair;
    ColorManager();
    ~ColorManager();
public:
    ColorManager * shared();

    void create_color(std::string name, Color *color);
    void create_pair(std::string name, Color *one, Color *two);
    void toggle_pair(std::string pair);
};