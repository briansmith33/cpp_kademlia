#pragma once
#include <sstream>
#include <iostream>

struct Peer {
    long long int id;
    Peer* left;
    Peer* right;
    Peer();
    Peer(long long int id);
    void print();
};
