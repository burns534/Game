#include <iostream>
#include <string>
#include <curses.h>
#include <dict>
#include <random>
#include <vector>
#include <fstream>

#define HERE_G 5
#define LG_G 6
#define LB_B 7
#define LIGHT_GREEN 8
#define HERE_B 9
#define LIGHT_BLUE 10
#define COLOR_BROWN 11
#define BROWN_BLK 12

std::ofstream outfile;
struct Coord
{
  int x,y;
};

class Tile
{
  private:
    Coord location;
    bool visited;
    std::string name;
    char symbol;
    bool can_pass;
    bool blink;
    short color_pair;
    short background;
    short foreground;
    bool is_water;
    Tile** Nbors;
    Tile** Nbref;
  public:
    Tile* up;
    Tile* down;
    Tile* left;
    Tile* right;
    Tile(int x, int y)
    {
      location.x = x;
      location.y = y;
      visited = false;
      up = down = left = right = NULL;
      can_pass = true;
      blink = false;
      symbol = '/';
      is_water = false;
      init_color(LIGHT_GREEN, 700, 850, 149);
      init_color(LIGHT_BLUE, 359, 846, 1000);
      init_color(COLOR_BROWN, 121*3.9, 80*3.9, 54*3.9);
      init_pair(LG_G, LIGHT_GREEN, COLOR_GREEN);
      init_pair(HERE_G, COLOR_BROWN, COLOR_GREEN);
      init_pair(HERE_B, LIGHT_BLUE, COLOR_BLUE);
      init_pair(BROWN_BLK, -1, COLOR_BROWN);
      init_pair(LB_B, LIGHT_BLUE, COLOR_BLUE);
      color_pair = LG_G;
      //std::cout << "Tile ctor\n";
    }
    ~Tile() {}
    /* need cpy ctor */
    Coord* getpos() { return &location; }
    void setpos(int x, int y) { location.x = x; location.y = y; }
    void setpos(const Coord &obj) { location = obj; }
    void setsymbol(char c) { symbol = c; }
    char getsymbol() { return symbol; }
    void setcolor(short color) { color_pair = color; }
    short getcolor() { return color_pair; }
    void setblink(bool b) { blink = b; }
    bool getblink() { return blink; }
    void setcan_pass(bool can) { can_pass = can; }
    bool getcan_pass() { return can_pass; }
    void render(bool here)
    {
      //has to use colorpair
      //if (!can_pass) return;
      char temp = symbol;
      short tempcolor = color_pair;
      if (here)
      {
        color_pair = HERE_G;
        temp = 'X';
        blink = 1;
      }
      if(blink) attron(A_BLINK);
      attron(COLOR_PAIR(color_pair));
      mvaddch(location.y, location.x * 2, temp);
      if(!here) mvaddch(location.y, location.x *2 + 1, ' ');
      attroff(A_BLINK);
      attroff(COLOR_PAIR(color_pair));
      blink = false;
      color_pair = tempcolor;
    }
    Tile** &neighbors(int radius)
    {
      Nbors = Nbref; delete[] Nbors;
      Nbors = new Tile*[(2*radius + 1) * (2*radius + 1)];
      Nbref = Nbors;
      Tile * current = this->up;
      for(int i = 0; i < radius-1 && current->up; i++) current = current->up;
      for(int i = 0; i < radius && current->left; i++) current = current->left;
      Tile * topleft = current;
      for(int i = 0; i < 2 * radius + 1 && current->down; i++, current = current->down)
      {
        topleft = current;
        outfile << "here!\n";
        for (int k = 0; k < 2*radius + 1 && current->right; k++, current = current->right, Nbors++)
        {
          *Nbors = current;
          outfile << "position: " << current->up->getpos()->x <<
          ", " << current->up->getpos()->y << "\n";
        }
        current = topleft;
      }
      Nbors = Nbref;
      topleft = NULL; current = NULL;
      delete topleft; delete current;
      return Nbors;
    }

};

class Dice
{
  public:
    Dict<char> places;
    long prev;
    Dice(): prev(0) {};
    int roll(int sides)
    {
      sides++;
      srand( time(NULL) + prev); //seems to work pretty well..
      prev+=rand() % 137;
      return (rand()%sides + rand()%sides);
    }

    bool drop(int prob, int precision)
    {
      srand( time(NULL) + prev);
      prev += rand() % 137;
      return (rand() % precision < prob)? true:false;
    }
};

class Map
{
  private:
    Tile* start;
    Tile* pos;
    Dice* d;

