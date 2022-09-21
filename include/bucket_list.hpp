#pragma once
#include "kbucket.hpp"
#include "peer.hpp"

template<typename T>
struct BucketList {
    BucketList();

    KBucket* head;
    int k_nodes;

    int len();

    void append(KBucket<T>* bucket);

    void prepend(KBucket<T>* bucket);

    void addNode(Peer* peer);
    void addNode(File* file);

    T* findNode(std::string node_id);

    void insert(KBucket<T>* bucket);

    void addAfterNode(std::string node_id, KBucket<T>* bucket);

    void addBeforeNode(std::string node_id, KBucket<T>* bucket);

    void removeBucket(KBucket<T>* bucket);

    KBucket<T>* findBucket(std::string bucket_id);

    KBucket<T>* findClosest(std::string bucket_id);

    void reverse();

    void removeDuplicates();

    std::vector<KBucket<T>*> list();

    std::vector<T*> listNodes();

    std::vector<std::tuple<std::string, int, time_t>> asTuples();

    void printList();
};
