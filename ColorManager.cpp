#include "ColorManager.h"

static void error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

ColorManager::ColorManager() {
    assert(COLOR_PAIRS >= 256);
    assert(COLORS >= 16);
    for (char i = 0; i < COLOR_PAIRS; i++) {
        pair_pool.push(i);
    }
    for (char i = 0; i < COLORS; i++) {
        color_pool.push(i);
    }
}

ColorManager::~ColorManager() {
    delete shared_instance;
}

ColorManager * ColorManager::shared() {
    if (!shared_instance) {
        shared_instance = new ColorManager();
    }
    return shared_instance;
}

void ColorManager::create_color(std::string name, short r, short g, short b) {
    if (color_values.size() == COLORS) {
        error("Error: maximum colors created");
    }
    color_values[name] = Color(r, g, b, color_pool.top());
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

void ColorManager::create_pair(std::string name, std::string foreground_name, std::string background_name) {
    std::map<std::string, Color>::iterator f = color_values.find(foreground_name);
    std::map<std::string, Color>::iterator b = color_values.find(background_name);
    if (f == color_values.end() || b == color_values.end()) {
        error("Error: Supplied invalid color identifier");
    }
    if (color_pool.empty()) {
        error("Error: maximum number of color pairs created");
    } else if (color_values.find(foreground_name) != color_values.end() && color_values.find(background_name) != color_values.end()) {
        color_pairs[name] = ColorPair(color_values[foreground_name], color_values[background_name], color_pool.top());
        color_pool.pop();
    } else {
        error("Error: Error creating color pair");
    }
}

void ColorManager::toggle_pair(std::string name) {
    std::map<std::string, ColorPair>::iterator it = color_pairs.find(name);
    if (it == color_pairs.end()) {
        error("Error: Color pair does not exist");
    } else if (name.compare(active_pair) == 0) {
        attroff(COLOR_PAIR(it->second.tag));
    } else {
        attron(COLOR_PAIR(it->second.tag));
    }
}

void ColorManager::delete_pair(std::string name) {
    std::map<std::string, ColorPair>::iterator it = color_pairs.find(name);
    if (it == color_pairs.end()) {
        error("Error: Color pair does not exist");
    } else {
        pair_pool.push(it->second.tag);
        color_pairs.erase(it);
    }
}