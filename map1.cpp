#include <iostream>
#include <string>
#include <curses.h>
#include <dict>
#include <random>
#include <vector>
#include <fstream>


#define LIGHT_GREEN 8
#define LIGHT_BLUE 9
#define COLOR_BROWN 10
#define COLOR_TAN 11
#define COLOR_ORANGE 12
#define YELLOW 13
#define GREEN1 14
#define GREEN2 15
#define GREEN3 16
#define GREEN4 17
#define GREEN5 18


std::ofstream outfile;
struct Coord
{
  int x,y;
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
    int r()
    {
      prev += rand() % 137;
      return (rand() + prev);
    }
    bool drop(int prob, int precision)
    {
      srand( time(NULL) + prev);
      prev += rand() % 137;
      return (rand() % precision < prob)? true:false;
    }
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
    short background;
    short foreground;
    bool is_water;
    bool is_terrain;
    std::string tag;
    short green[5];
    Dice* d;
  public:
    Tile(int x, int y)
    {
      location.x = x;
      location.y = y;
      d = new Dice();
      visited = false;
      can_pass = true;
      blink = false;
      symbol = '/';
      is_water = false;
      is_terrain = false;
      green[0] = GREEN1;
      green[1] = GREEN2;
      green[2] = GREEN3;
      green[3] = GREEN4;
      green[4] = GREEN5;
      init_color(LIGHT_GREEN, 700, 850, 149);
      init_color(LIGHT_BLUE, 359, 846, 1000);
      init_color(COLOR_BROWN, 121*3.9, 80*3.9, 54*3.9);
      init_color(COLOR_TAN, 220*3.9, 200*3.9, 160*3.9);
      init_color(COLOR_ORANGE, 255*3.9, 150*3.9, 0);
      init_color(YELLOW, 255*3.9, 255*3.9, 30*3.9);
      init_color(GREEN1, 190*3.9, 200*3.9, 110*3.9);
      init_color(GREEN2, 190*3.9, 200*3.9, 170*3.9);
      init_color(GREEN3, 90*3.9, 110*3.9, 40*3.9);
      init_color(GREEN4, 40*3.9, 80*3.9, 15*3.9);
      init_color(GREEN5, 60*3.9, 150*3.9, 40*3.9);
      foreground = GREEN2;
      background = green[d->r() % 5];
    }
    ~Tile() {}
    /* need cpy ctor */
    Coord* getpos() { return &location; }
    void setpos(int x, int y) { location.x = x; location.y = y; }
    void setpos(const Coord &obj) { location = obj; }
    void setsymbol(char c) { symbol = c; }
    char getsymbol() { return symbol; }
    void setblink(bool b) { blink = b; }
    bool getblink() { return blink; }
    void setcan_pass(bool can) { can_pass = can; }
    bool getcan_pass() { return can_pass; }
    bool get_terrain() { return is_terrain; }
    void set_terrain(bool t) { is_terrain = t; }
    void set_tag(std::string t) { tag = t; }
    void set_tag(const char* t) { tag = t; }
    void set_back(short c) { background = c; }
    void set_fore(short c) { foreground = c; }
    short get_back() { return background; }
    short get_fore() { return foreground; }
    const char * get_tag() { return tag.c_str(); }
    std::string &get_tags() { return tag; }
};


class Map
{
  private:
    Tile* World[51][51];
    Coord currentpos;
    double mountains[51][51];
    double sand[51][51];
    short colors[30][30];
    double water[51][51];
    int colorcount;
  public:
    Map()
    {
      for (int i = 0; i < 51; i++)
      {
        for(int k = 0; k < 51; k++)
        {
          water[i][k] = 0;
          sand[i][k] = 0;
          colors[i][k] = 0;
          mountains[i][k] = 0;
        }
      }
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
      srand( time(NULL) );
      outfile << "breakpoint\n";
      colorcount = 10;
      for (int x = 0; x < 51; x++)
      {
        for (int y = 0; y < 51; y++) World[x][y] = new Tile(x,y);
      }
      outfile << "breakpointhere\n";
      currentpos.x = rand() % 30 + 10; currentpos.y = rand() % 30 + 10;
      render(true, *World[currentpos.x][currentpos.y]);
      for (int i = 0; i < 3; i++)
      terrain("water", '~', LIGHT_BLUE, COLOR_BLUE, 1.5);
      for (int i = 0; i < 2; i++)
      terrain("mountain", '^', COLOR_BLACK, COLOR_BROWN, .6);
      for (int i = 0; i < 2; i++)
      terrain("sand", '*', COLOR_BROWN, COLOR_TAN, .8);
      for (int i = 0; i < 3; i ++)
      blur(water, "water", LIGHT_BLUE, COLOR_BLUE);
      for (int i = 0; i < 20; i++)
      {
      blur(mountains, "mountain", COLOR_BLACK, COLOR_BROWN);
      blur(sand, "sand", COLOR_BROWN, COLOR_TAN);
      }

      /* NEED RIVER FUNCTION or option for terrain */
      //generates terrain variation e.g. lakes, dirt, mountains
      outfile << "breakpoint 1\n";
    }

