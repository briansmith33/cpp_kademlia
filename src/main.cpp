#include "kbucket.cpp"
#include "peer.cpp"
#include <bitset>
#include <iostream>
#include <ctime>

int main() {
    
    KBucket<Peer> bucket;
    std::string addr1 = "192.168.1.9";
    std::string addr2 = "127.0.0.1";
    Peer peer1 = Peer(addr1.c_str());
    Peer peer2 = Peer(addr2.c_str());
    bucket.addNode(&peer1);
    bucket.addNode(&peer2);
    
    Peer* found = bucket.findNode(peer2.id);
    found->print();
    
    return EXIT_SUCCESS;
}
