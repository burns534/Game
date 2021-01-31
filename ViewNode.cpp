#include "ViewNode.h"

ViewNode::ViewNode() {}

ViewNode::ViewNode(Point position, std::string foreground_color, std::string background_color) {
    this->foreground_color = foreground_color;
    this->background_color = background_color;
    this->position = position;
}