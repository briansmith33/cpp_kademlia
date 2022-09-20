#pragma once
#include <vector>
#include "peer.hpp"
#include "big_int.hpp"

struct Stack {
    Stack();
    std::vector<Peer*> items;
    void push(Peer*);
    Peer* pop();
    bool isEmpty();
    Peer* peek();
};

std::string sha256(const std::string str);

std::string sha1(std::string text);

std::ifstream::pos_type filesize(const char* filename);

BigInt bigPow(BigInt base, BigInt exponent);

BigInt bigHexor(std::string hex_str1, std::string hex_str2);

BigInt hexToInt(std::string hex_str);
