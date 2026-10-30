#pragma once
#include "include.h"
#include "transaction.h"
#include "hash.h"
#include "block.h"
#include "user.h"

class Blockchain {
    private:
    vector<Block> chain;
    unordered_map<string, int64_t> balances;
    unordered_map<string, uint64_t> initialBalances;
    vector<Transaction> transactionPool;
    uint32_t difficulty;
    unordered_set<string> confirmedTransactionIds;
    vector<std::pair<string, string>> rejectedTransactions;

    void initializeTransactionPool(const vector<Transaction>& transactions);
    void GenesisBlock();
    bool isValidTransaction(const Transaction& transaction, const unordered_map<string, int64_t>& balances) const;
    bool isValidBlock(const Block& block, unordered_map<string, int64_t>& updatedBalances) const;

    public:
    Blockchain();
    Blockchain(const vector<User>& users,const vector<Transaction>& transactions, uint32_t difficulty);
    ~Blockchain();
    const vector<Block>& getChain() const;
    vector<Transaction> selectTransactions() const;
    bool addBlock();
    bool verifyBalances() const;
    size_t getConfirmedTransactionCount() const;
    size_t getRemainingTransactionCount() const;
    size_t getRejectedTransactionCount() const;
};