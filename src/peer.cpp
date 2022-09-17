#include "../include/peer.hpp"
#include "../include/network.hpp"

Peer::Peer() {}

Peer::Peer(long long int ident) {
    id = ident;
}

std::tuple<std::string, int> Peer::address() {
    return std::tuple(host, port);
}

std::tuple<std::string, int, time_t> Peer::asTuple() {
    return std::tuple(host, port, last_seen);
}

bool Peer::send(int port, std::string header, std::string message) {
    try
    {
        WSASession Session;
        UDPSocket Socket;
        Socket.SendTo(host, port, header.c_str(), header.size());
        Socket.SendTo(host, port, message.c_str(), message.size());
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what();
    }

}

std::tuple<std::string, std::string> Peer::receive() {
    try
    {
        WSASession Session;
        UDPSocket Socket;
        char header[100];
        Socket.RecvFrom(header, 100);

        char message[100];
        Socket.RecvFrom(message, 100);

        return std::tuple(header, message);
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what();
    }
}

std::tuple<std::string, std::string> Peer::sendReceive(int port, std::string header, std::string message) {
    try
    {
        WSASession Session;
        UDPSocket Socket;
        Socket.SendTo(host, port, header.c_str(), header.size());
        Socket.SendTo(host, port, message.c_str(), message.size());

        char receive_header[100];
        Socket.RecvFrom(receive_header, 100);

        char receive_message[100];
        Socket.RecvFrom(receive_message, 100);

        return std::tuple(receive_header, receive_message);
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what();
    }
}

bool Peer::ping(int port) {
    try
    {
        WSASession Session;
        UDPSocket Socket;
        std::string send_header = "ping";
        std::string send_message = "";
        Socket.SendTo(host, port, send_header.c_str(), send_header.size());
        Socket.SendTo(host, port, send_message.c_str(), send_message.size());

        char receive_header[100];
        Socket.RecvFrom(receive_header, 100);

        char receive_message[100];
        Socket.RecvFrom(receive_message, 100);
        if (receive_header == "pong") {
            return true;
        } else {
            return false;
        }
        
    }
    catch (std::exception &ex)
    {
        return false;
    }
}

std::tuple<std::string, std::string> Peer::findNode(std::string target, int port) {
    return sendReceive(port, "find_node", target);
}

std::tuple<std::string, std::string> Peer::store(File file, int port) {
    std::string key;
    std::string value;
    std::tie(key, value) = file.asTuple();

    return sendReceive(port, "store", key+":"+value);
}

std::tuple<std::string, std::string> Peer::findValue(std::string target, int port) {
    return sendReceive(port, "find_value", target);
}

std::tuple<std::string, std::string> Peer::getValue(std::string target, int port) {
    return sendReceive(port, "get_value", target);
}

bool Peer::isOlderThan(int n_seconds) {
    time_t now = time(0);
    return now - last_seen > n_seconds && last_seen > 0;
}

void Peer::print() {
    std::cout << id << std::endl; 
}
