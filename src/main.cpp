#include "include.h"
#include "user.h"
#include "hash.h"

vector<User> generateUsers(int numUsers) {
   vector<User> users;
   users.reserve(numUsers);
   std::mt19937 gen(42);
   std::uniform_int_distribution<> dis(100, 1000000);
    for( int i = 0; i < numUsers; i++)
    {
        string name = "User" + std::to_string(i);
        auto hash = gautiHash(name);
        string publicKey = hashIString(hash);
        uint32_t balance = dis(gen);
        users.push_back(User(name, publicKey, balance));    
    }
   return users;
}

int main() {
    vector<User> users = generateUsers(10);
    for (const auto& user : users) {
        cout << "Name: " << user.getName() << ", Public Key: " << user.getPublicKey() << ", Balance: " << user.getBalance() << endl;
    }
}


