#include "kbucket.cpp"
#include "peer.cpp"
#include "utils.cpp"

 
int main() {

    KBucket bucket = KBucket();
    Peer peer1 = Peer(65389757258397095);
    Peer peer2 = Peer(83256023750982500);
    bucket.addPeer(peer1);
    bucket.addPeer(peer2);
    Peer found = bucket.findPeer(83256023750982500);
    found.print();

    return EXIT_SUCCESS;
}
