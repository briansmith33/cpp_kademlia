#pragma once
#include <vector>
#include "peer.hpp"

struct Stack {
    Stack();
    std::vector<Peer> items;
    void push(Peer peer);
    Peer pop();
    bool isEmpty();
    Peer peek();
};
