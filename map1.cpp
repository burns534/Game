#include <iostream>
#include <string>
#include <ncurses.h>
#include <dict>
#include <random>
#include <vector>
#include <fstream>
#include <math.h>

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
#define DEFAULT 255


std::ofstream outfile;
struct Coord
{
  int x,y;
};

struct RGB
{
  short r,g,b;
  RGB(short q, short w, short e)
  {
    r = q; g = w; b = e;
  }
  void set(short q, short w, short e) { r = q; g = w; b = e; }
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
    short R;
    short G;
    short B;
    short pairid;
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
      foreground = GREEN3;
      background = green[d->r() % 5];
      short pairid = 0;

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
    void set_r(short r) { R = r; }
    void set_g(short g) { G = g; }
    void set_b(short b) { B = b; }
    short get_r() { return R; }
    short get_g() { return G; }
    short get_b() { return B; }
    short get_pairid() { return pairid; }
    void set_pairid(short k) { pairid = k; }
    const char * get_tag() { return tag.c_str(); }
    std::string &get_tags() { return tag; }
};

template< int ex , int why >
class Map
{
  private:
    Tile* World[ex + 2][why + 2];
    Coord currentpos;
    double mountains[ex + 2][why + 2];
    double sand[ex + 2][why + 2];
    double water[ex + 2][why + 2];
    short colors[256][256];
    short R[ex + 2][why + 2];
    short G[ex + 2][why + 2];
    short B[ex + 2][why + 2];
    int colorcount; //counts how many new colors initialized, for indexing
    int paircount; //counts how many pairs initialized, for indexing
    std::vector< std::vector< std::vector< short> > > lookup; //r,g,b key gives colorpair value
    Dict<RGB*> indexing; //native color key gives pointer to rgb struct
  public:
    Map()
    {
      for (int k = 0; k < why + 2; k++)
      {
        for(int i = 0; i < ex + 2; i++)
        {
          water[i][k] = 0;
          sand[i][k] = 0;
          colors[i][k] = 0;
          mountains[i][k] = 0;
        }
      }
      lookup = std::vector< std::vector< std::vector<short> > >
      (256, std::vector< std::vector<short> >(256, std::vector<short>(256)));
      for (int i = 0; i < 256; i++)
      {
        for (int k = 0; k < 256; k++)
        {
          for (int j = 0; j < 256; j++) lookup[i][k].push_back(0);
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
      init_pair(DEFAULT, COLOR_BLACK, COLOR_BLACK);
      wbkgd(stdscr, COLOR_PAIR(DEFAULT));
      init_color(LIGHT_GREEN, 700, 850, 149);
      init_color(LIGHT_BLUE, 50*3.9, 160*3.9, 255*3.9);
      init_color(COLOR_BROWN, 121*3.9, 80*3.9, 54*3.9);
      init_color(COLOR_TAN, 220*3.9, 200*3.9, 160*3.9);
      init_color(COLOR_ORANGE, 255*3.9, 150*3.9, 0);
      init_color(YELLOW, 255*3.9, 255*3.9, 30*3.9);
      init_color(GREEN1, 190*3.9, 200*3.9, 110*3.9);
      init_color(GREEN2, 190*3.9, 200*3.9, 170*3.9);
      init_color(GREEN3, 90*3.9, 110*3.9, 40*3.9);
      init_color(GREEN4, 40*3.9, 80*3.9, 15*3.9);
      init_color(GREEN5, 60*3.9, 150*3.9, 40*3.9);
      init_color(6, 0, 0, 0);

      indexing[COLOR_GREEN] = new RGB(0, 255, 0);
      indexing[6] = new RGB(0, 0, 0);
      indexing[COLOR_BLUE] = new RGB(0, 83, 233);
      indexing[LIGHT_GREEN] = new RGB(180, 223, 38);
      indexing[LIGHT_BLUE] = new RGB(50, 160, 255);
      indexing[COLOR_BROWN] = new RGB(121, 80, 54);
      indexing[COLOR_TAN] = new RGB(220, 200, 160);
      indexing[COLOR_ORANGE] = new RGB(255, 150, 0);
      indexing[YELLOW] = new RGB(255, 255, 30);
      indexing[GREEN1] = new RGB(190, 200, 110);
      indexing[GREEN2] = new RGB(190, 200, 170);
      indexing[GREEN3] = new RGB(90, 110, 40);
      indexing[GREEN4] = new RGB(40, 80, 15);
      indexing[GREEN5] = new RGB(60, 150, 40);
      srand( time(NULL) );
      outfile << "breakpoint\n";
      colorcount = 20;
      paircount = 20;
      for (int y = 0; y < why + 2 ; y++)
      {
        for (int x = 0; x < ex + 2; x++) World[x][y] = new Tile(x,y);
      }
      for (int y = 0; y < why + 2 ; y++)
      {
        for (int x = 0; x < ex + 2; x++)
        {
          initialize(*(World[x][y])); //adds rgb values to each tile
        }
      }
      outfile << "breakpointhere\n";

      //gaussian blur
      blur(R, 'r');
      blur(G, 'g');
      blur(B, 'b');
      blur(R, 'r');
      blur(B, 'b'); //blurring green looks bad

      //generate terrain
      for (int i = 0; i < 3; i++)
      terrain("water", '~', LIGHT_BLUE, COLOR_BLUE, 1.5);
      for (int i = 0; i < 2; i++)
      terrain("mountain", '#', 6, COLOR_BROWN, .5);
      for (int i = 0; i < 2; i++)
      terrain("sand", '*', COLOR_BROWN, COLOR_TAN, .8);

      //smooth edges, fill in holes
      for (int i = 0; i < 20; i++)
      {
      blur(mountains, "mountain", COLOR_BLACK, COLOR_BROWN);
      blur(sand, "sand", COLOR_BROWN, COLOR_TAN);
      }
      for (int i = 0; i < 3; i ++)
      blur(water, "water", LIGHT_BLUE, COLOR_BLUE);
      currentpos.x = rand() % (ex - 20) + 10; currentpos.y = rand() % (why-20) + 10;
      vision(*World[currentpos.x][currentpos.y]->getpos(), 6);

      /* NEED RIVER FUNCTION or option for terrain */

    }

    ~Map()
    {
      endwin(); exit(1); outfile.close();
      delete indexing[LIGHT_GREEN];
      delete indexing[LIGHT_BLUE];
      delete indexing[COLOR_BROWN];
      delete indexing[COLOR_TAN];
      delete indexing[COLOR_ORANGE];
      delete indexing[YELLOW];
      delete indexing[GREEN1];
      delete indexing[GREEN2];
      delete indexing[GREEN3];
      delete indexing[GREEN4];
      delete indexing[GREEN5];
    }

    void initialize(Tile &t)
    {
      short c = t.get_back();
      //define rgb values for each tile
      t.set_r(indexing[c]->r);
      t.set_g(indexing[c]->g);
      t.set_b(indexing[c]->b);
      //add values to r g and b arrays
      int posx = t.getpos()->x;
      int posy = t.getpos()->y;
      R[posx][posy] = t.get_r();
      //outfile << "r: " << t.get_r() << "\n";
      G[posx][posy] = t.get_g();
      B[posx][posy] = t.get_b();
    }

    void render( bool here, Tile &t, double f )
    {
      if(paircount == 128) paircount = 5; //reset paircount
      if(colorcount > 254) colorcount = 19;
      char temp = t.getsymbol();
      outfile << "before, after: " << t.get_r() << ", " << t.get_r() * f << "\n";
      float r = t.get_r() * f;
      float g = t.get_g() * f;
      float b = t.get_b() * f;
      short fore = t.get_fore(); // tile should know its symbol color
      // going to add noise to foreground color
      short back = colorcount;
      short pair;
      init_color(back, r*3.9, g*3.9, b*3.9); //initialize background color
      colorcount++;
      if(here)
      {
        float red = r + 2*(128-r); //makes it a little redder so it stands out
        float green = 255 * (g + 2*(128-g) - 50)/255;
        float blue = 255 * (b + 2*(128-b) - 50)/255;
        fore = colorcount;
        init_color(fore, red*3.9, green*3.9, blue*3.9); //initialize cursor color
        colorcount++;
        temp = 'X';
      }
      pair = paircount;
      paircount++;
      init_pair(pair, fore, back); //initialize pair
      attron(COLOR_PAIR(pair));
      mvaddch(t.getpos()->y, t.getpos()->x * 2, temp);
      mvaddch(t.getpos()->y, t.getpos()->x * 2 + 1, ' ');
      attroff(COLOR_PAIR(pair));
    }

    //gaussian blur to short matrix
    void blur(short (&type)[ex+2][why+2], char t)
    {
      srand( time(NULL) );
      double sum;
      for(int y = 0; y < why; y ++)
      {
        for(int x = 0; x < ex; x ++)
        {
          sum = 0;
          sum += type[x][y]*.25;
          if ( y - 1 >= 0) sum += type[x][y-1]*.125;
          else if (t == 'r') sum += 114*.125; //part of y = 0 edge;
          else if (t == 'g') sum += 128*.125;
          else sum += 56 * .125;
          if ( y + 1 <= why + 1) sum += type[x][y+1]*.125;
          if (x - 1 >=0)
          {
            sum += type[x-1][y]*.125;
            if (y - 1 >= 0) sum += type[x-1][y-1]*.0625;
            if (y + 1 <= why + 1) sum += type[x-1][y+1]*.0625;
          }
          // for the x = 0 edge with top left corner
          else if (t == 'r') sum += 114*.125;
          else if (t == 'g') sum += 128*.25;
          else sum += 56 * .125;
          if(x + 1 <= ex + 1)
          {
            if (y - 1 >= 0) sum += type[x+1][y-1]*.0625; // last y = 0
            else if (t == 'r') sum += 114 * .0625;
            else if (t == 'g') sum += 128 * .0625;
            else sum += 56 * .0625;
            sum += type[x+1][y]*.125;
            if (y + 1 <= ex + 1) sum += type[x+1][y+1]*.0625;
          }
          type[x][y] = (int)sum;
          //outfile << (int)sum << " ";
          if (t == 'r') World[x][y]->set_r((int)sum);
          else if (t == 'g') World[x][y]->set_g((int)sum);
          else if (t == 'b') World[x][y]->set_b((int)sum);
        }
      }
    }

    void gen(std::string tag, Coord &loc, int count, short &stop, char texture,
      short fore, short back, bool &failbit, float step)
    {
      Coord temp = loc;
      short ct;
      double prob;
      failbit = false;
      bool w = false;
      if(tag.compare("mountain") == 0) mountains[temp.x][temp.y] = 1;
      else if (tag.compare("water") == 0)
      {
        water[temp.x][temp.y] = 1;
        w = true;
      }
      else if (tag.compare("sand") == 0) sand[temp.x][temp.y] = 1;
      if (temp.x < 1 || temp.x > ex-2 || temp.y < 1 || temp.y > why-2)
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

      if (temp.x < 1 || temp.x > ex-2 || temp.y < 1 || temp.y > why-2)
      {
        temp = loc;
        goto retry; //change directions because previous step failed
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
        World[temp.x][temp.y]->set_fore(fore);
        //World[temp.x][temp.y]->set_back(back);
        World[temp.x][temp.y]->set_r(noise(false, indexing[back]->r, 20));
        World[temp.x][temp.y]->set_g(noise(w, indexing[back]->g, 10));
        World[temp.x][temp.y]->set_b(noise(w, indexing[back]->b, 20));
        //initialize(*World[temp.x][temp.y]);
        World[temp.x][temp.y]->setsymbol(texture);
        loc = temp;
      }
      else
      {
        stop++; //closer to termination
      }
    }
    void getwater()
    {
      for (int i = 0; i < why; i++)
      {
        for (int k = 0; k < ex; k++) outfile << water[k][i] << " ";
      }
    }

    void terrain(const char* name, char texture, short fore, short back, float step)
    {
      Coord temp;
      std::string t = name;
      temp.x = rand() % (ex-2); temp.y = rand() % (why-2);
      int count = 0; bool failbit;
      for(short stop = 0; stop < 4; count++)
      {
        gen(t, temp, count, stop, texture, fore, back, failbit, step);
        if(failbit)
        {
          temp.x = rand() % (ex-2);
          temp.y = rand() % (why-2);
          count--;
        }
      }
    }
    short noise(bool water, short value, unsigned amount)
    {
      if(water) amount *= 4;
      short temp = value + (rand()%3 - 1) * (rand() % amount);
      (temp > 255) ? temp -= 2*(temp - value): 0;
      (temp < 0) ? temp -= 2*(temp - value): 0;
      return temp;
    }

    void blur(double (&type)[ex+2][why+2], const char* tag, short fore, short back)
    {
      srand( time(NULL) );
      double sum;
      std::string t = tag;
      char temp;
      bool w = false;
      if (t.compare("water") == 0)
      {
        temp = '~';
        w = true;
      }
      else if (t == "mountain") temp = '#';
      else if (t.compare("sand") == 0) temp = '*';
      for(int y = 1; y < why-2; y ++)
      {
        for(int x = 1; x < ex-2; x ++)
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
              World[x+randx][y+randy]->set_r(noise(false, indexing[back]->r, 20));
              World[x+randx][y+randy]->set_g(noise(w, indexing[back]->g, 10));
              World[x+randx][y+randy]->set_b(noise(w, indexing[back]->b, 20));
              //World[x+randx][y+randy]->set_back(back);
              //initialize(*World[x+randx][y+randy]);
            }
              World[x][y]->set_tag(tag);
              World[x][y]->set_terrain(1);
              World[x][y]->setsymbol(temp);
              World[x][y]->set_fore(fore);
              World[x][y]->set_r(noise(false, indexing[back]->r, 20));
              World[x][y]->set_g(noise(w, indexing[back]->g, 10));
              World[x][y]->set_b(noise(w, indexing[back]->b, 20));
              //World[x][y]->set_back(back);
              //initialize(*World[x][y]);
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

    void vision(Coord loc, short radius)
    {
      short x = loc.x - radius; short y = loc.y;
      register int start;
      register int stop;
      for (; x <= loc.x + radius; x++)
      {
        start = -(int)sqrt(radius*radius - (loc.x-x)*(loc.x-x));
        stop = (int)sqrt(radius*radius - (loc.x-x)*(loc.x-x));
        // outfile << "radius, x: " << radius << ", " << loc.x-x<< "\n";
        // outfile << "start,stop: " << start << ", " << stop << "\n";
        for (y = start + loc.y; y < stop + loc.y+1; y ++)
        {
          if(x < ex && y < why && x >= 0 && y >= 0)
          {
            float factor = (1 - .5*(pow((loc.x - x)*(loc.x - x) + (loc.y - y)*(loc.y - y),.6) + .5)/radius);
            if (x == loc.x && y == loc.y) render(true, *World[x][y], factor);
            else render(false, *World[x][y], factor);
          }
        }
      }
      if(loc.x + 1 < ex && loc.y + 6 < why)
      render(false, *World[loc.x+1][loc.y + 6], .2);
      if(loc.x - 1 >= 0 && loc.y + 6 < why)
      render(false, *World[loc.x-1][loc.y + 6], .2);
      if(loc.x + 1 < ex && loc.y - 6 >= 0)
      render(false, *World[loc.x+1][loc.y - 6], .2);
      if(loc.x - 1 >= 0 && loc.y - 6 >= 0)
      render(false, *World[loc.x-1][loc.y - 6], .2);
      if(loc.x - 6 >= 0 && loc.y - 1 >= 0)
      render(false, *World[loc.x - 6][loc.y - 1], .2);
      if(loc.x - 6 >= 0 && loc.y + 1 < why)
      render(false, *World[loc.x - 6][loc.y + 1], .2);
      if(loc.x + 6 < ex && loc.y - 1 >= 0)
      render(false, *World[loc.x + 6][loc.y - 1], .2);
      if(loc.x + 6 < ex && loc.y + 1 < why)
      render(false, *World[loc.x + 6][loc.y + 1], .2);
    }
    int getx() { return currentpos.x; }
    int gety() { return currentpos.y; }
    void move(char in)
    {
      Coord temp = currentpos;
      short x, y;
      bool lr;
      //std::cout << pos->getpos()->x << " " << pos->getpos()->y << "\n";
      if( in == 'a' && currentpos.x > 0 )
      {
        currentpos.x -= 1;
        y = 1; x = -1;
        lr = 1;
      }
      else if ( in == 'd' && currentpos.x < ex - 1 )
      {
        currentpos.x += 1;
        y = 1; x = 1;
        lr = 1;
      }
      else if ( in == 'w' && currentpos.y > 0 )
      {
        currentpos.y -= 1;
        y = -1; x = 1;
        lr = 0;
      }
      else if ( in == 's' && currentpos.y < why - 1 )
      {
        currentpos.y += 1;
        y = 1; x = 1;
        lr = 0;
      }
      // if we moved
      if(temp.x != currentpos.x || temp.y != currentpos.y)
      {
        // possible segmentation fault issue
        if(lr)
        {
          mvaddch(temp.y, (temp.x - 6 * x) * 2 + 1, ' ');
          mvaddch(temp.y, (temp.x - 6 * x) * 2, ' ');
          mvaddch(temp.y - y, (temp.x - 6 * x) * 2 + 1, ' ');
          mvaddch(temp.y - y, (temp.x - 6 * x) * 2, ' ');
          mvaddch(temp.y + y, (temp.x - 6 * x) * 2 + 1, ' ');
          mvaddch(temp.y + y, (temp.x - 6 * x) * 2, ' ');
          mvaddch(temp.y - 2 * y, (temp.x - 5 * x) * 2 + 1, ' ');
          mvaddch(temp.y - 2 * y, (temp.x - 5 * x) * 2, ' ');
          mvaddch(temp.y + 2 * y, (temp.x - 5 * x) * 2 + 1, ' ');
          mvaddch(temp.y + 2 * y, (temp.x - 5 * x) * 2, ' ');
          mvaddch(temp.y - 3 * y, (temp.x - 5 * x) * 2 + 1, ' ');
          mvaddch(temp.y - 3 * y, (temp.x - 5 * x) * 2, ' ');
          mvaddch(temp.y + 3 * y, (temp.x - 5 * x) * 2 + 1, ' ');
          mvaddch(temp.y + 3 * y, (temp.x - 5 * x) * 2, ' ');
          mvaddch(temp.y - 4 * y, (temp.x - 4 * x) * 2 + 1, ' ');
          mvaddch(temp.y - 4 * y, (temp.x - 4 * x) * 2, ' ');
          mvaddch(temp.y + 4 * y, (temp.x - 4 * x) * 2 + 1, ' ');
          mvaddch(temp.y + 4 * y, (temp.x - 4 * x) * 2, ' ');
          mvaddch(temp.y - 5 * y, (temp.x - 3 * x) * 2 + 1, ' ');
          mvaddch(temp.y - 5 * y, (temp.x - 3 * x) * 2, ' ');
          mvaddch(temp.y + 5 * y, (temp.x - 3 * x) * 2 + 1, ' ');
          mvaddch(temp.y + 5 * y, (temp.x - 3 * x) * 2, ' ');
          mvaddch(temp.y - 6 * y, (temp.x - 1 * x) * 2 + 1, ' ');
          mvaddch(temp.y - 6 * y, (temp.x - 1 * x) * 2, ' ');
          mvaddch(temp.y + 6 * y, (temp.x - 1 * x) * 2 + 1, ' ');
          mvaddch(temp.y + 6 * y, (temp.x - 1 * x) * 2, ' ');
          mvaddch(temp.y - 7 * y, temp.x * 2 + 1, ' ');
          mvaddch(temp.y - 7 * y, temp.x * 2, ' ');
          mvaddch(temp.y + 7 * y, temp.x * 2 + 1, ' ');
          mvaddch(temp.y + 7 * y, temp.x * 2, ' ');
        }
        else
        {
          mvaddch(temp.y, (temp.x - 7 * x) * 2 + 1, ' ');
          mvaddch(temp.y, (temp.x - 7 * x) * 2, ' ');
          mvaddch(temp.y, (temp.x + 7 * x) * 2 + 1, ' ');
          mvaddch(temp.y, (temp.x + 7 * x) * 2, ' ');
          mvaddch(temp.y - 1 * y, (temp.x - 6 * x) * 2 + 1, ' ');
          mvaddch(temp.y - 1 * y, (temp.x - 6 * x) * 2, ' ');
          mvaddch(temp.y - 1 * y, (temp.x + 6 * x) * 2 + 1, ' ');
          mvaddch(temp.y - 1 * y, (temp.x + 6 * x) * 2, ' ');
          mvaddch(temp.y - 3 * y, (temp.x - 5 * x) * 2 + 1, ' ');
          mvaddch(temp.y - 3 * y, (temp.x - 5 * x) * 2, ' ');
          mvaddch(temp.y - 3 * y, (temp.x + 5 * x) * 2 + 1, ' ');
          mvaddch(temp.y - 3 * y, (temp.x + 5 * x) * 2, ' ');
          mvaddch(temp.y - 4 * y, (temp.x - 4 * x) * 2 + 1, ' ');
          mvaddch(temp.y - 4 * y, (temp.x - 4 * x) * 2, ' ');
          mvaddch(temp.y - 4 * y, (temp.x + 4 * x) * 2 + 1, ' ');
          mvaddch(temp.y - 4 * y, (temp.x + 4 * x) * 2, ' ');
          mvaddch(temp.y - 5 * y, (temp.x - 3 * x) * 2 + 1, ' ');
          mvaddch(temp.y - 5 * y, (temp.x - 3 * x) * 2, ' ');
          mvaddch(temp.y - 5 * y, (temp.x + 3 * x) * 2 + 1, ' ');
          mvaddch(temp.y - 5 * y, (temp.x + 3 * x) * 2, ' ');
          mvaddch(temp.y - 5 * y, (temp.x - 2 * x) * 2 + 1, ' ');
          mvaddch(temp.y - 5 * y, (temp.x - 2 * x) * 2, ' ');
          mvaddch(temp.y - 5 * y, (temp.x + 2 * x) * 2 + 1, ' ');
          mvaddch(temp.y - 5 * y, (temp.x + 2 * x) * 2, ' ');
          mvaddch(temp.y - 6 * y, (temp.x - 1 * x) * 2 + 1, ' ');
          mvaddch(temp.y - 6 * y, (temp.x - 1 * x) * 2, ' ');
          mvaddch(temp.y - 6 * y, (temp.x + 1 * x) * 2 + 1, ' ');
          mvaddch(temp.y - 6 * y, (temp.x + 1 * x) * 2, ' ');
          mvaddch(temp.y - 6 * y, temp.x * 2 + 1, ' ');
          mvaddch(temp.y - 6 * y, temp.x * 2, ' ');
        }
        vision(currentpos, 6);
        // render(false, *World[temp.x][temp.y], 1);
        // render(true, *World[currentpos.x][currentpos.y], 1);
      }
    }
    void debug()
    {
      outfile << "DEBUG\n";
      for(int i = 0; i < why; i++)
      {
        for(int k = 0; k < ex; k++) render(false, *World[k][i], 1);
      }
    }

};
