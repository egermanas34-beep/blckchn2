#include "transaction.h"
#include "hash.h"
Transaction::Transaction()
{
    sender = "";
    receiver = "";
    amount = 0;
    sequence = 0;
    transaction_id = "";
}
Transaction::Transaction(string sender, string receiver, int64_t amount, uint64_t sequence)
{
    this->sender = sender;
    this->receiver = receiver;
    this->amount = amount;
    this->sequence = sequence;
   string data = sender + "|" + receiver + "|" + std::to_string(amount) + "|" + std::to_string(sequence);
    auto hash = gautiHash(data);
    this->transaction_id = hashIString(hash);
}
Transaction::~Transaction()
{
    sender = "";
    receiver = "";
    amount = 0;
    sequence = 0;
    transaction_id = "";
}
string Transaction::getSender() const
{
    return this->sender;
}

string Transaction::getReceiver() const
{
    return this->receiver;
}

int64_t Transaction::getAmount() const
{
    return this->amount;
}

uint64_t Transaction::getSequence() const
{
    return this->sequence;
}

string Transaction::getTransactionId() const
{
    return this->transaction_id;
}
