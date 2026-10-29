#include "block.h"

Block::Block(){
    
    prev_hash = "";
    timestamp = 0;
    version = 0;
    tx_root = "";
    nonce = 0;
    difficulty = 0;
    transactions = {};
}

Block::Block(string prev_hash, uint64_t timestamp, uint32_t version, uint32_t difficulty, const vector<Transaction>& transactions){
    this->prev_hash = prev_hash;
    this->timestamp = timestamp;
    this->version = version;
    this->tx_root = "";
    this->nonce = 0;
    this->difficulty = difficulty;
    this->transactions = transactions;
}

Block::~Block(){
prev_hash = "";
timestamp = 0;
version = 0;
tx_root = "";
nonce = 0;
difficulty = 0;
transactions.clear();
}

string Block::getPrevHash() const{
    return prev_hash;
}
uint64_t Block::getTimestamp() const{
    return timestamp;
}
uint32_t Block::getVersion() const{
    return version;
}
string Block::getTxRoot() const{
    return tx_root;
}
uint64_t Block::getNonce() const{
    return nonce;
}
uint32_t Block::getDifficulty() const{
    return difficulty;
}
const vector<Transaction>& Block::getTransactions() const{
    return transactions;
}
