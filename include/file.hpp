#pragma once
#include <tuple>
#include <sstream>
#include <ctime>
#include "node.hpp"

struct File {
    File();
    File(std::string filename, std::string peer_id);

    std::string id;
    File* left;
    File* right;

    std::string   owner_id;
    std::string   filename;
    long long int file_size;
    std::string   hash;
    time_t        published_on;

    std::tuple<std::string, std::string, std::string, long long int, time_t> asTuple();

    void fromTuple(std::tuple<std::string, std::string, std::string, long long int, time_t> file_tuple);
};
