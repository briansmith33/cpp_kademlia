#include "../include/kbucket.hpp"
#include <math.h>
#include <algorithm>
#include <random>
#include "utils.cpp"

template<typename T>
KBucket<T>::KBucket() {
    root = nullptr;
    next = nullptr;
    prev = nullptr;
    k_nodes = 20;
    alpha = 3;
}

template<typename T>
void KBucket<T>::addNode(T* new_node) {
    if (root == nullptr) {
        root = new_node;
        return;
    }
    addNode(new_node, root);
}

template<typename T>
void KBucket<T>::addNode(T* new_node, T* current) {
    
    BigInt new_id;
    BigInt current_id;
    new_id = hexToInt(new_node->id);
    current_id = hexToInt(current->id);
    
    if (new_id < current_id) {
        if (current->left == nullptr) {
            current->left = new_node;
        } else {
            addNode(new_node, current->left);
        }
    }

    if (new_id > current_id) {
        if (current->right == nullptr) {
            current->right = new_node;
        } else {
            addNode(new_node, current->right);
        }
    }
}

template<typename T>
void KBucket<T>::removeNode(T* node) {
    removeNode(node, root);
}

template<typename T>
T* KBucket<T>::removeNode(T* node, T* current) {
    BigInt node_id;
    BigInt current_id;
    node_id = hexToInt(node->id);
    current_id = hexToInt(current->id);
    if (node_id < current_id) {
        current->left = removeNode(node, current->left);
    } else
    if (node_id > current_id) {
        current->right = removeNode(node, current->right);
    } else {
        if (current->left == nullptr) {
            return current->right;
        } else
        if (current->right == nullptr) {
            return current->left;
        } else {
            T* temp = min(current->right);
            current->id = temp->id;
            current->right = removeNode(temp, current->right);
        }
    }
    return current;
}

template<typename T>
T* KBucket<T>::findNode(std::string node_id) {
    return findNode(node_id, root);
}

template<typename T>
T* KBucket<T>::findNode(std::string node_id, T* current) {
    if (current == nullptr) {
        return nullptr;
    }
    BigInt target_id;
    BigInt current_id;
    target_id = hexToInt(node_id);
    current_id = hexToInt(current->id);
    if (target_id < current_id) {
        if (current->left != nullptr) {
            return findNode(node_id, current->left);
        }
        return nullptr;
    } else
    if (target_id > current_id) {
        if (current->right != nullptr) {
            return findNode(node_id, current->right);
        }
        return nullptr;
    } else {
        return current;
    }
}

template<typename T>
T* KBucket<T>::findClosest(std::string node_id) {
    BigInt max_id("115792089237316195423570985008687907853269984665640564039457584007913129639935");
    return findClosest(node_id, max_id, root, root);
}

template<typename T>
T* KBucket<T>::findClosest(std::string node_id, BigInt distance, T* closest, T* current) {
    if (distance > bigHexor(current->id, node_id)) {
        distance = bigHexor(current->id, node_id);
        closest = current;
    }
    if (current->left != nullptr) {
        return findClosest(node_id, distance, closest, current->left);
    }
    if (current->right != nullptr) {
        return findClosestPeer(node_id, distance, closest, current->right);
    }
    return closest;
}

template<typename T>
std::vector<T*> KBucket<T>::findAClosest(std::string node_id) {
    std::vector<T*> nodes = inOrder();
    std::vector<T*> a_closest;
    for (int i = 0; i < alpha; i++) {
        if (nodes.size() > 0) {
            int closest_index = 0;
            BigInt closest_distance("115792089237316195423570985008687907853269984665640564039457584007913129639935");
            for (int j = 0; j < nodes.size(); j++) {
                BigInt distance;
                distance = bigHexor(nodes[j]->id, node_id);
                if (distance < closest_distance) {
                    closest_index = j;
                    closest_distance = distance;
                }
            }
            a_closest.push_back(nodes[closest_index]);
            nodes.erase(nodes.begin() + closest_index);
        }
    }
    return a_closest;
}

template<typename T>
T* KBucket<T>::min() {
    return min(root);
}

template<typename T>
T* KBucket<T>::min(T* current) {
    if (current->left != nullptr) {
        return min(current->left);
    }
    return current;
}

template<typename T>
T* KBucket<T>::max() {
    return max(root);
}

template<typename T>
T* KBucket<T>::max(T* current) {
    if (current->right != nullptr) {
        return maxPeer(current->right);
    }
    return current;
}

template<typename T>
int KBucket<T>::height() {
    return height(root);
}

template<typename T>
int KBucket<T>::height(T* current) {
    if (&current == nullptr) {
        return -1;
    }
    int left_height = height(current->left);
    int right_height = height(current->right);
    return 1 + std::max(left_height, right_height);
}

