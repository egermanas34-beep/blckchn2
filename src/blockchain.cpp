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
bool Blockchain::isValidTransaction(const Transaction& transaction, const unordered_map<string, int64_t>& balances) const
{
    if(balances.find(transaction.getSender()) == balances.end())
    {
        return false;
    }
    if(balances.find(transaction.getReceiver()) == balances.end())
    {
        return false;
    }
    if(transaction.getSender() == transaction.getReceiver())
    {
        return false;
    }
    if(transaction.getAmount() <= 0)
    {
        return false;
    }
    if(balances.at(transaction.getSender()) < transaction.getAmount())
    {
        return false;
    }
    string data = transaction.getSender() + "|" + transaction.getReceiver() + "|" + std::to_string(transaction.getAmount()) + "|" + std::to_string(transaction.getSequence());
    auto hash = gautiHash(data);
    string calculatedTransactionId = hashIString(hash);
    if(calculatedTransactionId != transaction.getTransactionId())
    {
        return false;
    }
    
    if(confirmedTransactionIds.find(transaction.getTransactionId()) != confirmedTransactionIds.end())
    {
        return false;
    }
    return true;
}
 vector<Transaction> Blockchain::selectTransactions() const 
 {
    vector<Transaction> selectedTransactions;
    selectedTransactions.reserve(100);
    auto tempBalances = balances;
    std::mt19937 gen(42);
    vector<Transaction> shuffledTransactions = transactionPool;
    std::shuffle(shuffledTransactions.begin(), shuffledTransactions.end(), gen);
    for(const auto& transaction : shuffledTransactions)
    {
        if(selectedTransactions.size() >= 100)
        {
            break;
        }
        if(isValidTransaction(transaction, tempBalances))
        {
            selectedTransactions.push_back(transaction);
            tempBalances[transaction.getSender()] -= transaction.getAmount();
            tempBalances[transaction.getReceiver()] += transaction.getAmount();
        }
    }


    return selectedTransactions;
 }