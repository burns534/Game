#include "ColorManager.h"

static void error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

ColorManager::ColorManager() {
    this->pair_pool = new short[max_colors];
    static_assert(max_colors == 256);
}

ColorManager::~ColorManager() {
    delete[] pair_pool;
    delete shared_instance;
}

ColorManager * ColorManager::shared() {
    if (!shared_instance) {
        shared_instance = new ColorManager();
    }
    return shared_instance;
}

void ColorManager::create_color(std::string name, Color *color) {
    assert(color);
    color_values[name] = color;
}

void ColorManager::create_pair(std::string name, std::string one, std::string two) {
    Color *one, *two;
    if (!(one = color_values[one]).empty() && !(two = color_values[two]).empty()) {
        color_pairs[name] = ColorPair(one, two);
    } else {
        error("Error: Error creating color pair");
    }
}

void ColorManager::toggle_pair(std::string pair) {
    ColorPair *pair;
    if ((pair = color_pairs[name]) != nullptr) {
        if (pair == active_pair) {
            attroff(COLOR_PAIR(pair->tag));
        } else {
            attron(COLOR_PAIR(pair->tag));
        }
    } else {
        error("Error: Color pair does not exist");
    }
}