template<typename T>
int KBucket<T>::size() {
    if (root == nullptr) {
        return 0;
    }
    
    Stack<T> stack = Stack<T>();
    stack.push(root);
    int size = 1;
    while (!stack.isEmpty()) {
        T* current = stack.pop();
        if (current->left != nullptr) {
            size++;
            stack.push(current->left);
        }
        if (current->right != nullptr) {
            size++;
            stack.push(current->right);
        }
    }
    return size;
}

template<typename T>
std::tuple<KBucket<T>, KBucket<T>> KBucket<T>::split() {
    if (root == nullptr) {
        return std::tuple<KBucket, KBucket>(KBucket(), KBucket());
    }

    std::vector<T*> nodes = inOrder();
    std::vector<T*> k1_nodes;
    std::vector<T*> k2_nodes;

    for (int i = 0; i < nodes.size(); i++) {
        if (i < nodes.size() / 2) {
            k1_nodes.push_back(nodes[i]);
        } else {
            k2_nodes.push_back(nodes[i]);
        }
    }

    T* k1_head = k1_nodes[std::round(k1_nodes.size()/2)];
    T* k2_head = k2_nodes[std::round(k2_nodes.size()/2)];

    k1_nodes.erase(k1_nodes.begin() + std::round(k1_nodes.size()/2));
    k2_nodes.erase(k2_nodes.begin() + std::round(k2_nodes.size()/2));

    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(k1_nodes.begin(), k1_nodes.end(), rng);
    std::shuffle(k2_nodes.begin(), k2_nodes.end(), rng);

    k1_nodes.insert(k1_nodes.begin(), k1_head);
    k2_nodes.insert(k2_nodes.begin(), k2_head);

    KBucket k1 = KBucket();
    KBucket k2 = KBucket();

    for (auto &node : k1_nodes) {
        k1.addNode(node);
    }

    for (auto &node : k2_nodes) {
        k2.addNode(node);
    }
    return std::tuple<KBucket, KBucket>(k1, k2);
}

template<typename T>
std::vector<T*> KBucket<T>::preOrder() {
    std::vector<T*> node_list;
    return preOrder(node_list, root);
}

template<typename T>
std::vector<T*> KBucket<T>::preOrder(std::vector<T*> node_list, T* current) {
    if (current == nullptr) {
        return {nullptr};
    }
    node_list.push_back(current);
    if (current->left != nullptr) {
       preOrder(node_list, current->left);
    }
    if (current->right != nullptr) {
        preOrder(node_list, current->right);
    }
    return node_list;
}

template<typename T>
std::vector<T*> KBucket<T>::inOrder() {
    std::vector<T*> node_list;
    return inOrder(node_list, root);
}

template<typename T>
std::vector<T*> KBucket<T>::inOrder(std::vector<T*> node_list, T* current) {
    if (current == nullptr) {
        return {nullptr};
    }
    if (current->left != nullptr) {
       inOrder(node_list, current->left);
    }
    node_list.push_back(current);
    if (current->right != nullptr) {
        inOrder(node_list, current->right);
    }
    return node_list;
}

template<typename T>
std::vector<T*> KBucket<T>::postOrder() {
    std::vector<T*> node_list;
    return postOrder(node_list, root);
}

template<typename T>
std::vector<T*> KBucket<T>::postOrder(std::vector<T*> node_list, T* current) {
    if (current == nullptr) {
        return {nullptr};
    }
    if (current->left != nullptr) {
       postOrder(node_list, current->left);
    }
    
    if (current->right != nullptr) {
        postOrder(node_list, current->right);
    }
    node_list.push_back(current);

    return node_list;
}

template<typename T>
std::vector<T*> KBucket<T>::timeSort() {
    std::vector<T*> nodes = inOrder();
    int n = size();
    for (int i = std::round(n / 2); i >= 0; i--) {
        nodes = timeHeap(nodes, n, i);
    }

    for (int i = n - 1; i > 0; i--) {
        T* temp = nodes[i];
        nodes[i] = nodes[0];
        nodes[0] = temp;
        nodes = timeHeap(nodes, i, 0);
    }
    return nodes;
}

template<typename T>
std::vector<T*> KBucket<T>::timeHeap(std::vector<T*> heap, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && heap[i]->last_seen < heap[left]->last_seen) {
        largest = left;
    }

    if (right < n && heap[largest]->last_seen < heap[right]->last_seen) {
        largest = right;
    }

    if (largest != i) {
        Peer* temp = heap[i];
        heap[i] = heap[largest];
        heap[largest] = temp;
        timeHeap(heap, n, largest);
    }
    return {nullptr};
}

template<typename T>
T* KBucket<T>::oldest() {
    return timeSort()[0];
}

template<typename T>
std::vector<std::tuple<std::string, int, time_t>> KBucket<T>::asTuples() {
    std::vector<std::tuple<std::string, int, time_t>> node_tuples;
    for (auto &node : preOrder()) {
        node_tuples.push_back(node->asTuple());
    }
    return node_tuples;
}
