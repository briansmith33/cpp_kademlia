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

    void addNode(T* new_node);
    void addNode(T* new_node, T* current);

    void removeNode(T* node);
    T* removeNode(T* node, T* current);

    T* findNode(std::string node_id);
    T* findNode(std::string node_id, T* current);

    T* findClosest(std::string node_id);
    T* findClosest(std::string node_id, BigInt distance, T* closest, T* current);

    std::vector<T*> findAClosest(std::string node_id);
    
    T* min();
    T* min(T* current);

    T* max();
    T* max(T* current);

    int height();
    int height(T* current);

    int size();
    int size(T* current);

    std::tuple<KBucket, KBucket> split();

    std::vector<T*> preOrder();
    std::vector<T*> preOrder(std::vector<T*> node_list, T* current);

    std::vector<T*> inOrder();
    std::vector<T*> inOrder(std::vector<T*> node_list, T* current);

    std::vector<T*> postOrder();
    std::vector<T*> postOrder(std::vector<T*> node_list, T* current);

    std::vector<T*> timeSort();
    std::vector<T*> timeHeap(std::vector<T*> heap, int n, int i);

    T* oldest();

    std::vector<std::tuple<std::string, int, time_t>> asTuples();
};
