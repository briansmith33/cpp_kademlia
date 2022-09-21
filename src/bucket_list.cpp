#include "../include/bucket_list.hpp"

template<typename T>
BucketList<T>::BucketList() {
    k_nodes = 20;
}

template<typename T>
int BucketList<T>::len() {
    int length = 0;
    KBucket* current = head;
    do {
        length++;
        current = current->next;
    } while (current != head);
    return length;
}

template<typename T>
void BucketList<T>::append(KBucket<T>* bucket) {
    if (head == nullptr) {
        head = bucket;
        head->next = head;
        head->prev = head;
    } else {
        KBucket* prev = head->prev;
        head->prev = bucket;
        bucket->next = head;
        prev->next = bucket;
        bucket->prev = prev;
    }
}

template<typename T>
void BucketList<T>::prepend(KBucket<T>* bucket) {
    if (head == nullptr) {
        head = bucket;
        head->next = head;
        head->prev = head;
    } else {
        KBucket* prev = head->prev;
        head->prev = bucket;
        bucket->next = head;
        bucket->prev = prev;
        prev->next = bucket;
        head = bucket;
    }
}

template<typename T>
void BucketList<T>::addNode(Peer* peer) {
    KBucket* bucket = findClosest(peer->id);
    if (bucket == nullptr) {
        *bucket = KBucket();
        bucket->addNode(peer);
        insert(bucket);
    } else
    if (bucket->size<Peer>() < bucket->k_nodes) {
        bucket->addNode(peer);
    } else {
        Peer* oldest_peer = bucket->oldest<Peer>();
        if (oldest_peer->isOlderThan(3600)) {
            bool is_up = oldest_peer->ping();
            if (!is_up) {
                bucket->removeNode(oldest_peer);
                bucket->addNode(peer);
                return;
            }
        }
        KBucket k1;
        KBucket k2;
        std::tie(k1, k2) = bucket->split<Peer>();

        removeBucket(bucket);
        BigInt left_distance = bigHexor(k1.root->id, peer->id);
        BigInt right_distance = bigHexor(k2.root->id, peer->id);
        if (left_distance < right_distance) {
            k1.addNode(peer);
        } else {
            k2.addNode(peer);
        }
        insert(&k1);
        insert(&k2);
    }
}

template<typename T>
void BucketList<T>::addNode(File* file) {
    KBucket* bucket = findClosest(file->id);
    if (bucket == nullptr) {
        *bucket = KBucket();
        bucket->addNode<File>(file);
        insert(bucket);
    } else
    if (bucket->size<Peer>() < bucket->k_nodes) {
        bucket->addNode(file);
    } else {
        /*
        File* oldest_file = bucket->oldest<File>();
        if (oldest_file->isOlderThan(3600)) {
            bucket->removeNode(oldest_peer);
            bucket->addNode(peer);
            return;
        }
        */
        KBucket k1;
        KBucket k2;
        std::tie(k1, k2) = bucket->split<File>();

        removeBucket(bucket);
        BigInt left_distance = bigHexor(k1.root->id, file->id);
        BigInt right_distance = bigHexor(k2.root->id, file->id);
        if (left_distance < right_distance) {
            k1.addNode(file);
        } else {
            k2.addNode(file);
        }
        insert(&k1);
        insert(&k2);
    }
}

template<typename T>
T* BucketList<T>::findNode(std::string node_id) {
    KBucket* bucket = findClosest(node_id);
    if (bucket == nullptr) {
        T* peer = bucket->findNode<T>(node_id);
        if (peer == nullptr) {
            peer = bucket->prev->findNode<T>(node_id);
        }
        if (peer == nullptr) {
            peer = bucket->next->findNode<T>(node_id);
        }
        return peer;
    }
    return nullptr;
}

template<typename T>
void BucketList<T>::insert(KBucket<T>* bucket) {
    std::vector<KBucket*> buckets = list();
    if (buckets.size() == 0) {
        append(bucket);
        return;
    }

    BigInt root_id = hexToInt(bucket->root->id);
    BigInt last_id = hexToInt(buckets[buckets.size()-1]->root->id);
    BigInt first_id = hexToInt(buckets[0]->root->id);
    if (root_id > last_id) {
        append(bucket);
        return;
    }

    if (root_id < first_id) {
        prepend(bucket);
        return;
    }
    
    for (int i = 0; i < buckets.size(); i++) {
        BigInt current_id = hexToInt(buckets[i]->root->id);
        BigInt next_id = hexToInt(buckets[i+1]->root->id);
        if (current_id < root_id < next_id) {
            addAfterNode(buckets[i]->root->id, bucket);
            return;
        }
    }
}
template<typename T>
void BucketList<T>::addAfterNode(std::string node_id, KBucket<T>* bucket) {
    KBucket* current = head;
    while (current != nullptr) {
        if (current->next == head && current->root->id == node_id) {
            append(bucket);
            return;
        } else
        if (current->root->id == node_id) {
            KBucket* next = current->next;
            current->next = bucket;
            bucket->next = next;
            bucket->prev = current;
            next->prev = bucket;
            return;
        }
        current = current->next;
    }
}

