#pragma once
#include <sstream>

struct Event {
    Event();
    Event(std::string data, std::byte signature);

    std::string data;
    std::byte   signature;
    std::string hash;
    std::string prev_hash;
    int         nonce;
    Event*      next;
};
