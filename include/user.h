#pragma once
#include "include.h"

class User{
    private:
        string name;
        string publicKey;
        uint32_t balance;

    public:
        User();
        User(string name, string publicKey, uint32_t balance);
        ~User();
        string getName() const;
        string getPublicKey() const;
        uint32_t getBalance() const;
        void setName(string name);
        void setPublicKey(string publicKey);
        void setBalance(uint32_t balance);
};