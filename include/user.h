#pragma once
#include "include.h"

class User{
    private:
        string name;
        string publicKey;
        uint32_t balance;

    public:
        User();
        ~User();
        string getName();
        string getPublicKey();
        uint32_t getBalance();
        void setName(string name);
        void setPublicKey(string publicKey);
        void setBalance(uint32_t balance);
};