#pragma once
#include <sstream>
#include <iostream>
#include "file.hpp"
#include "network.hpp"
#include "utils.hpp"

enum MsgType {
    Ping = 0, Pong, FindPeer, FindFile, GetFile, StoreFile, Stored, Found, NotFound
};

struct Peer {
    Peer();
    Peer(const char* host);

    std::string id;
    Peer*       left;
    Peer*       right;
    const char* host;
    int         port;
    time_t      last_seen;
    int         buffer_size;

    std::tuple<std::string, time_t> asTuple();

    std::tuple<std::string, int> address();

    bool send(MsgType msg_type, std::string message = "");

    std::tuple<MsgType, std::string, sockaddr_in> receive();

    std::tuple<MsgType, std::string, sockaddr_in> sendReceive(MsgType msg_type, std::string message = "");

    bool ping();

    std::tuple<MsgType, std::string, sockaddr_in> findNode(std::string target);

    std::tuple<MsgType, std::string, sockaddr_in> store(File* file);

    std::tuple<MsgType, std::string, sockaddr_in> findValue(std::string target);

    std::tuple<MsgType, std::string, sockaddr_in> getValue(std::string target);

    bool isOlderThan(int n_seconds);

    void print();
};
