#pragma once

#include "include.h"
#include "transaction.h"

string calculateMerkleRoot(const vector<Transaction>& transactions);
string hexToBytes(const string& hex);