#include "map-rewrite.h"

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
    game_window = newwin(height, width, 0, 0);
    /*
        The keypad option enables the keypad of the user's terminal.  If enabled (bf is TRUE), the  user  can
        press  a  function  key (such as an arrow key) and wgetch(3X) returns a single value representing the
        function key, as in KEY_LEFT.  If disabled (bf is FALSE), curses does not treat  function  keys  spe-
        cially  and  the program has to interpret the escape sequences itself.  If the keypad in the terminal
        can be turned on (made to transmit) and off (made to work locally), turning on this option causes the
        terminal keypad to be turned on when wgetch(3X) is called.  The default value for keypad is FALSE.
    */
    keypad(stdscr, TRUE);
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
        The  echo and noecho routines control whether characters typed by the user are echoed by getch(3X) as
        they are typed.  Echoing by the tty driver is always disabled, but initially getch is in  echo  mode,
        so  characters typed are echoed.  Authors of most interactive programs prefer to do their own echoing
        in a controlled area of the screen, or not to echo at all, so they disable echoing by calling noecho.
        [See curs_getch(3X) for a discussion of how these routines interact with cbreak and nocbreak.]
    */
    noecho();

    if (!has_colors() || !can_change_color()) {
        error("Your terminal does not support colors");
    }

    clear(); // clears screen
    
    wbkgd(game_window, 0); // set background to white
}

void Map::configure_colors() {
    ColorManager::shared()->create_color("light-green", 180, 215, 40);
    ColorManager::shared()->create_color("light-blue", 50, 160, 255);
    ColorManager::shared()->create_color("brown", 121, 80, 54);
    ColorManager::shared()->create_color("tan", 220, 200, 160);
    ColorManager::shared()->create_color("orange", 255, 150, 0);
    ColorManager::shared()->create_color("yellow", 255, 255, 30);
    ColorManager::shared()->create_color("green1", 190, 200, 110);
    ColorManager::shared()->create_color("green2", 190, 215, 170);
    ColorManager::shared()->create_color("green3", 90, 110, 40);
    ColorManager::shared()->create_color("green4", 40, 80, 15);
    ColorManager::shared()->create_color("green5", 60, 150, 40);
}

void Map::deconfigure() {
    delwin(game_window);
    endwin(); // unable to close actual terminal that was opened
}