  public:
    Map()
    {
      outfile.open("log.txt");
      initscr(); curs_set(0); use_default_colors();
      start_color(); keypad(stdscr, TRUE);
      cbreak(); noecho();
      if (!has_colors() || !can_change_color())
      {
        endwin();
        std::cout << "Your terminal does not support color\n";
        exit(1);
      }
      clear();
      outfile << "breakpoint\n";
      d = new Dice();
      start = new Tile(0,0);
      Tile* start1 = new Tile(0,1);
      pos = start;
      Tile* pos1 = start;
      for(int k = 0; k < 50; k++)
      {
        for (int i = 1; i < 51; i++)
        {
          //link new tile to the right, only need new on first row
          if (k == 0) start->right = new Tile(i,0);
          // if(d->roll(10) > 19)
          // {
          //   start->right->setsymbol(' ');
          //   start->right->setcan_pass(false);
          // }
          start->right->left = start; //link new tile to self. redundant
          start1->right = new Tile(i, k+1); //link new tile to right
          // if(d->roll(10) > 19)
          // {
          //   start1->right->setsymbol(' ');
          //   start1->right->setcan_pass(false);
          // }
          start1->right->left = start1; //link new tile to self
          start1->up = start; //link lower to upper
          start->down = start1; //link upper to lower
          start = start->right; //move right
          start1 = start1->right; //move right
          start1->up = start; //link the last column upper and lower
          start->down = start1;
        }
        if(k == 49) break;
        start = pos1;
        start1 = start->down;
        start = start1; //move down
        start1->down = new Tile(0, k+2); //make space to move down
        start1 = start1->down; //move down
        pos1 = start;
      }
      pos1 = NULL; start1 = NULL;
      delete pos1; delete start1;
      start = pos;

      //generates terrain variation e.g. lakes, dirt, mountains
      outfile << "breakpoint 1\n";
      for (int i = 0; i < 5; i++)
      {
        int sz = 2 * d->roll(2) + 1;
        int randx = rand() % (50-2*(sz+1)) + sz;
        int randy = rand() % (50-2*(sz+1)) + sz;
        for(int i = 0; i < randx; i++) start = start->right;
        for(int i = 0; i < randy; i++) start = start->down;
        terrain(LB_B, start, false, 50, 100, sz, false);
        start = pos;
      }
      //puts player at random starting location within bounds
      int randx = rand() % 30 + 10; int randy = rand() % 30 + 10;
      for(int i = 0; i < randx; i++) start = start->right;
      for(int i = 0; i < randy; i++) start = start->down;
      pos = start;
      start->render(1); //sets starting position to red X
      vision(start);
    }
    ~Map()
    {
      endwin(); exit(1); outfile.close();
    }
    void terrain(short colorpair, Tile* position, bool passable,
      int prob, int precision, short size, bool water)
    {
      outfile << "breakpoint2\n";
      if(!d->drop(prob, precision)) return;
      position->setcolor(colorpair);
      position->setcan_pass(passable);
      position->render(false);
      bool river = false;
      outfile << "breakpoint3\n";
      if(water && d->drop(30,100)) river = true;
      outfile << "breakpoint4\n";
      if (!river)
      {
        Tile** area = position->neighbors(size);
        outfile << "breakpoint5\n";
        for(;*area; area++)
        {
          //just did a complete fill for now
          (*area)->setcan_pass(passable);
          (*area)->setcolor(colorpair);
          (*area)->render(false);
        }
        area = NULL;
        delete area;
      }
    }
    void vision(Tile* po)
    {
      if (po->right) po->right->render(0);
      if (po->right && po->right->right) po->right->right->render(0);
      if (po->left) po->left->render(0);
      if (po->left && po->left->left) po->left->left->render(0);
      if (po->up) po->up->render(0);
      if (po->up && po->up->up) po->up->up->render(0);
      if (po->down) po->down->render(0);
      if (po->down && po->down->down) po->down->down->render(0);
      if (po->right && po->right->up) po->right->up->render(0);
      if (po->right && po->right->down) po->right->down->render(0);
      if (po->left && po->left->down) po->left->down->render(0);
      if (po->left && po->left->up) po->left->up->render(0);
    }
    int getx() { return pos->getpos()->x; }
    int gety() { return pos->getpos()->y; }
    void move(char in)
    {
      Tile* temp = pos;
      //std::cout << pos->getpos()->x << " " << pos->getpos()->y << "\n";
      if(in == 'd' && pos->right && pos->right->getcan_pass()) pos = pos->right;
      else if(in == 'a' && pos->left && pos->left->getcan_pass()) pos = pos->left;
      else if(in == 's' && pos->down && pos->down->getcan_pass()) pos = pos->down;
      else if(in == 'w' && pos->up && pos->up->getcan_pass()) pos = pos->up;
      if(temp != pos)
      {
        temp->render(0); pos->render(1);
        vision(pos);
      }
    }


};
