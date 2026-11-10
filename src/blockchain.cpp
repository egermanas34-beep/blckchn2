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
   
   for(const auto& user : users)
   {
    balances[user.getPublicKey()] = user.getBalance();
    initialBalances[user.getPublicKey()] = user.getBalance();
}
    initializeTransactionPool(transactions);
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
    uint32_t version = 2;
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
 vector<Transaction> Blockchain::selectTransactions(uint32_t seed) const 
 {
     vector<Transaction> selectedTransactions;
    selectedTransactions.reserve(100);

    // Kiekvienas kandidatas turi savo laikina busena
    unordered_map<string, int64_t> tempBalances = balances;

    // Vietoje transakciju kopijavimo kuriame indeksus
    vector<size_t> indices(transactionPool.size());

    std::iota(indices.begin(), indices.end(), 0);

    std::mt19937 gen(seed);

    std::shuffle(indices.begin(), indices.end(), gen);

    unordered_set<string> selectedIDs;

    for (size_t index : indices)
    {
        const Transaction& transaction = transactionPool[index];

        // Ta pati transakcija negali kartotis kandidate
        if (selectedIDs.find(transaction.getTransactionId())
            != selectedIDs.end())
        {
            continue;
        }

        // Patikriname pagal laikina balansa
        if (!isValidTransaction(transaction, tempBalances))
        {
            continue;
        }

        // Atnaujiname tik laikina kandidato busena
        tempBalances[transaction.getSender()] -= transaction.getAmount();
        tempBalances[transaction.getReceiver()] += transaction.getAmount();

        selectedTransactions.push_back(transaction);

        selectedIDs.insert(transaction.getTransactionId());

        if (selectedTransactions.size() == 100)
        {
            break;
        }
    }

    return selectedTransactions;
 }
 bool Blockchain::addBlock()
{
    const int candidateCount = 5;

    string prevHash = chain.back().calculateBlockHash();
    uint64_t timestamp = static_cast<uint64_t>(std::time(nullptr));
    uint32_t version = 2;

    vector<Block> candidates;
    candidates.reserve(candidateCount);

    // Sukuriame 5 kandidatus
    for (int i = 0; i < candidateCount; i++)
    {
        uint32_t seed = 42 + static_cast<uint32_t>(chain.size()) * 5 + i;

        vector<Transaction> selectedTransactions = selectTransactions(seed);

        if (selectedTransactions.empty())
        {
            return false;
        }

        candidates.emplace_back(
            prevHash, timestamp, version,
            difficulty, selectedTransactions
        );
    }

    vector<int> found(candidateCount, 0); // Sekame, kurie kandidatai rado tinkamą nonce
    vector<uint64_t> totalAttempts(candidateCount, 0); // Sekame, kiek bandymų kiekvienas kandidatas atliko
    vector<double> miningTimes(candidateCount, 0.0); // Sekame, kiek laiko užtruko kiekvienas kandidatas

    int winnerIndex = -1;

    // Pradedame matuoti bendra kasimo laika
    auto miningStart = std::chrono::steady_clock::now();

    // Kandidatu kasimas
    for (int round = 0; round < 8; round++) // 8 raundai, kiekviename raunde didiname biudžetą
    {
        uint64_t budget = 1000ULL << round; // Biudžetas didėja eksponentiškai: 1000, 2000, 4000, ..., 128000
        
        #pragma omp parallel for schedule(static) // Naudojame OpenMP, kad kandidatai būtų kasami lygiagrečiai
        for (int i = 0; i < candidateCount; i++)
        {
            if (found[i])
            {
                continue;
            }

            uint64_t attempts = 0;

            auto start = std::chrono::steady_clock::now();

            bool success = candidates[i].mineBlockWithLimit(budget, attempts); // Bandome iškasti kandidatą su biudžetu

            auto end = std::chrono::steady_clock::now();

            std::chrono::duration<double> duration = end - start;

            found[i] = success ? 1 : 0; // Pažymime, ar kandidatas rado tinkamą nonce
            totalAttempts[i] += attempts;
            miningTimes[i] += duration.count();
        }

        // Ieskom laimetojo tarp sekmingu kandidatu
        for (int i = 0; i < candidateCount; i++)
        {
            if (found[i] &&
                (winnerIndex == -1 || totalAttempts[i] < totalAttempts[winnerIndex]))
            {
                winnerIndex = i;
            }
        }

        if (winnerIndex != -1)
        {
            break;
        }
    }
    auto miningEnd = std::chrono::steady_clock::now();
    std::chrono::duration<double> totalMiningDuration = miningEnd - miningStart;
    cout << "Bendra kasimo trukme: " << totalMiningDuration.count() << " s" << endl;
    // Kandidatu rezultatai
    for (int i = 0; i < candidateCount; i++)
    {
        cout << "Kandidatas " << i + 1
             << " | Bandymai: " << totalAttempts[i]
             << " | Laikas: " << miningTimes[i] << " s"
             << " | Rastas: " << (found[i] ? "TAIP" : "NE")
             << endl;
    }

    if (winnerIndex == -1)
    {
        cout << "Nepavyko rasti tinkamo bloko!" << endl;
        return false;
    }

    // Paimame laimejusi kandidata
    const Block& newBlock = candidates[winnerIndex];

    unordered_map<string, int64_t> updatedBalances; // Naujas balansų žemėlapis, kuris bus naudojamas patikrinimui

    if (!isValidBlock(newBlock, updatedBalances))
    {
        return false;
    }

    // Patvirtiname tik laimetojo bloka
    chain.push_back(newBlock);
    balances = std::move(updatedBalances);

    for (const auto& transaction : newBlock.getTransactions())
    {
        confirmedTransactionIds.insert(transaction.getTransactionId());
    }

    // Atnaujiname transakciju fonda
    vector<Transaction> remainingTransactions;
    remainingTransactions.reserve(transactionPool.size());

    for (const auto& transaction : transactionPool)
    {
        if (confirmedTransactionIds.find(transaction.getTransactionId()) ==confirmedTransactionIds.end())
        {
            remainingTransactions.push_back(transaction);
        }
    }

    transactionPool = std::move(remainingTransactions);

    cout << "Laimejo kandidatas: " << winnerIndex + 1 << endl;
    cout << "Bloko hash: " << newBlock.calculateBlockHash() << endl;

    return true;
}
 bool Blockchain::isValidBlock(const Block& block, unordered_map<string, int64_t>& updatedBalances) const
 {
    if(block.getPrevHash() != chain.back().calculateBlockHash())
    {
        cout << "Klaida: neteisingas prev_hash!" << endl;
        return false;
    }
    if(block.getVersion() != 2)
    {
        cout << "Klaida: neteisingas versijos numeris!" << endl;
        return false;
    }
    if(block.getDifficulty() != difficulty)
    {
        cout << "Klaida: neteisingas sunkumo lygis!" << endl;
        return false;
    }
    if(block.getTransactions().empty() || block.getTransactions().size() > 100)
    {
        cout << "Klaida: neteisingas transakcijų skaičius!" << endl;
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
            cout << "Klaida: netinkama transakcija! ID: "<< transaction.getTransactionId() << endl;
            return false;
        }
        transactionIdsInBlock.insert(transaction.getTransactionId());
        tempBalances[transaction.getSender()] -= transaction.getAmount();
        tempBalances[transaction.getReceiver()] += transaction.getAmount();
    }
    /*string concatenatedHashes;
    for(const auto& transaction : block.getTransactions())
    {
        concatenatedHashes += transaction.getTransactionId();
    }   
    auto hash = gautiHash(concatenatedHashes);
    string calculatedTxRoot = hashIString(hash);*/
    string calculatedTxRoot = calculateMerkleRoot(block.getTransactions());
    if(calculatedTxRoot != block.getTxRoot())
    {
        cout << "Klaida: neteisingas tx_root!" << endl;
    cout << "Apskaiciuotas: " << calculatedTxRoot << endl;
    cout << "Bloke saugomas: " << block.getTxRoot() << endl;
        return false;
    }
    string blockHash = block.calculateBlockHash();

    if (blockHash.substr(0, difficulty) != string(difficulty, '0'))
    {
         cout << "Klaida: neteisingas PoW!" << endl;
        return false;
    }
    updatedBalances = std::move(tempBalances);
    return true;
 }
