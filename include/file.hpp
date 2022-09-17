#pragma once
#include <tuple>
#include <sstream>

struct File {
    File();

    long long int file_id;
    long long int owner_id;
    std::string   filename;
    long long int file_size;
    time_t        published_on;

    std::tuple<long long int, long long int, std::string, long long int, time_t> asTuple();
    
    void fromTuple(std::tuple<long long int, long long int, std::string, long long int, time_t> file_tuple);
};
