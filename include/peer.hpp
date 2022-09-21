#pragma once
#include <sstream>
#include <iostream>
#include "file.hpp"
#include "network.hpp"
#include "utils.hpp"

enum MsgType {
    Ping = 0, Pong, FindPeer, FindFile, GetFile, StoreFile, Found, NotFound
};

struct Peer {
    Peer();
    Peer(std::string id);

    std::string id;
    Peer* left;
    Peer* right;

    std::string   host;
    int           port;
    time_t        last_seen;
    int           buffer_size;

    std::tuple<std::string, int, time_t> asTuple();

    std::tuple<std::string, int> address();

    bool send(int, MsgType, std::string);

    std::tuple<MsgType, std::string, sockaddr_in> receive();

    std::tuple<MsgType, std::string, sockaddr_in> sendReceive(int, MsgType, std::string);

    bool ping(int);

    std::tuple<MsgType, std::string, sockaddr_in> findNode(std::string, int);

    std::tuple<MsgType, std::string, sockaddr_in> store(File*, int);

    std::tuple<MsgType, std::string, sockaddr_in> findValue(std::string, int);

    std::tuple<MsgType, std::string, sockaddr_in> getValue(std::string, int);

    bool isOlderThan(int);

    void print();
};
