#pragma once
#include "kbucket.hpp"
#include "peer.hpp"

struct BucketList {
    BucketList();

    KBucket* head;
    int k_nodes;

    int len();

    void append(KBucket*);

    void prepend(KBucket*);

    void addPeer(int, Peer*);

    Peer* findPeer(std::string);

    void insert(KBucket*);

    void addAfterPeer(std::string, KBucket*);

    void addBeforePeer(std::string, KBucket*);

    void removeBucket(KBucket*);

    KBucket* findBucket(std::string);

    KBucket* findClosest(std::string);

    void reverse();

    void removeDuplicates();

    std::vector<KBucket*> list();

    std::vector<Peer*> listPeers();

    std::vector<std::tuple<std::string, int, time_t>> asTuples();

    void printList();
};
