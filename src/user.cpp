#include "user.h"

User::User()
{
   name = "";
    publicKey = "";
    balance = 0;
}
User::~User()
{
    name = "";
    publicKey = "";
    balance = 0;
}
string User::getName()
{
    return this->name;
}

string User::getPublicKey()
{
    return this->publicKey;
}

uint32_t User::getBalance()
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

void User::setBalance(uint32_t balance)
{
    this->balance = balance;
}