#include "../include/utils.hpp"
#include <openssl/sha.h>
#include <iomanip>
#include <fstream>
#include "big_int.cpp"
#include <openssl/evp.h>

template<typename T>
Stack<T>::Stack() {}

template<typename T>
void Stack<T>::push(T* node) {
    items.push_back(node);
}

template<typename T>
T* Stack<T>::pop() {
    if (!isEmpty()) {
        T* temp = items[items.size()-1];
        items.pop_back();
        return temp;
    }
    return nullptr;
}

template<typename T>
bool Stack<T>::isEmpty() {
    return items.size() == 0;
}

template<typename T>
T* Stack<T>::peek() {
    if (!isEmpty())  return items[items.size()-1];
    return nullptr;
}

std::string sha256(std::string text)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, text.c_str(), text.size());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return ss.str();
}

std::string sha1(std::string text) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA_CTX sha1;
    SHA1_Init(&sha1);
    SHA1_Update(&sha1, text.c_str(), text.size());
    SHA1_Final(hash, &sha1);

    std::stringstream ss;
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return ss.str();
}

std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); 
}

BigInt bigPow(BigInt base, BigInt exponent) {
	BigInt total("1");
	for (int i = 0; i < exponent; i++)
		total *= base;
	return total;
}

BigInt bigHexor(std::string hex_str1, std::string hex_str2) {
    std::string bin_str1;
	for (int i = 0; i < hex_str1.size(); i+=2) 
		bin_str1 += std::bitset<8>(stoi(hex_str1.substr(i, 2), 0, 16)).to_string();

    std::string bin_str2;
	for (int i = 0; i < hex_str2.size(); i+=2) 
		bin_str2 += std::bitset<8>(stoi(hex_str2.substr(i, 2), 0, 16)).to_string();

    BigInt result("0");
	BigInt x(bin_str1.size());
    BigInt two("2");
    for (int i = 0; i < bin_str1.size(); i++) {    
        BigInt big_num;   
        const char* num1 = reinterpret_cast<const char *>(bin_str1[i]);
        const char* num2 = reinterpret_cast<const char *>(bin_str2[i]);
        ((num1 == "0" && num2 == "0")||(num1 == "1" && num2 == "1")) ? (big_num = "0") : (big_num = "1");
        result += big_num * bigPow(two, x);
        x--;
    }
	return result;
}

BigInt hexToInt(std::string hex_str) {
    BigInt result("0");
    BigInt two("2");
    long bit_count = hex_str.size()*4-1;
	std::string bin_str;
	for (int i = 0; i < hex_str.size(); i+=2) {
        for (auto &bit : std::bitset<8>(stoi(hex_str.substr(i, 2), 0, 16)).to_string()) {
            BigInt big_num(bin_str[i]);
            BigInt x(bit_count);
            result += big_num * bigPow(two, x);
            bit_count--;
        } 
	}
	return result;
}

char* base64_encode(const char *input) {
  const auto pl = 4*((strlen(input)+2)/3);
  auto output = reinterpret_cast<char *>(calloc(pl+1, 1)); //+1 for the terminating null that EVP_EncodeBlock adds on
  const auto ol = EVP_EncodeBlock(reinterpret_cast<unsigned char *>(output), reinterpret_cast<const unsigned char *>(input), strlen(input));
  if (pl != ol) { std::cerr << "Whoops, encode predicted " << pl << " but we got " << ol << "\n"; }
  return output;
}

char* base64_decode(const char *input) {
  const auto pl = 3*strlen(input)/4;
  auto output = reinterpret_cast<unsigned char *>(calloc(pl+1, 1));
  const auto ol = EVP_DecodeBlock(output, reinterpret_cast<const unsigned char *>(input), strlen(input));
  if (pl != ol) { std::cerr << "Whoops, decode predicted " << pl << " but we got " << ol << "\n"; }
  return reinterpret_cast<char *>(output);
}

void joinMessage(std::string peer_id) {
    std::cout << "\n" << peer_id << " joined!\n>> ";
}
