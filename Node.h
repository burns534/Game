#ifndef Node_h
#define Node_h

#include "Point.h"
#include <string>
#include <map>

class Node {
public:
    size_t hash;
    Node *parent;
    std::map<size_t, Node *> children; // speeds up removal
    Point position;
    Node();
    Node(Point position);
    ~Node();
    void addChild(Node *node);
    void removeFromParent();
};

#endif