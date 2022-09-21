#include "kbucket.cpp"
#include "peer.cpp"
#include <bitset>
#include <iostream>
#include <ctime>

int main() {
    
	std::string hash1 = sha1("192.168.1.9");
	std::string hash2 = sha1("127.0.0.1");
    
    KBucket<Peer> bucket;
    Peer peer1 = Peer(hash1);
    Peer peer2 = Peer(hash2);
    bucket.addNode(&peer1);
    bucket.addNode(&peer2);
    
    Peer* found = bucket.findNode(hash2);
    found->print();
    
    return EXIT_SUCCESS;
}
