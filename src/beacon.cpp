#include "../include/beacon.hpp"
#include "network.cpp"
#include "utils.cpp"

WSASession Session;
UDPSocket Socket;


Beacon::Beacon(char* baddr) {
    id = sha1(baddr);
    port = 4444;
    boot_addr = baddr;
    host = "127.0.0.1";
    buffer_size = 4096;
    alpha = 3;
}

void Beacon::send(sockaddr_in addr, MsgType msg_type, std::string message) {
    std::string header = std::to_string(msg_type);
    header += ":" + message.size();
    message = base64_encode(message);
    header = base64_encode(message);
    Socket.SendTo(addr, header.c_str(), header.size());
    Socket.SendTo(addr, message.c_str(), message.size());
}

std::tuple<MsgType, std::string, sockaddr_in> Beacon::receive() {
    char buffer[100];
    sockaddr_in addr = Socket.RecvFrom(buffer, sizeof(buffer));
    std::string buf(base64_decode(buffer));
    std::vector<std::string> response;
    
    char* c = const_cast<char*>(buf.c_str());
    char *ptr;  
    ptr = strtok(c, ":");
    while (ptr != NULL)  
    {  
        response.push_back(ptr);
        ptr = strtok (NULL, ":");  
    }  

    std::string header(response[0]);
    std::string msg_len(response[1]);

    int type_int;
    std::stringstream ms;
    ms << header;
    ms >> type_int;

    MsgType msg_type = static_cast<MsgType>(type_int);

    int length;
    std::stringstream ss; 
    ss << msg_len;
    ss >> length;

    char msg_buffer[length];
    sockaddr_in addr = Socket.RecvFrom(msg_buffer, sizeof(msg_buffer));

    std::string message(msg_buffer);

    message = base64_decode(message);

    return std::tuple(msg_type, message, addr);
}

KBucket<Peer>* Beacon::bootstrap() {
    return findPeer(id, &Peer(boot_addr));
}

KBucket<Peer>* Beacon::findPeer(std::string peer_id, Peer* boot_peer, KBucket<Peer>* nearest_bucket) {
    if (boot_peer == nullptr) {
        KBucket<Peer>* closest_bucket = routing_table.findClosest(peer_id);
        boot_peer = closest_bucket->findClosest(peer_id);
    }

    if (nearest_bucket == nullptr) {
        std::tuple<MsgType, std::string, sockaddr_in> response = boot_peer->findNode(peer_id);
        MsgType msg_type;
        std::string message;
        sockaddr_in addr;
        std::tie(msg_type, message, addr) = response;
        if (msg_type == NotFound) {
            return nullptr;
        }
        if (msg_type == Found) {
            KBucket<Peer> bucket;
            char* c = const_cast<char*>(message.c_str());
            char *peer_tuple;  
            peer_tuple = strtok(c, ":");
            while (peer_tuple != NULL)  
            {  
                char *peer_info;  
                peer_info = strtok(peer_info, ",");
                std::vector<char*> peer_vector;
                while (peer_info != NULL) {
                    peer_vector.push_back(peer_info);
                    peer_info = strtok(NULL, ","); 
                }
                Peer peer(peer_vector[0]);
                if (peer.id != boot_peer->id) {
                    bool response = peer.ping();
                    if (response) {
                        bucket.addNode(&peer);
                    }
                } else {
                    bucket.addNode(&peer);
                }
                peer_tuple = strtok(NULL, ":");  
            }
            routing_table.append(&bucket);
            return findPeer(peer_id, boot_peer, &bucket);
        }
    } else {
        std::string nearest = nearest_bucket->findClosest(peer_id)->id;
        if (nearest == boot_peer->id) {
            return nearest_bucket;
        }
        std::string original = nearest;
        std::vector<Peer*> a_closest = nearest_bucket->findAClosest(peer_id);
        for (auto &peer : a_closest) {
            if (peer->id != boot_peer->id) {
                std::tuple<MsgType, std::string, sockaddr_in> response = boot_peer->findNode(peer_id);

                MsgType msg_type;
                std::string message;
                sockaddr_in addr;
                std::tie(msg_type, message, addr) = response;

                if (msg_type == Found) {
                    KBucket<Peer> bucket;
                    char* c = const_cast<char*>(message.c_str());
                    char *peer_tuple;  
                    peer_tuple = strtok(c, ":");
                    while (peer_tuple != NULL)  
                    {  
                        char *peer_info;  
                        peer_info = strtok(peer_info, ",");
                        std::vector<char*> peer_vector;
                        while (peer_info != NULL) {
                            peer_vector.push_back(peer_info);
                            peer_info = strtok(NULL, ","); 
                        }
                        Peer peer(peer_vector[0]);
                        if (routing_table.findNode(peer.id) != nullptr && peer.id != id) {
                            bool response = peer.ping();
                            if (response) {
                                routing_table.addNode(&peer);
                                bucket.addNode(&peer);
                            }
                        }
                        peer_tuple = strtok(NULL, ":");  
                    }
                    if (bucket.size() > 0) {
                        std::string closest = bucket.findClosest(peer_id)->id;
                        if (hexToInt(closest) < hexToInt(nearest)) {
                            nearest = closest;
                            nearest_bucket = &bucket;
                        }
                    }
                }
            }
        }
        if (hexToInt(nearest) < hexToInt(original)) {
            return findPeer(peer_id, boot_peer, nearest_bucket);
        }
    }
    return nearest_bucket;
}

