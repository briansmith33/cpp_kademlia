#pragma once
#include "bucket_list.hpp"
#include "event_chain.hpp"
#include "event.hpp"
#include "network.hpp"
#include <sstream>

struct Beacon {
    Beacon();
    Beacon(int, int);

    std::string         host;
    int                 port;
    std::string         boot_addr;
    int                 boot_port;
    std::string         id;
    BucketList<Peer>    routing_table;
    BucketList<File>    file_storage;
    EventChain          events;
    int                 buffer_size;
    int                 alpha;

    void send(sockaddr_in, MsgType, std::string);

    std::tuple<MsgType, std::string, sockaddr_in> receive();

    KBucket<Peer>* bootstrap();

    KBucket<Peer>* findPeer(std::string, Peer*, KBucket<Peer>*);

    File* findValue(std::string);

    void store(std::string);

    void saveState();

    void broadcast(Event*);

    void run();
};