    ~Map()
    {
      endwin(); exit(1); outfile.close();
    }

    void render(bool here, Tile &t)
    {
      std::string p = t.get_tag();
      // if(p == "mountain")
      // {
      //   t.set_fore(COLOR_BLACK); //change
      //   t.set_back(COLOR_BROWN);
      // }
      // if(p == "water")
      // {
      //   t.set_fore(LIGHT_BLUE);
      //   t.set_back(COLOR_BLUE);
      // }
      // if(p == "sand")
      // {
      //   outfile << "sand true ";
      //   t.set_fore(COLOR_BROWN);
      //   t.set_back(COLOR_TAN);
      // }
      char temp = t.getsymbol();
      short f = t.get_fore();
      short tempcolor;
      if (here)
      {
        if (t.get_back() == COLOR_BROWN) f = COLOR_ORANGE;
        else if (t.get_back() == COLOR_BLUE) f = YELLOW;
        else if (t.get_back() == COLOR_TAN) f = COLOR_RED;
        else f = COLOR_BROWN;
        if (!colors[f][t.get_back()])
        {
          colors[f][t.get_back()] = colorcount;
          init_pair(colorcount, f, t.get_back());
          colorcount++;
        }
        temp = 'X';
        t.setblink(true);
      }
      else if (!colors[f][t.get_back()])
      {
        colors[f][t.get_back()] = colorcount;
        init_pair(colorcount, f, t.get_back());
        colorcount++;
      }
      tempcolor = colors[f][t.get_back()];
      //if(blink) attron(A_BLINK);
      attron(COLOR_PAIR(tempcolor));
      mvaddch(t.getpos()->y, t.getpos()->x * 2, temp);
      mvaddch(t.getpos()->y, t.getpos()->x * 2 +1, ' ');
      if(!here) mvaddch(t.getpos()->y, t.getpos()->x *2 + 1, ' ');
      attroff(A_BLINK);
      attroff(COLOR_PAIR(tempcolor));
      t.setblink(false);
    }

    void gen(std::string tag, Coord &loc, int count, short &stop, char texture,
      short colorfore, short colorback, bool &failbit, float step)
    {
      Coord temp = loc;
      short ct;
      double prob;
      failbit = false;
      if (temp.x < 1 || temp.x > 49 || temp.y < 1 || temp.y > 49)
      {
        failbit = true; return; //prevent segfault
      }
      retry:
      for(ct = 0; ct < 10; ct++)
      {
        int dir = rand() % 4;
        if(dir == 3 && !World[temp.x+1][temp.y]->get_terrain()) { temp.x++; break; }
        if(dir == 2 && !World[temp.x][temp.y+1]->get_terrain()) { temp.y++; break; }
        if(dir == 1 && !World[temp.x-1][temp.y]->get_terrain()) { temp.x--; break; }
        if(dir == 0 && !World[temp.x][temp.y-1]->get_terrain()) { temp.y--; break; }
      }

      if (temp.x < 1 || temp.x > 49 || temp.y < 1 || temp.y > 49)
      {
        temp = loc;
        goto retry;
        //gen(tag, loc, count, stop, texture, colorfore, colorback, failbit); //change directions and prevent segfault
      }
      // failed to find undeclared neighbor
      if (ct == 9)
      {
        failbit = true;
        prob = 100;
        goto here;
      }

      prob = 50; //out of 100
      if (tag.compare("mountain") == 0 || tag.compare("sand") == 0) prob = 70;
      /* checks if neighboring tiles are of the same type, increasing probability
      of rendering new tile for each of these */
      if(World[temp.x+1][temp.y]->get_tags().compare(tag) == 0)
      prob *= 1.19;
      if(World[temp.x-1][temp.y]->get_tags().compare(tag) == 0)
      prob *= 1.19;
      if(World[temp.x][temp.y+1]->get_tags().compare(tag) == 0)
      prob *= 1.19;
      if(World[temp.x][temp.y-1]->get_tags().compare(tag) == 0)
      prob *= 1.19;
      prob *= (100 - step * count);
      here:
      if ( rand() % 100 < prob )
      {
        World[temp.x][temp.y]->set_terrain(1);
        World[temp.x][temp.y]->set_tag(tag);
        World[temp.x][temp.y]->set_fore(colorfore);
        World[temp.x][temp.y]->set_back(colorback);
        World[temp.x][temp.y]->setsymbol(texture);
        loc = temp;
      }
      else
      {
        stop++;
      }
      if(tag.compare("mountain") == 0) mountains[temp.x][temp.y] = 1;
      else if (tag.compare("water") == 0)
      {
        water[temp.x][temp.y] = 1;
        //outfile << "it happened: " << water[temp.x][temp.y] << "\n";
      }
      else if (tag.compare("sand") == 0) sand[temp.x][temp.y] = 1;
      // if(World[temp.x+1][temp.y]->get_tags().compare(World[temp.x][temp.y]->get_tags()) == 0)
      // if(World[temp.x-1][temp.y]->get_tags().compare(World[temp.x][temp.y]->get_tags()) == 0)
      // if(World[temp.x][temp.y+1]->get_tags().compare(World[temp.x][temp.y]->get_tags()) == 0)
      // if(World[temp.x][temp.y-1]->get_tags().compare(World[temp.x][temp.y]->get_tags()) == 0)
    }
    void getwater()
    {
      for (int i = 0; i < 51; i++)
      {
        for (int k = 0; k < 51; k++) outfile << water[i][k] << " ";
      }
    }

