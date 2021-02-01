#include "ColorManager.h"

static void error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

ColorManager * ColorManager::shared_instance = nullptr;

ColorManager::ColorManager() {
    /*
        The start_color routine requires no arguments.  It must be called if the programmer wants to use col-
        ors, and before any other color manipulation routine is called.  It is good  practice  to  call  this
        routine right after initscr.  start_color does this:

        o   It  initializes  two  global variables, COLORS and COLOR_PAIRS (respectively defining the maximum
            number of colors and color-pairs the terminal can support).

        o   It initializes the special color pair 0 to the default foreground and background colors.  No oth-
            er color pairs are initialized.

        o   It  restores  the colors on the terminal to the values they had when the terminal was just turned
            on.

        o   If the terminal supports the initc (initialize_color) capability, start_color initializes its in-
            ternal table representing the red, green and blue components of the color palette.

            The  components  depend  on  whether  the  terminal  uses  CGA (aka "ANSI") or HLS (i.e., the hls
            (hue_lightness_saturation) capability is set).  The table is initialized first  for  eight  basic
            colors (black, red, green, yellow, blue, magenta, cyan, and white), and after that (if the termi-
            nal supports more than eight colors) the components are initialized to 1000.

            start_color does not attempt to set the terminal's color palette to match its built-in table.  An
            application may use init_color to alter the internal table along with the terminal's color.

        These  limits  apply to color values and color pairs.  Values outside these limits are not legal, and
        may result in a runtime error:

        o   COLORS corresponds to the terminal database's max_colors capability, (see terminfo(5)).

        o   color values are expected to be in the range 0 to COLORS-1, inclusive (including 0 and COLORS-1).

        o   a  special  color value -1 is used in certain extended functions to denote the default color (see
            use_default_colors).

        o   COLOR_PAIRS corresponds to the terminal database's max_pairs capability, (see terminfo(5)).

        o   legal color pair values are in the range 1 to COLOR_PAIRS-1, inclusive.

        o   color pair 0 is special; it denotes "no color".

            Color pair 0 is assumed to be white on black, but is actually whatever  the  terminal  implements
            before color is initialized.  It cannot be modified by the application.
    */
    if (!has_colors() || !can_change_color()) {
        error("Your terminal does not support colors");
    }
    
    start_color();
    assert(COLOR_PAIRS >= MAX_COLOR_PAIRS);
    assert(COLORS >= MAX_COLORS);
    for (short i = 0; i < MAX_COLOR_PAIRS; i++) {
        pair_pool.push(i);
    }
    for (short i = 0; i < MAX_COLORS; i++) {
        color_pool.push(i);
    }
}

ColorManager * ColorManager::shared() {
    if (!shared_instance) {
        static ColorManager instance;
        shared_instance = &instance; // not thread safe
    }
    return shared_instance;
}

void ColorManager::create_color(std::string name, short r, short g, short b) {
    if (color_values.size() == MAX_COLORS) {
        error("Error: maximum colors created");
    }
    color_values[name] = Color(r, g, b, color_pool.top());
    init_color(color_pool.top(), r * 3.9, g * 3.9, b * 3.9);
    color_pool.pop();
}

void ColorManager::delete_color(std::string name) {
    std::map<std::string, Color>::iterator it = color_values.find(name);
    if (it == color_values.end()) {
        error("Error: Color pair does not exist");
    } else {
        color_pool.push(it->second.id);
        color_values.erase(it);

    }
}
// probably shouldn't be used
chtype ColorManager::color_id(std::string name) {
    std::map<std::string, Color>::iterator it = color_values.find(name);
    if (it == color_values.end()) {
        error("Error: Color pair does not exist");
    }
    return it->second.id;
}

void ColorManager::create_pair(std::string name, std::string foreground_name, std::string background_name) {
    if (color_pairs.size() == MAX_COLOR_PAIRS) {
        error("Error: Maximum number of color pairs created");
    }
    std::map<std::string, Color>::iterator f = color_values.find(foreground_name);
    std::map<std::string, Color>::iterator b = color_values.find(background_name);
    if (f == color_values.end() || b == color_values.end()) {
        error("Error: Supplied invalid color identifier");
    }
    if (color_pool.empty()) {
        error("Error: maximum number of color pairs created");
    } else {
        color_pairs[name] = ColorPair(f->second, b->second, pair_pool.top());
        init_pair(pair_pool.top(), f->second.id, b->second.id);
        pair_pool.pop();
    }
}

void ColorManager::toggle_pair(std::string name) {
    std::map<std::string, ColorPair>::iterator it = color_pairs.find(name);
    if (it == color_pairs.end()) {
        error("Error: Color pair does not exist");
    } else if (name.compare(active_pair) == 0) {
        wattroff(active_window, COLOR_PAIR(it->second.id));
    } else {
        wattron(active_window, COLOR_PAIR(it->second.id));
    }
}

void ColorManager::delete_pair(std::string name) {
    std::map<std::string, ColorPair>::iterator it = color_pairs.find(name);
    if (it == color_pairs.end()) {
        error("Error: Color pair does not exist");
    } else {
        pair_pool.push(it->second.id);
        color_pairs.erase(it);
    }
}

chtype ColorManager::pair_id(std::string name) {
    std::map<std::string, ColorPair>::iterator it = color_pairs.find(name);
    if (it == color_pairs.end()) {
        error("Error: Color pair does not exist");
    }
    return it->second.id;
}

void ColorManager::set_background(std::string name) {
    std::map<std::string, ColorPair>::iterator it = color_pairs.find(name);
    if (it == color_pairs.end()) {
        error("Error: Color pair does not exist");
    }
    
    wbkgd(active_window, COLOR_PAIR(it->second.id));
    refresh();
    wrefresh(active_window);
}

void ColorManager::set_window(WINDOW *win) {
    this->active_window = win;
}

void ColorManager::move_putc(std::string name, int x, int y, char c) {
    std::map<std::string, ColorPair>::iterator it = color_pairs.find(name);
    if (it == color_pairs.end()) {
        error("Error: Color pair does not exist");
    }
    wattron(active_window, COLOR_PAIR(it->second.id));
    mvwaddch(active_window, y, 2 * x, c);
    mvwaddch(active_window, y, 2 * x + 1, 32);
}
