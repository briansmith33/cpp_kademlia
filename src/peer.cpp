#include "../include/peer.hpp"
#include "network.cpp"
#include "file.cpp"
#include <vector>

Peer::Peer() {}

Peer::Peer(char* addr) {
    id = sha1(addr);
    host = addr;
    port = 4444;
    left = nullptr;
    right = nullptr;
    buffer_size = 4096;
}

std::tuple<std::string, int> Peer::address() {
    return std::tuple<std::string, int>(host, port);
}

std::tuple<std::string, time_t> Peer::asTuple() {
    return std::tuple<std::string, time_t>(host, last_seen);
}

bool Peer::send(MsgType msg_type, std::string message) {
    try
    {
        WSASession Session;
        UDPSocket Socket;

        std::string header = std::to_string(msg_type);

        header += ":" + message.size();
        message = base64_encode(message);
        header = base64_encode(header);
        Socket.SendTo(host, port, header.c_str(), header.size());
        Socket.SendTo(host, port, message.c_str(), message.size());
        return true;
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what();
        return false;
    }
    
    return true;
}

std::tuple<MsgType, std::string, sockaddr_in> Peer::receive() {
    try
    {
        WSASession Session;
        UDPSocket Socket;
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
        Socket.RecvFrom(msg_buffer, sizeof(msg_buffer));

        std::string message(msg_buffer);

        message = base64_decode(message);

        return std::tuple<MsgType, std::string, sockaddr_in>(msg_type, message, addr);
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what();
        return std::tuple<MsgType, std::string, sockaddr_in>(NotFound, "", {0});
    }
    
    return std::tuple<MsgType, std::string, sockaddr_in>(NotFound, "", {0});
}

std::tuple<MsgType, std::string, sockaddr_in> Peer::sendReceive(MsgType msg_type, std::string message) {
    send(msg_type, message);    
    return receive();
}

bool Peer::ping() {
    bool success = send(Ping);
    if (!success)
        return false;

    std::tuple<MsgType, std::string, sockaddr_in> response = receive();
    MsgType msg_type;
    std::string message;
    sockaddr_in addr;
    std::tie(msg_type, message, addr) = response;

    if (msg_type == Pong)
        return true;
    else
        return false;
}

std::tuple<MsgType, std::string, sockaddr_in> Peer::findNode(std::string target) {
    return sendReceive(FindPeer, target);
}

std::tuple<MsgType, std::string, sockaddr_in> Peer::store(File* file) {
    
    std::string   file_id;
    std::string   owner_id;
    std::string   filename;
    long long int file_size;
    time_t        published_on;
    std::tuple<std::string, std::string, std::string, long long int, time_t> file_tuple;
    file_tuple = file->asTuple();
    std::tie(file_id, owner_id, filename, file_size, published_on) = file_tuple;

    std::ostringstream msg;
    msg << file_id << ":" << owner_id << ":" << filename << ":" << file_size << ":" << published_on;
    
    
    return sendReceive(StoreFile, msg.str());
}

std::tuple<MsgType, std::string, sockaddr_in> Peer::findValue(std::string target) {
    return sendReceive(FindFile, target);
}

std::tuple<MsgType, std::string, sockaddr_in> Peer::getValue(std::string target) {
    return sendReceive(GetFile, target);
}

bool Peer::isOlderThan(int n_seconds) {
    time_t now = time(0);
    return now - last_seen > n_seconds && last_seen > 0;
}

void Peer::print() {
    std::cout << id << std::endl; 
}
