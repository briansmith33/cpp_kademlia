#pragma once
#include <vector>
#include <ctime>
#include "peer.hpp"


struct KBucket {
    KBucket();
    Peer* root;
    KBucket* next;
    KBucket* prev;
    int k_nodes;
    int alpha;

    void addPeer(Peer new_peer);
    void addPeer(Peer new_peer, Peer current);

    void removePeer(Peer peer);
    Peer removePeer(Peer peer, Peer current);

    Peer findPeer(long long int peer_id);
    Peer findPeer(long long int peer, Peer current);

    Peer findClosest(long long int peer_id);
    Peer findClosest(long long int peer_id, long long int distance, Peer closest, Peer current);

    std::vector<Peer> findAClosest(long long int peer_id);
    
    Peer min();
    Peer min(Peer current);

    Peer max();
    Peer max(Peer current);

    int height();
    int height(Peer current);

    int size();
    int size(Peer current);

    std::tuple<KBucket, KBucket> split();

    std::vector<Peer> preOrder();
    std::vector<Peer> preOrder(std::vector<Peer> peer_list, Peer current);

    std::vector<Peer> inOrder();
    std::vector<Peer> inOrder(std::vector<Peer>peer_list, Peer current);

    std::vector<Peer> postOrder();
    std::vector<Peer> postOrder(std::vector<Peer>peer_list, Peer current);

    std::vector<Peer> timeSort();

    Peer oldest();

    std::vector<Peer> timeHeap();


    std::vector<std::tuple<std::string, int, time_t>> asTuples();
};
