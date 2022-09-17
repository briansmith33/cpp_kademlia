#pragma once
#include <sstream>
#include <iostream>
#include "file.hpp"

struct Peer {
    Peer();
    Peer(long long int id);

    long long int id;
    Peer*         left;
    Peer*         right;
    std::string   host;
    int           port;
    time_t        last_seen;
    int           buffer_size;

    std::tuple<std::string, int, time_t> asTuple();

    std::tuple<std::string, int> address();

    bool send(int port, std::string header, std::string message);

    std::tuple<std::string, std::string> receive();

    std::tuple<std::string, std::string> sendReceive(int port, std::string header, std::string message);

    bool ping(int port);

    std::tuple<std::string, std::string> findNode(std::string target, int port);

    std::tuple<std::string, std::string> store(File file, int port);

    std::tuple<std::string, std::string> findValue(std::string target, int port);

    std::tuple<std::string, std::string> getValue(std::string target, int port);

    bool isOlderThan(int seconds);

    void print();
};
