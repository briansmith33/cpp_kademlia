#include "../include/peer.hpp"

Peer::Peer() {}

Peer::Peer(long long int ident) {
    id = ident;
}

std::tuple<std::string, int, time_t> Peer::asTuple() {
    return std::tuple(host, port, last_seen);
}

void Peer::print() {
    std::cout << id << std::endl; 
}
