#include "blockchain.h"

Blockchain::Blockchain() {
    chain = {};
    balances = {};
    initialBalances = {};
    transactionPool = {};
    difficulty = 0;
    confirmedTransactionIds = {};

}
Blockchain::Blockchain(const vector<User>& users,const vector<Transaction>& transactions, uint32_t difficulty) {
   this->difficulty = difficulty;
   transactionPool = transactions;
   for(const auto& user : users)
   {
    balances[user.getPublicKey()] = user.getBalance();
    initialBalances[user.getPublicKey()] = user.getBalance();
}
    GenesisBlock();

}
Blockchain::~Blockchain() {
    chain.clear();
    balances.clear();
    initialBalances.clear();
    transactionPool.clear();
    confirmedTransactionIds.clear();
}
void Blockchain::GenesisBlock() {
    string prevHash(64, '0');
    uint64_t timestamp = 0;
    uint32_t version = 1;
    uint32_t genesisDifficulty = 0;

    vector<Transaction> genesisTransactions;
    chain.emplace_back(prevHash, timestamp, version, genesisDifficulty, genesisTransactions);
}
const vector<Block>& Blockchain::getChain() const {
    return chain;
}