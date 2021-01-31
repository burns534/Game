#ifndef ViewNode_h
#define ViewNode_h
#include "Node.h"

class ViewNode: public Node {
public: 
    std::string foreground_color, background_color;
    ViewNode();
    ViewNode(Point position, std::string foreground_color, std::string background_color);
};

#endif
