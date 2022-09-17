#pragma once
#include <sstream>
#include <iostream>

struct Peer {
    long long int id;
    Peer* left;
    Peer* right;
    std::string host;
    int port;
    time_t last_seen;
    Peer();
    Peer(long long int id);
    std::tuple<std::string, int, time_t> asTuple();
    void print();
};