File* Beacon::findValue(std::string key) {
    KBucket<Peer>* closest_bucket = findPeer(key);
    File found;
    for (auto &peer : closest_bucket->preOrder()) {
        std::tuple<MsgType, std::string, sockaddr_in> response = peer->findValue(key);
        MsgType msg_type;
        std::string message;
        sockaddr_in addr;
        std::tie(msg_type, message, addr) = response;
        if (msg_type == Found) {
            char* c = const_cast<char*>(message.c_str());
            char *file_info;  
            file_info = strtok(c, ":");
            std::vector<std::string> file_vector;
            while (file_info != NULL) {
                file_vector.push_back(file_info);
                file_info = strtok(NULL, ":"); 
            }

            long long int file_size;
            std::stringstream ss;
            ss << file_vector[3];
            ss >> file_size;

            long pub_time;
            std::stringstream ts;
            ts << file_vector[4];
            ts >> pub_time;

            std::string   file_id = file_vector[0];
            std::string   owner_id = file_vector[1];
            std::string   filename = file_vector[2];
            
            time_t        published_on = pub_time;
            std::tuple<std::string, std::string, std::string, long long int, time_t> file_tuple(
                    file_id, 
                    owner_id, 
                    filename, 
                    file_size, 
                    published_on);
            found.fromTuple(file_tuple);
        }
    }
    if (&found.id != nullptr) {
        file_storage.addNode(&found);
        KBucket<Peer>* closest_bucket = findPeer(found.owner_id);
        Peer* owner = closest_bucket->findNode(found.owner_id);
        MsgType msg_type;
        std::string message;
        sockaddr_in addr;
        std::tie(msg_type, message, addr) = owner->getValue(found.filename);
        if (msg_type == Found) {

        }
    }
}

void Beacon::store(std::string filename) {
    File file = File(filename, id);
    KBucket<Peer>* closest_bucket = findPeer(file.id);
    for (auto &peer : closest_bucket->preOrder()) {
        peer->store(&file);
    }
}

void Beacon::saveState() {

}

void Beacon::broadcast(Event*) {

}

void Beacon::run() {
    try
    {
        Socket.Bind(port);

        if (boot_addr != nullptr)
            bootstrap();

        while (true)
        {
            std::tuple<MsgType, std::string, sockaddr_in> response = receive();

            MsgType msg_type;
            std::string message;
            sockaddr_in addr;
            std::tie(msg_type, message, addr) = response;
            
            if (msg_type == Ping) {
                send(addr, Pong);
                char* peer_address = inet_ntoa(addr.sin_addr);
                std::string peer_id = sha1(peer_address);
                if (routing_table.findNode(peer_id) == nullptr) {
                    routing_table.addNode(&Peer(peer_address));
                    joinMessage(peer_id);
                }
            }

            if (msg_type == FindPeer) {
                std::string peer_id = message;
                KBucket<Peer>* bucket = routing_table.findClosest(peer_id);
                if (bucket == nullptr) {
                    std::ostringstream msg;
                    msg << host << "," << port << "," << time(NULL);
                    send(addr, Found, msg.str());
                } else {
                    std::vector<std::tuple<std::string, int, time_t>> found_tuples = bucket->asTuples();
                    std::ostringstream msg;
                    for (auto &tup : found_tuples) {
                        std::string adr;
                        int prt;
                        time_t ls;
                        std::tie(adr, prt, ls) = tup;
                        msg << adr << "," << prt << "," << ls << ":";
                    }
                    msg << host << "," << port << "," << time(NULL);
                    send(addr, Found, msg.str());
                }

                char* peer_address = inet_ntoa(addr.sin_addr);
                Peer peer(peer_address);
                if (routing_table.findNode(peer.id) == nullptr) {
                    routing_table.addNode(&peer);
                    joinMessage(peer.id);
                }
            }

            if (msg_type == FindFile) {
                std::string file_id = message;
                File* file = file_storage.findNode(file_id);
                if (file == nullptr) {
                    send(addr, NotFound);
                } else {
                    std::string   file_id;
                    std::string   owner_id;
                    std::string   filename;
                    long long int file_size;
                    time_t        published_on;

                    std::tie(file_id, owner_id, filename, file_size, published_on) = file->asTuple();

                    std::ostringstream msg;
                    msg << file_id << ":" << owner_id << ":" << filename << ":" << file_size << ":" << published_on;
                    send(addr, Found, msg.str());
                }
            }

            if (msg_type == StoreFile) {
                char* c = const_cast<char*>(message.c_str());
                char *file_info;  
                file_info = strtok(c, ":");
                std::vector<std::string> file_vector;
                while (file_info != NULL) {
                    file_vector.push_back(file_info);
                    file_info = strtok(NULL, ":"); 
                }

                long long int file_size;
                std::stringstream ss;
                ss << file_vector[3];
                ss >> file_size;

                long pub_time;
                std::stringstream ts;
                ts << file_vector[4];
                ts >> pub_time;

                std::string   file_id = file_vector[0];
                std::string   owner_id = file_vector[1];
                std::string   filename = file_vector[2];
                
                time_t        published_on = pub_time;
                std::tuple<std::string, std::string, std::string, long long int, time_t> file_tuple(
                    file_id, 
                    owner_id, 
                    filename, 
                    file_size, 
                    published_on);

                File* file;
                file->fromTuple(file_tuple);
            }
        }
    }
    catch (std::system_error& e)
    {
        std::cout << e.what();
    }
}
