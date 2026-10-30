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
    if(balances.at(transaction.getReceiver()) > (std::numeric_limits<int64_t>::max)() - transaction.getAmount())
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
    std::unordered_set<string> selectedIDs;
    for(const auto& transaction : shuffledTransactions)
    {
        if(selectedTransactions.size() >= 100)
        {
            break;
        }
        if(selectedIDs.find(transaction.getTransactionId()) != selectedIDs.end())
        {
            continue;
        }
        selectedIDs.insert(transaction.getTransactionId());
        if(isValidTransaction(transaction, tempBalances))
        {
            selectedTransactions.push_back(transaction);
            tempBalances[transaction.getSender()] -= transaction.getAmount();
            tempBalances[transaction.getReceiver()] += transaction.getAmount();
        }
    }


    return selectedTransactions;
 }
 bool Blockchain::addBlock()
 {
    vector<Transaction> selectedTransactions = selectTransactions();
    if(selectedTransactions.empty())
    {
        return false;
    }
    string prevHash = chain.back().calculateBlockHash();
    uint64_t timestamp = static_cast<uint64_t>(std::time(nullptr));
    uint32_t version = 1;
    Block newBlock(prevHash, timestamp, version, difficulty, selectedTransactions);
    newBlock.mineBlock();
    unordered_map<string, int64_t> updatedBalances;
    if(!isValidBlock(newBlock, updatedBalances))
    {
        return false;
    }
    chain.push_back(newBlock);
    balances = std::move(updatedBalances);
    for(const auto& transaction : selectedTransactions)
    {
        confirmedTransactionIds.insert(transaction.getTransactionId());
    }
    vector<Transaction> remainingTransactions;
    remainingTransactions.reserve(transactionPool.size());
    for(const auto& transaction : transactionPool)
    {
        if(confirmedTransactionIds.find(transaction.getTransactionId()) == confirmedTransactionIds.end())
        {
            remainingTransactions.push_back(transaction);
        }
    }
    transactionPool = std::move(remainingTransactions);
    return true;
 }
 bool Blockchain::isValidBlock(const Block& block, unordered_map<string, int64_t>& updatedBalances) const
 {
    if(block.getPrevHash() != chain.back().calculateBlockHash())
    {
        return false;
    }
    if(block.getVersion() != 1)
    {
        return false;
    }
    if(block.getDifficulty() != difficulty)
    {
        return false;
    }
    if(block.getTransactions().empty() || block.getTransactions().size() > 100)
    {
        return false;
    }
    auto tempBalances = balances;
    unordered_set<string> transactionIdsInBlock;
    for(const auto& transaction : block.getTransactions())
    {
        if(transactionIdsInBlock.find(transaction.getTransactionId()) != transactionIdsInBlock.end())
        {
            return false;
        }
        if(!isValidTransaction(transaction, tempBalances))
        {
            return false;
        }
        transactionIdsInBlock.insert(transaction.getTransactionId());
        tempBalances[transaction.getSender()] -= transaction.getAmount();
        tempBalances[transaction.getReceiver()] += transaction.getAmount();
    }
    string concatenatedHashes;
    for(const auto& transaction : block.getTransactions())
    {
        concatenatedHashes += transaction.getTransactionId();
    }   
    auto hash = gautiHash(concatenatedHashes);
    string calculatedTxRoot = hashIString(hash);
    if(calculatedTxRoot != block.getTxRoot())
    {
        return false;
    }
    string blockHash = block.calculateBlockHash();

    if (blockHash.substr(0, difficulty) != string(difficulty, '0'))
    {
        return false;
    }
    updatedBalances = std::move(tempBalances);
    return true;
 }