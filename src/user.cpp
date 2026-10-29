#include "user.h"

User::User()
{
   name = "";
    publicKey = "";
    balance = 0;
}
User::User(string name, string publicKey, uint64_t balance)
{
    this->name = name;
    this->publicKey = publicKey;
    this->balance = balance;
}
User::~User()
{
    name = "";
    publicKey = "";
    balance = 0;
}
string User::getName() const
{
    return this->name;
}

string User::getPublicKey() const
{
    return this->publicKey;
}

uint64_t User::getBalance() const
{
    return this->balance;
}
void User::setName(string name)
{
    this->name = name;
}

void User::setPublicKey(string publicKey)
{
    this->publicKey = publicKey;
}

void User::setBalance(uint64_t balance)
{
    this->balance = balance;
}