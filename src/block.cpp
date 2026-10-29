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
    calculateTXRoot();
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
void Block::calculateTXRoot(){
   string concatenatedHashes;
        
  for(const auto& transaction : transactions) 
  {
    concatenatedHashes += transaction.getTransactionId();
  }
    auto hash = gautiHash(concatenatedHashes);
     tx_root = hashIString(hash);
}
string Block::calculateBlockHash() const
{
    string blockData = prev_hash + "|" + std::to_string(timestamp) + "|" + std::to_string(version) + "|" + tx_root + "|" + std::to_string(nonce) + "|" + std::to_string(difficulty);
    auto hash = gautiHash(blockData);
    return hashIString(hash);
}
void Block::mineBlock()
{
    nonce = 0;
    uint64_t attempts = 0;
    auto start = std::chrono::high_resolution_clock::now();
    while(true)
    {
        string blockHash = calculateBlockHash();
        attempts++;
        if(blockHash.substr(0,difficulty) == string(difficulty, '0'))
        {
            break;
        }
        nonce++;
        if(nonce == UINT64_MAX)
        {
            throw std::overflow_error("Nonce pasiekė maksimalia reikšmę!");
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Blokas iškaltas! Nonce: " << nonce <<endl;
    std::cout << "Bandymų skaičius: " << attempts << endl;
    std::cout << "Praėjęs laikas: " << elapsed.count() << " sekundžių"<< endl;
}