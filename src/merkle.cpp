#include "merkle.h"
#include "hash.h"
string hexToBytes(const string& hex)
{
    if(hex.size() != 64)
    {
        throw std::invalid_argument("Invalid hex string length");
    }
    auto hexValue = [](char c) {
        if(c >= '0' && c <= '9') return c - '0';// Convert hex character to its integer value
        if(c >= 'a' && c <= 'f') return c - 'a' + 10;
        if(c >= 'A' && c <= 'F') return c - 'A' + 10;
        throw std::invalid_argument("Invalid hex character");
    };
    string bytes;
    bytes.reserve(hex.size() / 2);
    for(size_t i = 0; i < hex.size(); i +=2)
    {
        int value = hexValue(hex[i]) * 16 + hexValue(hex[i + 1]);// Convert two hex characters to a byte
        bytes.push_back(static_cast<char>(value));
    }
    return bytes;
}
string calculateMerkleRoot(const vector<Transaction>& transactions)
{
    if(transactions.empty())
    {
        return string(64, '0');
    }

    vector<string> leafHashes; // vektorius, kuriame bus laikomi transakciju hash'ai
    leafHashes.reserve(transactions.size());
    for(const auto& tx : transactions)
    {
        leafHashes.push_back(tx.getTransactionId());
    }
    while(leafHashes.size() > 1)
    {
        if(leafHashes.size() % 2 != 0)
        {
            leafHashes.push_back(leafHashes.back()); // jeigu nelyginis, dubliuojame paskutini
        }
        vector<string> newHashes; // naujas vektorius, kuriame bus laikomi sujungti hash'ai
        for(size_t i = 0; i < leafHashes.size(); i += 2)
        {
            string combined = leafHashes[i] + leafHashes[i + 1];
            auto hash = gautiHash(combined);
            
            newHashes.push_back(hashIString(hash));
        }
        leafHashes = move(newHashes); // perkeliam naujus hash'us i leafHashes
    }
    return leafHashes.front(); // graziname galutini Merkle root hash'a
}