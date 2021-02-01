#ifndef ViewNode_h
#define ViewNode_h
#include "Node.h"

struct ViewNode: public Node {
    std::string color;
    double zPosition;
    ViewNode();
    ViewNode(Point position, std::string color, double zPosition);
};

#endif