template<typename T>
void BucketList<T>::addBeforeNode(std::string node_id, KBucket<T>* bucket) {
    KBucket* current = head;
    while (current != nullptr) {
        if (current->prev == head && current->root->id == node_id) {
            prepend(bucket);
            return;
        } else
        if (current->root->id == node_id) {
            KBucket* prev = current->prev;
            prev->next = bucket;
            current->prev = bucket;
            bucket->next = current;
            bucket->prev = prev;
            return;
        }
        current = current->next;
    } 
}

template<typename T>
void BucketList<T>::removeBucket(KBucket<T>* bucket) {
    KBucket* current = head;
    do {
        if (current == bucket && current == head) {
            if (current->next == current) {
                head = nullptr;
            } else {
                KBucket* next = current->next;
                KBucket* prev = current->prev;
                prev->next = next;
                next->prev = prev;
                head = next;
            }
            return;
        } else
        if (current == bucket) {
            KBucket* next = current->next;
            KBucket* prev = current->prev;
            prev->next = next;
            next->prev = prev;
            return;
        }
        current = current->next;

    } while (current != head);
}

template<typename T>
KBucket<T>* BucketList<T>::findBucket(std::string bucket_id) {
    KBucket* current = head;
    if (current->root->id == bucket_id) {
        return current;
    }
    BigInt left_distance = bigHexor(current->prev->root->id, bucket_id);
    BigInt right_distance = bigHexor(current->next->root->id, bucket_id);
    if (left_distance < right_distance) {
        do {
            if (current->root->id == bucket_id) {
                return current;
            }
            current = current->prev;
        } while (current != head);
    } else {
        do {
            if (current->root->id == bucket_id) {
                return current;
            }
            current = current->next;
        } while (current != head);
    }
    return nullptr;
}

template<typename T>
KBucket<T>* BucketList<T>::findClosest(std::string bucket_id) {
    KBucket* current = head;
    do {
        BigInt left_distance = bigHexor(current->prev->root->id, bucket_id);
        BigInt right_distance = bigHexor(current->next->root->id, bucket_id);
        if (hexToInt(current->root->id) < hexToInt(bucket_id) < hexToInt(current->next->root->id)) {
            if (left_distance < right_distance) {
                return current;
            }
            return current->next;
        }
        if (current->next == head) {
            if (left_distance <= right_distance) {
                return current;
            }
            return current->next;
        }
        current = current->next;
    } while (current != head);
}

template<typename T>
void BucketList<T>::reverse() {
    KBucket* temp;
    KBucket* current = head;
    do {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;
        current = current->prev;
    } while (current != head);
    if (temp != nullptr) {
        head = temp->prev;
    }
}

template<typename T>
void BucketList<T>::removeDuplicates() {
    KBucket* current = head;
    std::vector<std::string> seen;
    do {
        std::vector<std::string>::iterator it;
        it = find(seen.begin(), seen.end(), current->root->id);

        if (it != seen.end()) {
            KBucket* next = current->next;
            removeBucket(current);
            current = next;
        } else {
            seen.push_back(current->root->id);
            current = current->next;
        }
        
    } while (current != head);
}

template<typename T>
std::vector<KBucket<T>*> BucketList<T>::list() {
    std::vector<KBucket*> buckets;
    KBucket* current = head;
    do {
        buckets.push_back(current);
        current = current->next;
    } while (current != head);
    return buckets;
}

template<typename T>
std::vector<T*> BucketList<T>::listNodes() {
    std::vector<T*> nodes;
    KBucket* current = head;
    do {
        for (auto &node : current->inOrder<T>()) {
            nodes.push_back(node);
        }
        current = current->next;
    } while (current != head);
    return nodes;
}

template<typename T>
std::vector<std::tuple<std::string, int, time_t>> BucketList<T>::asTuples() {
    std::vector<std::tuple<std::string, int, time_t>> tuples;
    KBucket* current = head;
    do {
        for (auto &peer : current->preOrder<Peer>()) {
            tuples.push_back(peer->asTuple());
        }
        current = current->next;
    } while (current != head);
    return tuples;
}

template<typename T>
void BucketList<T>::printList() {
    KBucket* current = head;
    do {
        std::cout << current->root->id << std::endl;
        current = current->next;
    } while (current != head);
}
