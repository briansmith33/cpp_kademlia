#include "../include/peer.hpp"

Peer::Peer() {}

Peer::Peer(long long int ident) {
    id = ident;
}

void Peer::print() {
    std::cout << id << std::endl; 
}
