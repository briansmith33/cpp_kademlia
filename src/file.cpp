#include "../include/file.hpp"

File::File() {}

std::tuple<long long int, long long int, std::string, long long int, time_t> File::asTuple() {
    return std::tuple(file_id, owner_id, filename, file_size, published_on);
}

void File::fromTuple(std::tuple<long long int, long long int, std::string, long long int, time_t> file_tuple) {
    std::tie(file_id, owner_id, filename, file_size, published_on) = file_tuple;
}
