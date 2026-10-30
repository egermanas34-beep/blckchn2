#pragma once
#include "include.h"
uint32_t rotl32(uint32_t x, unsigned r);
std::array<uint32_t, 8> gautiHash(string t);
string hashIString(std::array<uint32_t, 8>& hash);
