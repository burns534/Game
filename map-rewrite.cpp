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
    start_color();
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
}

