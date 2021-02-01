#include "Map.h"

static void error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

Map::Map(int width, int height) {
    this->width = width;
    this->height = height;
}

void Map::configure_window() {
    // determines terminal type and initializes all curses data structures
    if (initscr() == nullptr) {
        error("Error initializing screen");
    }
    
    /*
        The  echo and noecho routines control whether characters typed by the user are echoed by getch(3X) as
        they are typed.  Echoing by the tty driver is always disabled, but initially getch is in  echo  mode,
        so  characters typed are echoed.  Authors of most interactive programs prefer to do their own echoing
        in a controlled area of the screen, or not to echo at all, so they disable echoing by calling noecho.
        [See curs_getch(3X) for a discussion of how these routines interact with cbreak and nocbreak.]
    */
    noecho();
    /*
         Normally,  the  tty driver buffers typed characters until a newline or carriage return is typed.  The
         cbreak routine disables line buffering and erase/kill character-processing (interrupt and  flow  con-
         trol  characters  are  unaffected),  making characters typed by the user immediately available to the
         program.  The nocbreak routine returns the terminal to normal (cooked) mode.

         Initially the terminal may or may not be in cbreak mode, as the mode is inherited; therefore, a  pro-
         gram  should  call  cbreak  or  nocbreak  explicitly.  Most interactive programs using curses set the
         cbreak mode.  Note that cbreak overrides raw.  [See curs_getch(3X) for a discussion of how these rou-
         tines interact with echo and noecho.]
    */
     cbreak();
    /*
        The keypad option enables the keypad of the user's terminal.  If enabled (bf is TRUE), the  user  can
        press  a  function  key (such as an arrow key) and wgetch(3X) returns a single value representing the
        function key, as in KEY_LEFT.  If disabled (bf is FALSE), curses does not treat  function  keys  spe-
        cially  and  the program has to interpret the escape sequences itself.  If the keypad in the terminal
        can be turned on (made to transmit) and off (made to work locally), turning on this option causes the
        terminal keypad to be turned on when wgetch(3X) is called.  The default value for keypad is FALSE.
    */
    keypad(stdscr, true);
    /*
     Calling newwin creates and returns a pointer to a new window  with  the
            given  number  of lines and columns.  The upper left-hand corner of the
            window is at
                   line begin_y,
                   column begin_x

            If either nlines or ncols is zero, they default to
                   LINES - begin_y and
                   COLS - begin_x.

            A new full-screen window is created by calling newwin(0,0,0,0).
     */
    curs_set(0);
    clear();
    game_window = newwin(height, width, 0, 0);
}

void Map::configure_colors() {
    // maximum 15 because I need 1 slot for the inverse color cursor
    ColorManager::shared()->create_color("red", 255, 0, 0);
    ColorManager::shared()->create_color("blue", 38, 44, 242);
    ColorManager::shared()->create_color("dark-blue", 16, 22, 205);
    ColorManager::shared()->create_color("light-blue", 50, 160, 255);
    ColorManager::shared()->create_color("white", 255, 255, 255);
    ColorManager::shared()->create_color("black", 0, 0, 0);
    ColorManager::shared()->create_color("brown", 121, 80, 54);
    
    ColorManager::shared()->create_color("light-green", 180, 215, 40);
    ColorManager::shared()->create_color("tan", 220, 200, 160);
    
    ColorManager::shared()->create_color("green", 0, 204, 0);
    ColorManager::shared()->create_color("green1", 190, 200, 110);
    ColorManager::shared()->create_color("green3", 90, 110, 40);
    ColorManager::shared()->create_color("green4", 40, 80, 15);
    ColorManager::shared()->create_color("green5", 60, 150, 40);
    
    ColorManager::shared()->create_pair("default", "black", "white");
    ColorManager::shared()->create_pair("dirt", "black", "brown");
    ColorManager::shared()->create_pair("grass1", "green", "green5");
    ColorManager::shared()->create_pair("grass2", "green1", "green4");
    ColorManager::shared()->create_pair("shallow-water", "light-blue", "blue");
    ColorManager::shared()->create_pair("deep-water", "blue", "dark-blue");
    
    ColorManager::shared()->set_window(game_window);
}

void Map::generate_terrain(float zoom, float offset) {
    for (int i = 0; i < width * height; i++) {
        int x_int = i % width, y_int = i / width;
        float x = (float)(x_int) / width, y = (float)(y_int) / height;
        float noise_value = 0.5 * SimplexNoise::noise(zoom * x + offset, zoom * y + offset) + 0.5;
        Tile *tile;
        if (noise_value < 0.15) {
            tile = new Tile(x_int, y_int, "deep-water", SHALLOW_WATER, noise_value, '~');
        } else if (noise_value < 0.3) {
            tile = new Tile(x_int, y_int, "shallow-water", DEEP_WATER, noise_value, '~');
        } else if (noise_value < 0.8) {
            tile = new Tile(x_int, y_int, "grass1", GRASS, noise_value, '/');
        } else {
            tile = new Tile(x_int, y_int, "dirt", DIRT, noise_value, '#');
        }
        world.push_back(tile);
    }
}

void Map::render() {
    for (int i = 0; i < width * height; i++) {
        world[i]->render();
    }
}

void Map::deconfigure() {
    delwin(game_window);
    endwin(); // unable to close actual terminal that was opened
}

int Map::get_char() {
    return wgetch(game_window);
}

