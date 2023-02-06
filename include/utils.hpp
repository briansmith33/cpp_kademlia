#pragma once
#include <vector>
#include "peer.hpp"
#include "big_int.hpp"

template<typename T>
struct Stack {
    Stack();

    std::vector<T*> items;

    void push(T*);

    T* pop();

    bool isEmpty();

    T* peek();
};

std::string sha256(const std::string str);

std::string sha1(std::string text);

std::ifstream::pos_type filesize(const char* filename);

BigInt bigPow(BigInt base, BigInt exponent);

BigInt bigHexor(std::string hex_str1, std::string hex_str2);

BigInt hexToInt(std::string hex_str);

//::std::string base64_encode(const ::std::string &bindata);

//::std::string base64_decode(const ::std::string &ascdata);

char* base64_encode(const char *input);

char* base64_decode(const char *input);