bool Blockchain::verifyBalances() const
{
    uint64_t InitialBalanceSum = 0;
    uint64_t FinalBalanceSum = 0;
    if(initialBalances.size() != balances.size())
    {
        return false;
    }
    for(const auto& [publicKey, balance] : initialBalances)
    {
        if(balance < 0)
        {
            return false;
        }
        InitialBalanceSum += balance;
    }
    for(const auto& [publicKey, balance] : balances)
    {
        if(balance < 0)
        {
            return false;
        }
        if(initialBalances.find(publicKey) == initialBalances.end())
        {
            return false;
        }
        FinalBalanceSum += balance;
    }
    return InitialBalanceSum == FinalBalanceSum;
}
size_t Blockchain::getConfirmedTransactionCount() const
{
    return confirmedTransactionIds.size();
}
size_t Blockchain::getRemainingTransactionCount() const
{
    return transactionPool.size();
}
void Blockchain::initializeTransactionPool(const vector<Transaction>& transactions)
{
    std::unordered_set<string> poolIDs;
    for( const auto& transaction : transactions)
    {   
        // 1. Patikrinti, ar siuntejas egzistuoja
        if (balances.find(transaction.getSender()) == balances.end())
        {
            rejectedTransactions.emplace_back(
                transaction.getTransactionId(),
                "Nezinomas siuntejas"
            );
            continue;
        }

        // 2. Patikrinti, ar gavejas egzistuoja
        if (balances.find(transaction.getReceiver()) == balances.end())
        {
            rejectedTransactions.emplace_back(
                transaction.getTransactionId(),
                "Nezinomas gavejas"
            );
            continue;
        }

        // 3. Patikrinti, ar dalyviai skirtingi
        if (transaction.getSender() == transaction.getReceiver())
        {
            rejectedTransactions.emplace_back(
                transaction.getTransactionId(),
                "Siuntejas ir gavejas yra tas pats"
            );
            continue;
        }

        // 4. Patikrinti transakcijos suma
        if (transaction.getAmount() <= 0)
        {
            rejectedTransactions.emplace_back(
                transaction.getTransactionId(),
                "Neteisinga transakcijos suma"
            );
            continue;
        }

        // 5. Perskaiciuoti transakcijos ID
        string data = transaction.getSender() + "|" +
                      transaction.getReceiver() + "|" +
                      std::to_string(transaction.getAmount()) + "|" +
                      std::to_string(transaction.getSequence());

        auto hash = gautiHash(data);
        string calculatedTransactionId = hashIString(hash);

        if (calculatedTransactionId != transaction.getTransactionId())
        {
            rejectedTransactions.emplace_back(
                transaction.getTransactionId(),
                "Neteisingas transakcijos ID"
            );
            continue;
        }

        // 6. Patikrinti ID pasikartojima
        if (poolIDs.find(transaction.getTransactionId()) != poolIDs.end() ||
            confirmedTransactionIds.find(transaction.getTransactionId()) !=
                confirmedTransactionIds.end())
        {
            rejectedTransactions.emplace_back(
                transaction.getTransactionId(),
                "Dubliuotas ID"
            );
            continue;
        }

        // 7. Prideti tinkama transakcija i fonda
        transactionPool.push_back(transaction);
        poolIDs.insert(transaction.getTransactionId());
    }
}
size_t Blockchain::getRejectedTransactionCount() const
{
    return rejectedTransactions.size();
}
const vector<std::pair<string, string>>& Blockchain::getRejectedTransactions() const
{
    return rejectedTransactions;
}
int64_t Blockchain::getInitialBalanceSum() const
{
    int64_t sum = 0;

    for (const auto& [key, balance] : initialBalances)
        sum += balance;

    return sum;
}

