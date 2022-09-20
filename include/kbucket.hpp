#pragma once
#include <vector>
#include <ctime>
#include "peer.hpp"
#include "big_int.hpp"

struct KBucket {
    KBucket();

    Peer*    root;
    KBucket* next;
    KBucket* prev;
    int      k_nodes;
    int      alpha;

    void addPeer(Peer*);
    void addPeer(Peer*, Peer*);

    void removePeer(Peer*);
    Peer* removePeer(Peer*, Peer*);

    Peer* findPeer(std::string);
    Peer* findPeer(std::string, Peer*);

    Peer* findClosest(std::string);
    Peer* findClosest(std::string, BigInt, Peer*, Peer*);

    std::vector<Peer*> findAClosest(std::string);
    
    Peer* min();
    Peer* min(Peer*);

    Peer* max();
    Peer* max(Peer*);

    int height();
    int height(Peer*);

    int size();
    int size(Peer*);

    std::tuple<KBucket, KBucket> split();

    std::vector<Peer*> preOrder();
    std::vector<Peer*> preOrder(std::vector<Peer*>, Peer*);

    std::vector<Peer*> inOrder();
    std::vector<Peer*> inOrder(std::vector<Peer*>, Peer*);

    std::vector<Peer*> postOrder();
    std::vector<Peer*> postOrder(std::vector<Peer*>, Peer*);

    std::vector<Peer*> timeSort();

    std::vector<Peer*> timeHeap(std::vector<Peer*>, int, int);

    Peer* oldest();

    std::vector<std::tuple<std::string, int, time_t>> asTuples();
};
