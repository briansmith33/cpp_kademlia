#include "../include/kbucket.hpp"
#include <math.h>
#include <algorithm>
#include <random>
#include "utils.cpp"

KBucket::KBucket() {
    root = nullptr;
    next = nullptr;
    prev = nullptr;
    k_nodes = 20;
    alpha = 3;
}

void KBucket::addPeer(Peer new_peer) {
    if (root == nullptr) {
        root = &new_peer;
        return;
    }
    addPeer(new_peer, *root);
}

void KBucket::addPeer(Peer new_peer, Peer current) {
    if (new_peer.id < current.id) {
        if (current.left == nullptr) {
            current.left = &new_peer;
        } else {
            addPeer(new_peer, *current.left);
        }
    }

    if (new_peer.id > current.id) {
        if (current.right == nullptr) {
            current.right = &new_peer;
        } else {
            addPeer(new_peer, *current.right);
        }
    }
}

void KBucket::removePeer(Peer peer) {
    removePeer(peer, *root);
}

Peer KBucket::removePeer(Peer peer, Peer current) {
    if (peer.id < current.id) {
        *current.left = removePeer(peer, *current.left);
    } else
    if (peer.id > current.id) {
        *current.right = removePeer(peer, *current.right);
    } else {
        if (current.left == nullptr) {
            return *current.right;
        } else
        if (current.right == nullptr) {
            return *current.left;
        } else {
            Peer temp = min(*current.right);
            current.id = temp.id;
            *current.right = removePeer(temp, *current.right);
        }
    }
    return current;
}

Peer KBucket::findPeer(long long int peer_id) {
    return findPeer(peer_id, *root);
}

Peer KBucket::findPeer(long long int peer_id, Peer current) {
    if (&current == nullptr) {
        return Peer(-1);
    }
    if (peer_id < current.id) {
        if (current.left != nullptr) {
            return findPeer(peer_id, *current.left);
        }
        return Peer(-1);
    } else
    if (peer_id > current.id) {
        if (current.right != nullptr) {
            return findPeer(peer_id, *current.right);
        }
        return Peer(-1);
    } else {
        return current;
    }
}

Peer KBucket::findClosest(long long int peer_id) {
    return findClosest(peer_id, INFINITY, *root, *root);
}

Peer KBucket::findClosest(long long int peer_id, long long int distance, Peer closest, Peer current) {
    if (distance > current.id ^ peer_id) {
        distance = current.id ^ peer_id;
        closest = current;
    }
    if (current.left != nullptr) {
        return findClosest(peer_id, distance, closest, *current.left);
    }
    if (current.right != nullptr) {
        return findClosest(peer_id, distance, closest, *current.right);
    }
    return closest;
}

std::vector<Peer> KBucket::findAClosest(long long int peer_id) {
    std::vector<Peer> peers = inOrder();
    std::vector<Peer> a_closest;
    for (int i = 0; i < alpha; i++) {
        if (peers.size() > 0) {
            int closest_index = 0;
            long long int closest_distance = INFINITY;
            for (int j = 0; j < peers.size(); j++) {
                long long int distance = peers[j].id ^ peer_id;
                if (distance < closest_distance) {
                    closest_index = j;
                    closest_distance = distance;
                }
            }
            a_closest.push_back(peers[closest_index]);
            peers.erase(peers.begin() + closest_index);
        }
    }
    return a_closest;
}

Peer KBucket::min() {
    return min(*root);
}

Peer KBucket::min(Peer current) {
    if (current.left != nullptr) {
        return min(*current.left);
    }
    return current;
}

Peer KBucket::max() {
    return max(*root);
}

Peer KBucket::max(Peer current) {
    if (current.right != nullptr) {
        return max(*current.right);
    }
    return current;
}

int KBucket::height() {
    return height(*root);
}

int KBucket::height(Peer current) {
    if (&current == nullptr) {
        return -1;
    }
    int left_height = height(*current.left);
    int right_height = height(*current.right);
    return 1 + std::max(left_height, right_height);
}

int KBucket::size() {
    if (root == nullptr) {
        return 0;
    }
    
    Stack stack = Stack();
    stack.push(*root);
    int size = 1;
    while (!stack.isEmpty()) {
        Peer current = stack.pop();
        if (current.left != nullptr) {
            size++;
            stack.push(*current.left);
        }
        if (current.right != nullptr) {
            size++;
            stack.push(*current.right);
        }
    }
    return size;
}