int64_t Blockchain::getFinalBalanceSum() const
{
    int64_t sum = 0;

    for (const auto& [key, balance] : balances)
        sum += balance;

    return sum;
}
bool Blockchain::validateBlock(const Block& block) const
{
    unordered_map<string, int64_t> updatedBalances;
    return isValidBlock(block, updatedBalances);
}
void Blockchain::benchmarkTransactionSelection() const
{
    const int runs = 5;
    const int rounds = 10;
    const int candidates = 5;

    // v0.2 transakciju atrankos metodas
    auto selectV02 = [&](uint32_t seed)
    {
        vector<Transaction> shuffledTransactions = transactionPool;

        unordered_map<string, int64_t> tempBalances = balances;
        unordered_set<string> selectedIDs;

        vector<Transaction> selectedTransactions;
        selectedTransactions.reserve(100);

        std::mt19937 gen(seed);

        std::shuffle(
            shuffledTransactions.begin(),
            shuffledTransactions.end(),
            gen
        );

        for (const auto& transaction : shuffledTransactions)
        {
            if (selectedIDs.find(transaction.getTransactionId())
                != selectedIDs.end())
            {
                continue;
            }

            if (!isValidTransaction(transaction, tempBalances))
            {
                continue;
            }

            tempBalances[transaction.getSender()] -= transaction.getAmount();
            tempBalances[transaction.getReceiver()] += transaction.getAmount();

            selectedTransactions.push_back(transaction);
            selectedIDs.insert(transaction.getTransactionId());

            if (selectedTransactions.size() == 100)
            {
                break;
            }
        }

        return selectedTransactions;
    };

    // Patikriname, ar abu metodai atrenka tas pacias transakcijas
    for (int i = 0; i < rounds * candidates; i++)
    {
        uint32_t seed = 42 + i;

        auto oldSelected = selectV02(seed);
        auto newSelected = selectTransactions(seed);

        if (oldSelected.size() != newSelected.size())
        {
            throw std::runtime_error("Skiriasi atrinktu transakciju kiekis!");
        }

        for (size_t j = 0; j < oldSelected.size(); j++)
        {
            if (oldSelected[j].getTransactionId() !=
                newSelected[j].getTransactionId())
            {
                throw std::runtime_error("Skiriasi transakciju atrankos tvarka!");
            }
        }
    }

    // Ismatuojame pasirinkto metodo vykdymo laika
    auto measure = [&](bool optimized)
    {
        size_t totalSelected = 0;

        auto start = std::chrono::steady_clock::now();

        for (int round = 0; round < rounds; round++)
        {
            for (int candidate = 0; candidate < candidates; candidate++)
            {
                uint32_t seed = 42 + round * candidates + candidate;

                vector<Transaction> selected =
                    optimized
                    ? selectTransactions(seed)
                    : selectV02(seed);

                totalSelected += selected.size();
            }
        }

        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double> duration = end - start;

        return std::make_pair(duration.count(), totalSelected);
    };

    vector<double> timesV02, timesV03;

    cout << std::fixed << std::setprecision(6);

    cout << "\n===== TRANSAKCIJU ATRANKOS NASUMO TESTAS =====\n";
    cout << "Fondo dydis: " << transactionPool.size() << endl;
    cout << "Atranku per matavima: " << rounds * candidates << endl;

    for (int i = 0; i < runs; i++)
    {
        std::pair<double, size_t> oldResult, newResult;

        // Keiciame testu vykdymo tvarka
        if (i % 2 == 0)
        {
            oldResult = measure(false);
            newResult = measure(true);
        }
        else
        {
            newResult = measure(true);
            oldResult = measure(false);
        }

        if (oldResult.second != newResult.second)
        {
            throw std::runtime_error("Nesutampa atrinktu transakciju kiekiai!");
        }

        timesV02.push_back(oldResult.first);
        timesV03.push_back(newResult.first);

        cout << "Bandymas " << i + 1
             << " | v0.2: " << oldResult.first << " s"
             << " | v0.3: " << newResult.first << " s\n";
    }

    auto average = [](const vector<double>& times)
    {
        return std::accumulate(times.begin(), times.end(), 0.0)
               / times.size();
    };

    auto standardDeviation = [](const vector<double>& times, double avg)
    {
        double sum = 0.0;

        for (double time : times)
        {
            sum += (time - avg) * (time - avg);
        }

        return std::sqrt(sum / (times.size() - 1));
    };

    double avgV02 = average(timesV02);
    double avgV03 = average(timesV03);

    double improvement = (avgV02 - avgV03) / avgV02 * 100.0;

    cout << "\n===== REZULTATAI =====\n";

    cout << "v0.2 vidurkis: " << avgV02 << " s\n";
    cout << "v0.3 vidurkis: " << avgV03 << " s\n";

    cout << "v0.2 standartinis nuokrypis: "
         << standardDeviation(timesV02, avgV02) << " s\n";

    cout << "v0.3 standartinis nuokrypis: "
         << standardDeviation(timesV03, avgV03) << " s\n";

    cout << "Pagreitejimas: " << improvement << " %\n";
    cout << "Atrankos rezultatu patikra: SEKMINGA\n";
}
void Blockchain::benchmarkParallelMining()
{
    const int candidateCount = 5;
    const int repetitions = 10;
    const vector<int> threadCounts = {1, 2, 5};

    omp_set_dynamic(0);

    string prevHash = chain.back().calculateBlockHash();
    uint64_t timestamp = static_cast<uint64_t>(std::time(nullptr));
    uint32_t version = 2;

    // Sukuriame tuos pacius 5 kandidatus visiems testams
    vector<Block> baseCandidates;
    baseCandidates.reserve(candidateCount);

    for (int i = 0; i < candidateCount; i++)
    {
        uint32_t seed =
            42 + static_cast<uint32_t>(chain.size()) * 5 + i;

        vector<Transaction> selectedTransactions =
            selectTransactions(seed);

        if (selectedTransactions.empty())
        {
            cout << "Nepavyko sukurti kandidatu!" << endl;
            return;
        }

        baseCandidates.emplace_back(
            prevHash, timestamp, version,
            difficulty, selectedTransactions
        );
    }

    struct RunResult
    {
        double seconds;
        int winnerIndex;
        string blockHash;
    };

    // Vienas kasimo bandymas su nurodytu giju skaiciumi
    auto runMining = [&](int threadCount) -> RunResult
    {
        // Nauja kopija, kad kiekvienas testas prasidetu
        // nuo tos pacios nonce reiksmes
        vector<Block> candidates = baseCandidates;

        vector<int> found(candidateCount, 0);
        vector<uint64_t> totalAttempts(candidateCount, 0);

        int winnerIndex = -1;

        auto start = std::chrono::steady_clock::now();

        for (int round = 0; round < 8; round++)
        {
            uint64_t budget = 1000ULL << round;

            // Vieno kandidato kasimas
            auto mineCandidate = [&](int i)
            {
                if (found[i])
                {
                    return;
                }

                uint64_t attempts = 0;

                bool success = candidates[i].mineBlockWithLimit(
                    budget, attempts
                );

                found[i] = success ? 1 : 0;
                totalAttempts[i] += attempts;
            };

            if (threadCount == 1)
            {
                // Nuoseklus kasimas be OpenMP
                for (int i = 0; i < candidateCount; i++)
                {
                    mineCandidate(i);
                }
            }
            else
            {
                // Lygiagretus kasimas su OpenMP
                #pragma omp parallel for num_threads(threadCount) schedule(static)
                for (int i = 0; i < candidateCount; i++)
                {
                    mineCandidate(i);
                }
            }

            // Laimetoja renkame tik uzbaigus visas gijas
            for (int i = 0; i < candidateCount; i++)
            {
                if (found[i] &&
                    (winnerIndex == -1 ||
                     totalAttempts[i] < totalAttempts[winnerIndex]))
                {
                    winnerIndex = i;
                }
            }

            if (winnerIndex != -1)
            {
                break;
            }
        }

        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double> duration = end - start;

        if (winnerIndex == -1)
        {
            return {duration.count(), -1, ""};
        }

        return {
            duration.count(),
            winnerIndex,
            candidates[winnerIndex].calculateBlockHash()
        };
    };

    // Apsildymo bandymai
    for (int threads : threadCounts)
    {
        RunResult warmup = runMining(threads);

        if (warmup.winnerIndex == -1)
        {
            cout << "Apsildymo metu nepavyko iskasti bloko!" << endl;
            return;
        }
    }

    vector<vector<double>> times(threadCounts.size());

    int referenceWinner = -1;
    string referenceHash;

    cout << "\n=== OpenMP nasumo testas ===" << endl;
    cout << "Difficulty: " << difficulty << endl;
    cout << "Kandidatai: " << candidateCount << endl;
    cout << "Matavimai: " << repetitions << endl;

    // Kartojame bandymus, keisdami giju skaiciaus tvarka
    for (int rep = 0; rep < repetitions; rep++)
    {
        cout << "\nBandymas " << rep + 1 << ":" << endl;

        for (int j = 0; j < static_cast<int>(threadCounts.size()); j++)
        {
            int index = (rep + j) % threadCounts.size();
            int threads = threadCounts[index];

            RunResult result = runMining(threads);

            if (result.winnerIndex == -1)
            {
                cout << "Kasimas nepavyko!" << endl;
                return;
            }

            // Patikriname, ar rezultatai sutampa
            if (referenceWinner == -1)
            {
                referenceWinner = result.winnerIndex;
                referenceHash = result.blockHash;
            }
            else if (result.winnerIndex != referenceWinner ||
                     result.blockHash != referenceHash)
            {
                cout << "KLAIDA: Kasimo rezultatai nesutampa!" << endl;
                return;
            }

            times[index].push_back(result.seconds);

            cout << threads << " gijos: "
                 << std::fixed << std::setprecision(6)
                 << result.seconds << " s" << endl;
        }
    }

    // Vidurkio skaiciavimas
    auto calculateMean = [](const vector<double>& values)
    {
        double sum = std::accumulate(
            values.begin(), values.end(), 0.0
        );

        return sum / values.size();
    };

    // Imties standartinio nuokrypio skaiciavimas
    auto calculateStdDev = [&](const vector<double>& values)
    {
        double mean = calculateMean(values);
        double sum = 0.0;

        for (double value : values)
        {
            sum += (value - mean) * (value - mean);
        }

        return std::sqrt(sum / (values.size() - 1));
    };

    double sequentialMean = calculateMean(times[0]);

    cout << "\n=== GALUTINIAI REZULTATAI ===" << endl;

    for (int i = 0; i < static_cast<int>(threadCounts.size()); i++)
    {
        double mean = calculateMean(times[i]);
        double stddev = calculateStdDev(times[i]);
        double speedup = sequentialMean / mean;

        cout << "\nGijos: " << threadCounts[i] << endl;
        cout << "Vidutinis laikas: " << mean << " s" << endl;
        cout << "Standartinis nuokrypis: " << stddev << " s" << endl;
        cout << "Pagreitejimas: " << speedup << "x" << endl;
    }

    cout << "\nVisu testu laimetojas ir bloko hash sutapo!"
         << endl;
}