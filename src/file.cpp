#include "../include/file.hpp"
#include <fstream>
#include <string>
#include "utils.cpp"


File::File() {}

File::File(std::string f, long long int peer_id) {
    
    std::ifstream file(f);
    std::string text; 
    if (file.is_open()) {
        while (file) {
            std::string line;
            std::getline(file, line);
            text += line + '\n';
        }
    }
    file_id = std::stoi(sha1(text), 0, 16);
    owner_id = peer_id;
    filename = f;
    file_size = filesize(f.c_str());
    hash = sha256(text);
    published_on = time(NULL);
}

std::tuple<long long int, long long int, std::string, long long int, time_t> File::asTuple() {
    return std::tuple<long long int, long long int, std::string, long long int, time_t>(file_id, owner_id, filename, file_size, published_on);
}

void File::fromTuple(std::tuple<long long int, long long int, std::string, long long int, time_t> file_tuple) {
    std::tie(file_id, owner_id, filename, file_size, published_on) = file_tuple;
}
