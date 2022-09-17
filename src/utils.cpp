#include "../include/utils.hpp"

Stack::Stack() {}

void Stack::push(Peer peer) {
    items.push_back(peer);
}

Peer Stack::pop() {
    if (!isEmpty()) {
        Peer temp = items[items.size()-1];
        items.pop_back();
        return temp;
    }
    return Peer(-1);
}

bool Stack::isEmpty() {
    return items.size() == 0;
}

Peer Stack::peek() {
    if (!isEmpty()) {
        return items[items.size()-1];
    }
    return Peer(-1);
}
