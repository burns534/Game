#include "ViewNode.h"

ViewNode::ViewNode() {}

ViewNode::ViewNode(Point position, std::string color, double zPosition) {
    this->color = color;
    this->position = position;
    this->zPosition = zPosition;
}
