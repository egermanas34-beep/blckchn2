#include "merkle.h"

string hexToBytes(const string& hex)
{
    if(hex.size() != 64)
    {
        throw std::invalid_argument("Invalid hex string length");
    }
    auto hexValue = [](char c) {
        if(c >= '0' && c <= '9') return c - '0';// Convert hex character to its integer value
        if(c >= 'a' && c <= 'f') return c - 'a' + 10;
        if(c >= 'A' && c <= 'F') return c - 'A' + 10;
        throw std::invalid_argument("Invalid hex character");
    };
    string bytes;
    bytes.reserve(hex.size() / 2);
    for(size_t i = 0; i < hex.size(); i +=2)
    {
        int value = hexValue(hex[i]) * 16 + hexValue(hex[i + 1]);// Convert two hex characters to a byte
        bytes.push_back(static_cast<char>(value));
    }
    return bytes;
}