std::tuple<KBucket, KBucket> KBucket::split() {
    if (root == nullptr) {
        return std::tuple(KBucket(), KBucket());
    }

    std::vector<Peer> peers = inOrder();
    std::vector<Peer> k1_peers;
    std::vector<Peer> k2_peers;

    for (int i = 0; i < peers.size(); i++) {
        if (i < peers.size() / 2) {
            k1_peers.push_back(peers[i]);
        } else {
            k2_peers.push_back(peers[i]);
        }
    }

    Peer k1_head = k1_peers[std::round(k1_peers.size()/2)];
    Peer k2_head = k2_peers[std::round(k2_peers.size()/2)];

    k1_peers.erase(k1_peers.begin() + std::round(k1_peers.size()/2));
    k2_peers.erase(k2_peers.begin() + std::round(k2_peers.size()/2));

    std::random_device rd;
    std::default_random_engine rng(rd());
    shuffle(k1_peers.begin(), k1_peers.end(), rng);
    shuffle(k2_peers.begin(), k2_peers.end(), rng);

    k1_peers.insert(k1_peers.begin()+0, k1_head);
    k2_peers.insert(k2_peers.begin()+0, k2_head);

    KBucket k1 = KBucket();
    KBucket k2 = KBucket();

    for (auto &peer : k1_peers) {
        k1.addPeer(peer);
    }

    for (auto &peer : k2_peers) {
        k2.addPeer(peer);
    }
    return std::tuple(k1, k2);
}

std::vector<Peer> KBucket::preOrder() {
    std::vector<Peer> peer_list;
    return inOrder(peer_list, *root);
}

std::vector<Peer> KBucket::preOrder(std::vector<Peer> peer_list, Peer current) {
    if (&current == nullptr) {
        return {Peer(-1)};
    }
    peer_list.push_back(current);
    if (current.left != nullptr) {
       inOrder(peer_list, *current.left);
    }
    if (current.right != nullptr) {
        inOrder(peer_list, *current.right);
    }
    return peer_list;
}

std::vector<Peer> KBucket::inOrder() {
    std::vector<Peer> peer_list;
    return inOrder(peer_list, *root);
}

std::vector<Peer> KBucket::inOrder(std::vector<Peer> peer_list, Peer current) {
    if (&current == nullptr) {
        return {Peer(-1)};
    }
    if (current.left != nullptr) {
       inOrder(peer_list, *current.left);
    }
    peer_list.push_back(current);
    if (current.right != nullptr) {
        inOrder(peer_list, *current.right);
    }
    return peer_list;
}

std::vector<Peer> KBucket::postOrder() {
    std::vector<Peer> peer_list;
    return postOrder(peer_list, *root);
}

std::vector<Peer> KBucket::postOrder(std::vector<Peer> peer_list, Peer current) {
    if (&current == nullptr) {
        return {Peer(-1)};
    }
    if (current.left != nullptr) {
       postOrder(peer_list, *current.left);
    }
    
    if (current.right != nullptr) {
        postOrder(peer_list, *current.right);
    }
    peer_list.push_back(current);

    return peer_list;
}

std::vector<Peer> KBucket::timeSort() {
    std::vector<Peer> peers = inOrder();
    int n = size();
    for (int i = std::round(n / 2); i >= 0; i--) {
        peers = timeHeap(peers, n, i);
    }

    for (int i = n - 1; i > 0; i--) {
        Peer temp = peers[i];
        peers[i] = peers[0];
        peers[0] = temp;
        peers = timeHeap(peers, i, 0);
    }
    return peers;
}

std::vector<Peer> KBucket::timeHeap(std::vector<Peer> heap, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && heap[i].last_seen < heap[left].last_seen) {
        largest = left;
    }

    if (right < n && heap[largest].last_seen < heap[right].last_seen) {
        largest = right;
    }

    if (largest != i) {
        Peer temp = heap[i];
        heap[i] = heap[largest];
        heap[largest] = temp;
        timeHeap(heap, n, largest);
    }
}

Peer KBucket::oldest() {
    return timeHeap()[0];
}

std::vector<std::tuple<std::string, int, time_t>> KBucket::asTuples() {
    std::vector<std::tuple<std::string, int, time_t>> peer_tuples;
    for (auto &peer : preOrder()) {
        peer_tuples.push_back(peer.asTuple());
    }
    return peer_tuples;
}
