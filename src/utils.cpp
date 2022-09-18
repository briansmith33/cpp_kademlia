#include "../include/utils.hpp"
#include <openssl/sha.h>
#include <iomanip>
#include <fstream>

Stack::Stack() {}

void Stack::push(Peer peer) {
    items.push_back(peer);
}

Peer Stack::pop() {
    if (!isEmpty()) {
        Peer temp = items[items.size()-1];
        items.pop_back();
        return temp;
    }
    return Peer(-1);
}

bool Stack::isEmpty() {
    return items.size() == 0;
}

Peer Stack::peek() {
    if (!isEmpty()) {
        return items[items.size()-1];
    }
    return Peer(-1);
}

std::string sha256(const std::string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

std::string sha1(std::string text)  
{  
    const char *c = text.c_str();
    auto uchrs = reinterpret_cast<unsigned char*>(const_cast<char*>(c));

    unsigned char hash[SHA_DIGEST_LENGTH];  
  
    SHA1(uchrs, sizeof(uchrs) - 1, hash);  
  
    // do some stuff with the hash  
    //printf("raw: %s \n", hash);  
  
    char sha1string[SHA_DIGEST_LENGTH*2 +1];  
    for(int i = 0; i < SHA_DIGEST_LENGTH; ++i)  
    {  
        sprintf(&sha1string[i*2], "%02x", (unsigned int)hash[i]);  
    }  

    //printf("string: %s \n", sha1string);  
    std::string peer_id;
    for (int i = 0; i < sizeof(sha1string); i++) {
        peer_id += sha1string[i];
    }
    return peer_id;
} 

std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); 
}
