#pragma once
#include "kbucket.hpp"
#include "peer.hpp"

template<typename T>
struct BucketList {
    BucketList();

    KBucket* head;
    int k_nodes;

    int len();

    void append(KBucket<T>*);

    void prepend(KBucket<T>*);

    void addNode(int, Peer*);
    void addNode(File*);

    T* findNode(std::string);

    void insert(KBucket<T>*);

    void addAfterNode(std::string, KBucket<T>*);

    void addBeforeNode(std::string, KBucket<T>*);

    void removeBucket(KBucket<T>*);

    KBucket<T>* findBucket(std::string);

    KBucket<T>* findClosest(std::string);

    void reverse();

    void removeDuplicates();

    std::vector<KBucket<T>*> list();

    std::vector<T*> listNodes();

    std::vector<std::tuple<std::string, int, time_t>> asTuples();

    void printList();
};
