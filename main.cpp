#include "map1.cpp"

int main() {
    srand(time(nullptr));
    std::ofstream outfile;
    outfile.open("debug.txt");
    Map<119,73> map; // Jeez you could just start not quite in the center
    char c;
    //map.debug();
    while((c = getch()) != 'q') {
      map.move(c);
      //map.debug();
      //cout << "current pos: " << map.getx() << ", " << map.gety() << endl;
    }
    outfile.close();
    return 0;
}