    void terrain(const char* name, char texture, short colorfore, short colorback, float step)
    {
      Coord temp;
      std::string t = name;
      temp.x = rand() % 49; temp.y = rand() % 49;
      int count = 0; bool failbit;
      for(short stop = 0; stop < 4; count++)
      {
        gen(t, temp, count, stop, texture, colorfore, colorback, failbit, step);
        if(failbit)
        {
          temp.x = rand() % 49;
          temp.y = rand() % 49;
          count--;
        }
      }
    }

    void blur(double (&type)[51][51], const char* tag, short fore, short back)
    {
      srand( time(NULL) );
      double sum;
      std::string t = tag;
      char temp;
      if (t.compare("water") == 0) temp = '~';
      else if (t == "mountain") temp = '^';
      else if (t.compare("sand") == 0) temp = '*';
      for(int y = 1; y < 49; y ++)
      {
        for(int x = 1; x < 49; x ++)
        {
          sum = 0;
          sum += type[x][y]*.25;
          sum += type[x-1][y]*.125;
          sum += type[x+1][y]*.125;
          sum += type[x][y-1]*.125;
          sum += type[x][y+1]*.125;
          sum += type[x-1][y-1]*.0625;
          sum += type[x+1][y-1]*.0625;
          sum += type[x-1][y+1]*.0625;
          sum += type[x+1][y+1]*.0625;
          type[x][y] = sum;
          if (sum > .3)
          {
            //outfile << "blur change\n";
            if (rand() % 100 < 30)
            {
              bool randx = rand() % 2;
              bool randy = rand() % 2;
              World[x+randx][y+randy]->set_tag(tag);
              //outfile << "HERE!!!! " << World[x+randx][y+randy]->get_tags() << "\n";
              World[x+randx][y+randy]->set_terrain(1);
              World[x+randx][y+randy]->setsymbol(temp);
              World[x+randx][y+randy]->set_fore(fore);
              World[x+randx][y+randy]->set_back(back);
            }
              World[x][y]->set_tag(tag);
              World[x][y]->set_terrain(1);
              World[x][y]->setsymbol(temp);
              World[x][y]->set_fore(fore);
              World[x][y]->set_back(back);
              //type[x][y] = sum;
          }
          else
          {
            //outfile << "blur removal\n";
            World[x][y]->set_terrain(0);
            World[x][y]->set_tag("none");
            //type[x][y] = sum;
          }
        }
      }
    }
    void vision(Coord loc)
    {
      int startx = loc.x - 2; int starty = loc.y;
      int ct = 0; int limit = 1; int topy = loc.y;
      for (int x = startx; x < startx + 5; ct++, x++)
      {
        for ( int y = starty; y < limit + topy; y++)
        if (0 <= x && x < 51 && 0 <= y && y < 51) render(false, *World[x][y]);
        // make outer most layer more dim than the rest? would look really cool...
        (ct < 2)? limit+=1:limit-=1;
        (ct < 2)? starty-=1:starty+=1;
      }
    }
    int getx() { return currentpos.x; }
    int gety() { return currentpos.y; }
    void move(char in)
    {
      Coord temp = currentpos;
      //std::cout << pos->getpos()->x << " " << pos->getpos()->y << "\n";
      if( in == 'a' && currentpos.x > 0 ) currentpos.x -= 1;
      else if ( in == 'd' && currentpos.x < 50 ) currentpos.x += 1;
      else if ( in == 'w' && currentpos.y > 0 ) currentpos.y -= 1;
      else if ( in == 's' && currentpos.y < 50 ) currentpos.y += 1;
      if(temp.x != currentpos.x || temp.y != currentpos.y)
      {
        vision(currentpos);
        render(false, *World[temp.x][temp.y]);
        render(true, *World[currentpos.x][currentpos.y]);
      }
    }
    void debug()
    {
      outfile << "DEBUG\n";
      for(int i = 0; i < 51; i++)
      {
        for(int k = 0; k < 51; k++) render(false, *World[i][k]);
      }
    }


};
