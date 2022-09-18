#pragma once
#include "kbucket.hpp"
#include "peer.hpp"

struct BucketList {
    BucketList();

    KBucket head;
    int k_nodes;

    void append(KBucket bucket);

    void prepend(KBucket bucket);

    void addPeer(int port, Peer peer);

    Peer findPeer(long long int peer_id);

    void insert(KBucket bucket);

    void addAfterPeer(long long int peer_id, KBucket bucket);

    void addBeforePeer(long long int peer_id, KBucket bucket);

    void removeBucket(KBucket bucket);

    KBucket findBucket(long long int bucket_id);

    KBucket findClosest(long long int bucket_id);

    void reverse();

    void removeDuplicates();

    BucketList splitList();

    BucketList sort();

    std::vector<KBucket> list();

    std::vector<Peer> listPeers();

    std::vector<std::tuple<std::string, int>> asTuples();

    void printList();
};
