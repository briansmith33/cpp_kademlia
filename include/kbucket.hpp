#pragma once
#include <vector>
#include <ctime>
#include "peer.hpp"
#include "big_int.hpp"
#include "utils.hpp"

template<typename T>
struct KBucket {
    KBucket();

    T*    root;
    KBucket* next;
    KBucket* prev;
    int      k_nodes;
    int      alpha;

    void addNode(T*);
    void addNode(T*, T*);

    void removeNode(T*);
    T* removeNode(T*, T*);

    T* findNode(std::string);
    T* findNode(std::string, T*);

    T* findClosest(std::string);
    T* findClosest(std::string, BigInt, T*, T*);

    std::vector<T*> findAClosest(std::string);
    
    T* min();
    T* min(T*);

    T* max();
    T* max(T*);

    int height();
    int height(T*);

    int size();
    int size(T*);

    std::tuple<KBucket, KBucket> split();

    std::vector<T*> preOrder();
    std::vector<T*> preOrder(std::vector<T*>, T*);

    std::vector<T*> inOrder();
    std::vector<T*> inOrder(std::vector<T*>, T*);

    std::vector<T*> postOrder();
    std::vector<T*> postOrder(std::vector<T*>, T*);

    std::vector<T*> timeSort();
    std::vector<T*> timeHeap(std::vector<T*>, int, int);

    T* oldest();

    std::vector<std::tuple<std::string, int, time_t>> asTuples();
};
