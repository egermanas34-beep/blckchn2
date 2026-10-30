#include "include.h"
#include "user.h"
#include "hash.h"
#include "transaction.h"
#include "block.h"
#include "blockchain.h"

vector<User> generateUsers(int numUsers);
vector<Transaction> generateTransactions(const vector<User>& users, int numTransactions);
int main() {
    SetConsoleOutputCP(CP_UTF8); // Nustatome konsolės išvesties koduotę į UTF-8
    SetConsoleCP(CP_UTF8); // Nustatome konsolės įvesties koduotę į UTF-8


    vector<User> users = generateUsers(1000);
   /* for (int i = 0; i < 5; i++)
    {
        cout << "Vardas: " << users[i].getName() << "\n";
        cout << "Public key: " << users[i].getPublicKey() << "\n";
        cout << "Balansas: " << users[i].getBalance() << "\n\n";
    }
    */
    vector<Transaction> transactions = generateTransactions(users, 10000);
    /* for (int i = 0; i < 5; i++)
    {
        cout << "Siuntejas: " << transactions[i].getSender() << "\n";
        cout << "Gavejas: " << transactions[i].getReceiver() << "\n";
        cout << "Suma: " << transactions[i].getAmount() << "\n";
        cout << "Sekos numeris: " << transactions[i].getSequence() << "\n";
        cout << "Transakcijos ID: " << transactions[i].getTransactionId() << "\n\n";
    }
    */
   /*
   vector<Transaction> blockTransactions(transactions.begin(), transactions.begin() + 5);

   //sukursim bloka pirmoms 5 transakcijoms
    string prevHash(64, '0');
    uint64_t timestamp = 1761730000;
    uint32_t version = 1;
    uint32_t difficulty = 3;

    Block block(prevHash, timestamp, version, difficulty, blockTransactions);
    block.mineBlock();
    */
   /* Blockchain blockchain(users, transactions, 3);

    cout << "Bloku skaicius: "
         << blockchain.getChain().size() << endl;

    const Block& genesis = blockchain.getChain().front();

    cout << "Genesis prev_hash: "
         << genesis.getPrevHash() << endl;

    cout << "Genesis tx_root: "
         << genesis.getTxRoot() << endl;

    cout << "Genesis hash: "
         << genesis.calculateBlockHash() << endl;

    cout << "Genesis transakciju skaicius: "
         << genesis.getTransactions().size() << endl;
    */
    Blockchain blockchain(users, transactions, 3);

    cout << "Pradinis bloku skaicius: "
         << blockchain.getChain().size() << endl;

    if (blockchain.addBlock())
    {
        cout << "\nBlokas sekmingai pridetas!" << endl;
    }
    else
    {
        cout << "\nBloko prideti nepavyko!" << endl;
    }

    cout << "Galutinis bloku skaicius: "
         << blockchain.getChain().size() << endl;

    return 0;
}
vector<User> generateUsers(int numUsers) {
   vector<User> users;
   users.reserve(numUsers);
   std::mt19937 gen(42);
   std::uniform_int_distribution<> dis(100, 1000000);
    for( int i = 0; i < numUsers; i++)
    {
        string name = "User" + std::to_string(i+1);
        auto hash = gautiHash(name);
        string publicKey = hashIString(hash);
        uint64_t balance = dis(gen);
        users.push_back(User(name, publicKey, balance));    
    }
   return users;
}
vector<Transaction> generateTransactions(const vector<User>& users, int numTransactions) {
    if (numTransactions < 0 || users.size() < 2)
{
    throw std::invalid_argument("Neteisingi generavimo parametrai!");
}
    vector<Transaction> transactions;
    transactions.reserve(numTransactions);
    std::mt19937 gen(42);
    std::uniform_int_distribution<> dis(0, users.size() - 1);
    std::uniform_int_distribution<> amountDis(1, 1000);
    for( int i = 0; i < numTransactions; i++)
    {
        int senderIndex = dis(gen);
        int receiverIndex = dis(gen);
        while(receiverIndex == senderIndex)
        {receiverIndex = dis(gen);}
        string sender = users[senderIndex].getPublicKey();
        string receiver = users[receiverIndex].getPublicKey();
        int64_t amount = amountDis(gen);
        uint64_t sequence = i + 1;

        transactions.push_back(Transaction(sender, receiver, amount, sequence));
    }

    return transactions;
}

