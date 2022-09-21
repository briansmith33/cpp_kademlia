#include "../include/utils.hpp"
#include <openssl/sha.h>
#include <iomanip>
#include <fstream>
#include "big_int.cpp"

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
    if (!isEmpty()) {
        return items[items.size()-1];
    }
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
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

std::string sha1(std::string text) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA_CTX sha1;
    SHA1_Init(&sha1);
    SHA1_Update(&sha1, text.c_str(), text.size());
    SHA1_Final(hash, &sha1);

    std::stringstream ss;
    for(int i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
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
		std::stringstream ss1; 
		ss1 << bin_str1[i];
		ss1 >> num1;

        int num2;
		std::stringstream ss2; 
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
		std::stringstream ss; 
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
		std::stringstream ss; 
		ss << bin_str[i];
		ss >> num;
		BigInt big_num((char)num);
		BigInt two("2");
		hash_int += big_num * bigPow(two, x);
		x--;
	}
	return hash_int;
}


static const char b64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const char reverse_table[128] = {
   64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
   64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
   64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
   52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
   64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
   15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
   64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
   41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64
};

::std::string base64_encode(const ::std::string &bindata)
{
   using ::std::string;
   using ::std::numeric_limits;

   if (bindata.size() > (numeric_limits<string::size_type>::max() / 4u) * 3u) {
      throw ::std::length_error("Converting too large a string to base64.");
   }

   const ::std::size_t binlen = bindata.size();
   // Use = signs so the end is properly padded.
   string retval((((binlen + 2) / 3) * 4), '=');
   ::std::size_t outpos = 0;
   int bits_collected = 0;
   unsigned int accumulator = 0;
   const string::const_iterator binend = bindata.end();

   for (string::const_iterator i = bindata.begin(); i != binend; ++i) {
      accumulator = (accumulator << 8) | (*i & 0xffu);
      bits_collected += 8;
      while (bits_collected >= 6) {
         bits_collected -= 6;
         retval[outpos++] = b64_table[(accumulator >> bits_collected) & 0x3fu];
      }
   }
   if (bits_collected > 0) { // Any trailing bits that are missing.
      assert(bits_collected < 6);
      accumulator <<= 6 - bits_collected;
      retval[outpos++] = b64_table[accumulator & 0x3fu];
   }
   assert(outpos >= (retval.size() - 2));
   assert(outpos <= retval.size());
   return retval;
}

::std::string base64_decode(const ::std::string &ascdata)
{
   using ::std::string;
   string retval;
   const string::const_iterator last = ascdata.end();
   int bits_collected = 0;
   unsigned int accumulator = 0;

   for (string::const_iterator i = ascdata.begin(); i != last; ++i) {
      const int c = *i;
      if (::std::isspace(c) || c == '=') {
         // Skip whitespace and padding. Be liberal in what you accept.
         continue;
      }
      if ((c > 127) || (c < 0) || (reverse_table[c] > 63)) {
         throw ::std::invalid_argument("This contains characters not legal in a base64 encoded string.");
      }
      accumulator = (accumulator << 6) | reverse_table[c];
      bits_collected += 6;
      if (bits_collected >= 8) {
         bits_collected -= 8;
         retval += static_cast<char>((accumulator >> bits_collected) & 0xffu);
      }
   }
   return retval;
}

void joinMessage(std::string peer_id) {
    std::cout << "\n" << peer_id << " joined!\n>> ";
}
