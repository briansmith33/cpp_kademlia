#pragma once
#include "kbucket.hpp"
#include "peer.hpp"

struct BucketList {
    BucketList();

    KBucket* head;
    int k_nodes;

    int len();

    void append(KBucket* bucket);

    void prepend(KBucket* bucket);

    void addPeer(int port, Peer* peer);

    Peer* findPeer(std::string peer_id);

    void insert(KBucket* bucket);

    void addAfterPeer(std::string peer_id, KBucket* bucket);

    void addBeforePeer(std::string peer_id, KBucket* bucket);

    void removeBucket(KBucket* bucket);

    KBucket* findBucket(std::string bucket_id);

    KBucket* findClosest(std::string bucket_id);

    void reverse();

    void removeDuplicates();

    std::vector<KBucket*> list();

    std::vector<Peer*> listPeers();

    std::vector<std::tuple<std::string, int, time_t>> asTuples();

    void printList();
};
