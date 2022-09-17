//#include "kbucket.cpp"
//#include"peer.cpp"
#include <ctime>
#include <iostream>
#include <cmath>

int main() {
    for (int i = std::round(20 / 2); i >= 0; i--) {
        std::cout << i << std::endl;
    }
    /*
    time_t now = time(0);

    KBucket bucket = KBucket();
    Peer peer1 = Peer(65389757258397095);
    Peer peer2 = Peer(83256023750982500);
    bucket.addPeer(peer1);
    bucket.addPeer(peer2);
    Peer found = bucket.findPeer(83256023750982500);
    found.print();
    */
    return EXIT_SUCCESS;
}
