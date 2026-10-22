#pragma once
#include "include.h"

class User{
    private:
        string name;
        string publicKey;
        uint32_t balance;

    public:
        User(string name, string publicKey, uint32_t balance);
        string getName();
        string getPublicKey();
        uint32_t getBalance();
};