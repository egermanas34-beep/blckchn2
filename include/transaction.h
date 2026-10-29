#pragma once
#include "include.h"

class Transaction {
private:
    string sender;
    string receiver;
    int64_t amount;
    uint64_t sequence;
    string transaction_id;
public:
    Transaction();
    Transaction(string sender, string receiver, int64_t amount, uint64_t sequence);
    ~Transaction();
    string getSender() const;
    string getReceiver() const;
    int64_t getAmount() const;
    uint64_t getSequence() const;
    string getTransactionId() const;
    
};
