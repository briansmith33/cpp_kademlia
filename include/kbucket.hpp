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

    void addPeer(Peer* new_peer);
    void addPeer(Peer* new_peer, Peer* current);

    void removePeer(Peer* peer);
    Peer* removePeer(Peer* peer, Peer* current);

    Peer* findPeer(std::string peer_id);
    Peer* findPeer(std::string peer_id, Peer* current);

    Peer* findClosest(std::string peer_id);
    Peer* findClosest(std::string peer_id, BigInt distance, Peer* closest, Peer* current);

    std::vector<Peer*> findAClosest(std::string peer_id);
    
    Peer* min();
    Peer* min(Peer* current);

    Peer* max();
    Peer* max(Peer* current);

    int height();
    int height(Peer* current);

    int size();
    int size(Peer* current);

    std::tuple<KBucket, KBucket> split();

    std::vector<Peer*> preOrder();
    std::vector<Peer*> preOrder(std::vector<Peer*> peer_list, Peer* current);

    std::vector<Peer*> inOrder();
    std::vector<Peer*> inOrder(std::vector<Peer*>peer_list, Peer* current);

    std::vector<Peer*> postOrder();
    std::vector<Peer*> postOrder(std::vector<Peer*>peer_list, Peer* current);

    std::vector<Peer*> timeSort();

    std::vector<Peer*> timeHeap(std::vector<Peer*> heap, int n, int i);

    Peer* oldest();

    std::vector<std::tuple<std::string, int, time_t>> asTuples();
};
