#include "Node.h"

static size_t current_hash = 0;

Node::Node() {
    parent = nullptr;
}

Node::Node(Point position) {
    this->position = position;
    this->hash = current_hash++;
    parent = nullptr;
}

Node::~Node() {
    current_hash--;
}

void Node::addChild(Node *node) {
    children[node->hash] = node;
    node->parent = this;
}

void Node::removeFromParent() {
    if (parent) {
        parent->children.erase(parent->children.find(this->hash));
        parent = nullptr;
    }
}