#include "map-rewrite.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cout << "Incorrect argument count\n";
        exit(EXIT_FAILURE);
    }
    int width = atoi(argv[1]), height = atoi(argv[2]);
    Map map(width, height);
    map.configure_window();
    char c;
    while((c = getch()) != 'q') {
        
    }
    map.deconfigure();
    return 0;
}
