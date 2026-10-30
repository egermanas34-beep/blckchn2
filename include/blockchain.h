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

    void GenesisBlock();
    bool isValidTransaction(const Transaction& transaction, const unordered_map<string, int64_t>& balances) const;
    public:
    Blockchain();
    Blockchain(const vector<User>& users,const vector<Transaction>& transactions, uint32_t difficulty);
    ~Blockchain();
    const vector<Block>& getChain() const;
};