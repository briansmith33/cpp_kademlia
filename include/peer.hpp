#pragma once
#include <sstream>
#include <iostream>
#include "file.hpp"

struct Peer {
    Peer();
    Peer(std::string id);

    std::string   id;
    Peer*         left;
    Peer*         right;
    std::string   host;
    int           port;
    time_t        last_seen;
    int           buffer_size;

    std::tuple<std::string, int, time_t> asTuple();

    std::tuple<std::string, int> address();

    bool send(int, std::string, std::string);

    std::tuple<std::string, std::string> receive();

    std::tuple<std::string, std::string> sendReceive(int, std::string, std::string);

    bool ping(int);

    std::tuple<std::string, std::string> findNode(std::string, int);

    std::tuple<std::string, std::string> store(File*, int);

    std::tuple<std::string, std::string> findValue(std::string, int);

    std::tuple<std::string, std::string> getValue(std::string, int);

    bool isOlderThan(int);

    void print();
};
