#include "../include/bucket_list.hpp"
#include "big_int.cpp"
#include "utils.cpp"

BucketList::BucketList() {}

int BucketList::len() {
    int length = 0;
    KBucket* current = head;
    do {
        length++;
        current = current->next;
    } while (current != head);
    return length;
}

void BucketList::append(KBucket* bucket) {
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

void BucketList::prepend(KBucket* bucket) {
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

void BucketList::addPeer(int port, Peer* peer) {
    KBucket* bucket = findClosest(peer->id);
    if (bucket == nullptr) {
        *bucket = KBucket();
        bucket->addPeer(peer);
        insert(bucket);
    } else
    if (bucket->size() < bucket->k_nodes) {
        bucket->addPeer(peer);
    } else {
        Peer* oldest_peer = bucket->oldest();
        if (oldest_peer->isOlderThan(3600)) {
            bool is_up = oldest_peer->ping(port);
            if (!is_up) {
                bucket->removePeer(oldest_peer);
                bucket->addPeer(peer);
                return;
            }
        }
        KBucket k1;
        KBucket k2;
        std::tie(k1, k2) = bucket->split();

        removeBucket(bucket);
        BigInt left_distance = bigHexor(k1.root->id, peer->id);
        BigInt right_distance = bigHexor(k2.root->id, peer->id);
        if (left_distance < right_distance) {
            k1.addPeer(peer);
        } else {
            k2.addPeer(peer);
        }
        insert(&k1);
        insert(&k2);
    }
}

Peer* BucketList::findPeer(std::string peer_id) {
    KBucket* bucket = findClosest(peer_id);
    if (bucket == nullptr) {
        Peer* peer = bucket->findPeer(peer_id);
        if (peer == nullptr) {
            peer = bucket->prev->findPeer(peer_id);
        }
        if (peer == nullptr) {
            peer = bucket->next->findPeer(peer_id);
        }
        return peer;
    }
    return nullptr;
}

void BucketList::insert(KBucket* bucket) {
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
            addAfterPeer(buckets[i]->root->id, bucket);
            return;
        }
    }
}

void BucketList::addAfterPeer(std::string peer_id, KBucket* bucket) {
    KBucket* current = head;
    while (current != nullptr) {
        if (current->next == head && current->root->id == peer_id) {
            append(bucket);
            return;
        } else
        if (current->root->id == peer_id) {
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

void BucketList::addBeforePeer(std::string peer_id, KBucket* bucket) {
    KBucket* current = head;
    while (current != nullptr) {
        if (current->prev == head && current->root->id == peer_id) {
            prepend(bucket);
            return;
        } else
        if (current->root->id == peer_id) {
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

void BucketList::removeBucket(KBucket* bucket) {
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

KBucket* BucketList::findBucket(std::string bucket_id) {
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

KBucket* BucketList::findClosest(std::string bucket_id) {
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

void BucketList::reverse() {
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

void BucketList::removeDuplicates() {
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

std::vector<KBucket*> BucketList::list() {
    std::vector<KBucket*> buckets;
    KBucket* current = head;
    do {
        buckets.push_back(current);
        current = current->next;
    } while (current != head);
    return buckets;
}

std::vector<Peer*> BucketList::listPeers() {
    std::vector<Peer*> peers;
    KBucket* current = head;
    do {
        for (auto &peer : current->inOrder()) {
            peers.push_back(peer);
        }
        current = current->next;
    } while (current != head);
    return peers;
}

std::vector<std::tuple<std::string, int, time_t>> BucketList::asTuples() {
    std::vector<std::tuple<std::string, int, time_t>> tuples;
    KBucket* current = head;
    do {
        for (auto &peer : current->preOrder()) {
            tuples.push_back(peer->asTuple());
        }
        current = current->next;
    } while (current != head);
    return tuples;
}

void BucketList::printList() {
    KBucket* current = head;
    do {
        std::cout << current->root->id << std::endl;
        current = current->next;
    } while (current != head);
}
