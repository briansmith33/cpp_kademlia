#include "kbucket.cpp"
#include "peer.cpp"
#include <bitset>
#include <iostream>
#include <ctime>

int main() {
    
    KBucket<Peer> bucket;
    Peer peer1 = Peer("192.168.1.9");
    Peer peer2 = Peer("127.0.0.1");
    bucket.addNode(&peer1);
    bucket.addNode(&peer2);

    return EXIT_SUCCESS;
}
