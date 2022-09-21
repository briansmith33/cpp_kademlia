#pragma once
#include "bucket_list.hpp"
#include "event_chain.hpp"
#include "event.hpp"
#include "network.hpp"
#include <sstream>

struct Beacon {
    Beacon(char* boot_addr = nullptr);

    char*               host;
    int                 port;
    char*               boot_addr;
    std::string         id;
    BucketList<Peer>    routing_table;
    BucketList<File>    file_storage;
    EventChain          events;
    int                 buffer_size;
    int                 alpha;

    void send(sockaddr_in addr, MsgType msg_type, std::string message);

    std::tuple<MsgType, std::string, sockaddr_in> receive();

    KBucket<Peer>* bootstrap();

    KBucket<Peer>* findPeer(std::string peer_id, Peer* boot_peer = nullptr, KBucket<Peer>* nearest_bucket = nullptr);

    File* findValue(std::string key);

    void store(std::string);

    void saveState();

    void broadcast(Event* event);

    void run();
};
