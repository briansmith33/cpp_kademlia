#pragma once
#include "bucket_list.hpp"
#include "event_chain.hpp"
#include <sstream>

struct Beacon {
    Beacon();
    Beacon(std::string host, int port);

    std::string   host;
    int           port;
    long long int id;
    BucketList    routing_table;
    EventChain    events;
    int           buffer_size;
    int           alpha;
};
