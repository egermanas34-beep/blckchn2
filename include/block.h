#pragma once
#include "include.h"
#include "transaction.h"
#include "hash.h"
class Block {
    private:
    string prev_hash;
    uint64_t timestamp;
    uint32_t version;
    string tx_root;
    uint64_t nonce;
    uint32_t difficulty;
    vector<Transaction> transactions;
    public:
    Block();
    Block(string prev_hash, uint64_t timestamp, uint32_t version, uint32_t difficulty, const vector<Transaction>& transactions);
    ~Block();
    string getPrevHash() const;
    uint64_t getTimestamp() const;
    uint32_t getVersion() const;
    string getTxRoot() const;
    uint64_t getNonce() const;
    uint32_t getDifficulty() const;
    const vector<Transaction>& getTransactions() const;


    void calculateTXRoot();
    string calculateBlockHash() const;
};