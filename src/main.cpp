#include "kbucket.cpp"
#include "peer.cpp"
#include <bitset>


int main() {
	std::string hash1 = sha256("192.168.1.9");
	std::string hash2 = sha256("127.0.0.1");
    
    KBucket bucket = KBucket();
    Peer peer1 = Peer(hash1);
    Peer peer2 = Peer(hash2);
    bucket.addPeer(&peer1);
    bucket.addPeer(&peer2);
    Peer* found = bucket.findPeer(hash2);
    found->print();
    
    return EXIT_SUCCESS;
}
