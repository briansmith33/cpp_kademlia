#include "../include/utils.hpp"
#include <openssl/sha.h>
#include <iomanip>
#include <fstream>
#include "big_int.cpp"

Stack::Stack() {}

void Stack::push(Peer* peer) {
    items.push_back(peer);
}

Peer* Stack::pop() {
    if (!isEmpty()) {
        Peer* temp = items[items.size()-1];
        items.pop_back();
        return temp;
    }
    return nullptr;
}

bool Stack::isEmpty() {
    return items.size() == 0;
}

Peer* Stack::peek() {
    if (!isEmpty()) {
        return items[items.size()-1];
    }
    return nullptr;
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

BigInt bigPow(BigInt base, BigInt exponent) {
	BigInt total("1");
	for (int i = 0; i < exponent; i++) {
		total *= base;
	}
	return total;
}

BigInt bigHexor(std::string hex_str1, std::string hex_str2) {
    std::string bin_str1;
	for (int i = 0; i < hex_str1.size(); i+=2) {
		bin_str1 += std::bitset<8>(stoi(hex_str1.substr(i, 2), 0, 16)).to_string();
	}
    std::string bin_str2;
	for (int i = 0; i < hex_str2.size(); i+=2) {
		bin_str2 += std::bitset<8>(stoi(hex_str2.substr(i, 2), 0, 16)).to_string();
	}
    std::string bin_str3;
    for (int i = 0; i < bin_str1.size(); i++) {        
        int num1;
		stringstream ss1; 
		ss1 << bin_str1[i];
		ss1 >> num1;

        int num2;
		stringstream ss2; 
		ss2 << bin_str2[i];
		ss2 >> num2;

        if ((num1 == 0 && num2 == 0)||(num1 == 1 && num2 == 1)) {
            bin_str3 += "0";
        } else {
            bin_str3 += "1";
        }
    }
    BigInt hash_int("0");
	BigInt x(bin_str3.size()-1);
	for (int i = 0; i < bin_str3.size()-1; i++) {
		int num;
		stringstream ss; 
		ss << bin_str3[i];
		ss >> num;
		BigInt big_num((char)num);
		BigInt two("2");
		
		hash_int += big_num * bigPow(two, x);
		x--;
	}
	return hash_int;
}

BigInt hexToInt(std::string hex_str) {
	std::string bin_str;
	for (int i = 0; i < hex_str.size(); i+=2) {
		bin_str += std::bitset<8>(stoi(hex_str.substr(i, 2), 0, 16)).to_string();
	}
	BigInt hash_int("0");
	BigInt x(bin_str.size()-1);
	for (int i = 0; i < bin_str.size()-1; i++) {
		int num;
		stringstream ss; 
		ss << bin_str[i];
		ss >> num;
		BigInt big_num((char)num);
		BigInt two("2");
		hash_int += big_num * bigPow(two, x);
		x--;
	}
	return hash_int;
}
