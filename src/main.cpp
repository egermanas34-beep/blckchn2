#include "include.h"
#include "user.h"
#include "hash.h"
#include "transaction.h"
#include "block.h"
#include "blockchain.h"
#include "merkle.h"


struct BenchmarkResult
{
    double time;
    uint64_t checksum;
};

template<bool optimized>
BenchmarkResult measurePoW(uint64_t iterations)
{
    const string prevHash(64, '0');
    const uint64_t timestamp = 1234567890;
    const uint32_t version = 2;
    const string txRoot(64, 'a');
    const uint32_t difficulty = 3;

    // v0.3 optimizavimui
    const string prefix =
        prevHash + "|" +
        std::to_string(timestamp) + "|" +
        std::to_string(version) + "|" +
        txRoot + "|";

    const string suffix = "|" + std::to_string(difficulty);

    string data = prefix;
    data.reserve(prefix.size() + 20 + suffix.size());

    uint64_t checksum = 0;

    auto start = std::chrono::steady_clock::now();

    for (uint64_t nonce = 0; nonce < iterations; nonce++)
    {
        if constexpr (optimized)
        {
            // v0.3 - pakartotinis buferio naudojimas
            data.resize(prefix.size());
            data += std::to_string(nonce);
            data += suffix;
        }
        else
        {
            // v0.2 - visa antraste formuojama is naujo
            data =
                prevHash + "|" +
                std::to_string(timestamp) + "|" +
                std::to_string(version) + "|" +
                txRoot + "|" +
                std::to_string(nonce) + "|" +
                std::to_string(difficulty);
        }

        auto hash = gautiHash(data);
        string blockHash = hashIString(hash);

        // Kad skaiciavimai butu panaudojami
        checksum = checksum * 1315423911ULL
                 + std::hash<string>{}(blockHash);
    }

    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> duration = end - start;

    return {duration.count(), checksum};
}
void testPoWPerformance()
{
    const uint64_t iterations = 100000;
    const int runs = 5;

    double totalV02 = 0;
    double totalV03 = 0;

    cout << std::fixed << std::setprecision(6);

    cout << "\n===== PoW NASUMO TESTAS =====\n";
    cout << "Hash skaiciavimu: " << iterations << "\n\n";

    for (int i = 0; i < runs; i++)
    {
        BenchmarkResult v02;
        BenchmarkResult v03;

        // Keiciame vykdymo tvarka, kad sumazintume
        // pirmojo algoritmo matavimo pranasuma
        if (i % 2 == 0)
        {
            v02 = measurePoW<false>(iterations);
            v03 = measurePoW<true>(iterations);
        }
        else
        {
            v03 = measurePoW<true>(iterations);
            v02 = measurePoW<false>(iterations);
        }

        if (v02.checksum != v03.checksum)
        {
            cout << "KLAIDA: algoritmu rezultatai nesutampa!\n";
            return;
        }

        totalV02 += v02.time;
        totalV03 += v03.time;

        cout << "Bandymas " << i + 1
             << " | v0.2: " << v02.time << " s"
             << " | v0.3: " << v03.time << " s\n";
    }

    double avgV02 = totalV02 / runs;
    double avgV03 = totalV03 / runs;

    double improvement =
        (avgV02 - avgV03) / avgV02 * 100.0;

    cout << "\n===== REZULTATAI =====\n";

    cout << "v0.2 vidurkis: " << avgV02 << " s\n";
    cout << "v0.3 vidurkis: " << avgV03 << " s\n";

    cout << "Pagreitejimas: " << improvement << " %\n";

    cout << "Rezultatu patikra: SEKMINGA\n";
}

struct BlockTestAccess
{
    static void corruptMerkleRoot(Block& block)
    {
        block.tx_root[0] = (block.tx_root[0] == '0') ? '1' : '0';// Pakeiciame pirmaji simboli, kad Merkle root butu neteisingas
    }
};

// Kasa bloka tol, kol randamas tinkamas PoW
void mineTestBlock(Block& block)
{
    uint64_t attempts = 0;

    while (!block.mineBlockWithLimit(10000, attempts))
    {
    }
}

void testBlockValidation();
void testCandidateBalances();
vector<User> generateUsers(int numUsers);
vector<Transaction> generateTransactions(const vector<User>& users, int numTransactions);
void testMerkle();
void testInvalidPrevHash();
int main() {
    SetConsoleOutputCP(CP_UTF8); // Nustatome konsolės išvesties koduotę į UTF-8
    SetConsoleCP(CP_UTF8); // Nustatome konsolės įvesties koduotę į UTF-8

    
    vector<User> users = generateUsers(1000);

    vector<Transaction> transactions = generateTransactions(users, 10000);
    
    Blockchain blockchain(users, transactions, 3);
   
    if (blockchain.addBlock())
    {
        cout << "Blokas sekmingai iskastas!" << endl;
    }
    else
    {
        cout << "Bloko iskasti nepavyko!" << endl;
    }
    cout<<"ka norite padaryti? 1. isvesti 5 naudotoju informacija 2. isvesti 5 transakciju informacija 3. sukurti bloka 5 transakciju, 4. genesis blokas, 5. iskviesti kelis blokus, 6 pratestuoti tranzakciju patikra, 7 testuoti blokų generavimą, 8 dubliuotu transakciju tikrinimas, 9 istestuoti klaidu atsakymus, 10 testuoti Merkle medzių generavimą, 11 testuoti neteisingo prev_hash tikrinimą, 12 testuoti visus testus, 13 testuoti PoW našumą, 14 testuoti transakciju atranką "<<endl;
    int choice;
    cin >> choice;
    if (choice == 1) {
    for (int i = 0; i < 5; i++)
    {
        cout << "Vardas: " << users[i].getName() << "\n";
        cout << "Public key: " << users[i].getPublicKey() << "\n";
        cout << "Balansas: " << users[i].getBalance() << "\n\n";
    }
    }
    else if (choice == 2) {
        for (int i = 0; i < 5; i++)
    {
        cout << "Siuntejas: " << transactions[i].getSender() << "\n";
        cout << "Gavejas: " << transactions[i].getReceiver() << "\n";
        cout << "Suma: " << transactions[i].getAmount() << "\n";
        cout << "Sekos numeris: " << transactions[i].getSequence() << "\n";
        cout << "Transakcijos ID: " << transactions[i].getTransactionId() << "\n\n";
    }
    }
    else if (choice == 3) {
        vector<Transaction> blockTransactions(transactions.begin(), transactions.begin() + 5);
        string prevHash(64, '0');
        uint64_t timestamp = 1761730000;
        uint32_t version = 1;
        uint32_t difficulty = 3;

        Block block(prevHash, timestamp, version, difficulty, blockTransactions);
        block.mineBlock();
    }
    else if (choice == 4) {
        Blockchain blockchain(users, transactions, 3);

    cout << "Bloku skaicius: "
         << blockchain.getChain().size() << endl;

    const Block& genesis = blockchain.getChain().front();

    cout << "Genesis prev_hash: "
         << genesis.getPrevHash() << endl;

    cout << "Genesis tx_root: "
         << genesis.getTxRoot() << endl;

    cout << "Genesis hash: "
         << genesis.calculateBlockHash() << endl;

    cout << "Genesis transakciju skaicius: "
         << genesis.getTransactions().size() << endl;
    }
    else if (choice == 5) 
    {
        auto start = std::chrono::steady_clock::now();
        int minedBlocks = 0;
        while (blockchain.addBlock())
        {
            minedBlocks++;
        }
    
        
        const auto& chain = blockchain.getChain();

        bool validChain = true;

        for (size_t i = 1; i < chain.size(); i++)
        {
            if (chain[i].getPrevHash() != chain[i - 1].calculateBlockHash())
            {
                validChain = false;
            cout << "Neteisingas bloku rysys: " << i << endl;
            break;
            }
        }

        if (validChain)
        {
            cout << "Visi blokai teisingai susieti!" << endl;
        }
        auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> duration = end - start;

    cout << "\n========== GALUTINE SUVESTINE ==========\n";

    cout << "Vartotoju skaicius: " << users.size() << endl;
    cout << "Sugeneruotu transakciju: " << transactions.size() << endl;

    cout << "Iskastu bloku: " << minedBlocks << endl;
    cout << "Viso bloku: " << blockchain.getChain().size() << endl;

    cout << "Patvirtintu transakciju: "
         << blockchain.getConfirmedTransactionCount() << endl;

    cout << "Atmestu transakciju: "
         << blockchain.getRejectedTransactionCount() << endl;

    cout << "Likusiu transakciju: "
         << blockchain.getRemainingTransactionCount() << endl;

    cout << "Pradine lesu suma: "
         << blockchain.getInitialBalanceSum() << endl;

    cout << "Galutine lesu suma: "
         << blockchain.getFinalBalanceSum() << endl;

    cout << "Balansu patikra: "
         << (blockchain.verifyBalances() ? "SEKMINGA" : "NESEKMINGA")
         << endl;

    cout << "Bendras laikas: "
         << duration.count() << " s" << endl;
    }
    else if (choice == 6) 
    {
            vector<User> testUsers = {
            User("UserA", "a1", 100),
            User("UserB", "b2", 100),
            User("UserC", "c3", 100)
            };

            vector<Transaction> testTransactions = {
            Transaction("a1", "b2", 70, 1),
            Transaction("a1", "c3", 60, 2)
            };

        Blockchain testBlockchain(testUsers, testTransactions, 3);

        while (testBlockchain.addBlock())
        {
        }
        cout << "Patvirtintu transakciju: "<< testBlockchain.getConfirmedTransactionCount() << endl;
        cout << "Likusiu transakciju: "<< testBlockchain.getRemainingTransactionCount() << endl;
        cout << "Balansu patikra: "<< (testBlockchain.verifyBalances() ? "SEKMINGA" : "NESĖKMINGA")<< endl;
    }
    else if(choice == 7)
    {
            vector<User> users = {
            User("UserA", "a1", 1000000),
            User("UserB", "b2", 1000000)
            };

        vector<Transaction> transactions;

        for (int i = 0; i < 205; i++)
        {
            if (i % 2 == 0)
            {
               transactions.emplace_back("a1", "b2", 1, i + 1);
            }
            else
            {
             transactions.emplace_back("b2", "a1", 1, i + 1);
            }
        }

        Blockchain blockchain(users, transactions, 3);

        while (blockchain.addBlock())
        {
        }

        const auto& chain = blockchain.getChain();

        for (size_t i = 0; i < chain.size(); i++)
        {
            cout << "Blokas " << i<< ": " << chain[i].getTransactions().size()<< " transakciju" << endl;
        }

        cout << "Patvirtintu transakciju: "<< blockchain.getConfirmedTransactionCount() << endl;
        cout << "Likusiu transakciju: "<< blockchain.getRemainingTransactionCount() << endl;
        cout << "Balansu patikra: "<< (blockchain.verifyBalances() ? "SEKMINGA" : "NESĖKMINGA")<< endl;


    }
    else if(choice == 8)
    {
        vector<User> users = {
                User("UserA", "a1", 1000),
                User("UserB", "b2", 1000)
            };

            Transaction tx("a1", "b2", 50, 1);

            vector<Transaction> transactions = {
                tx,
                tx
            };

            Blockchain blockchain(users, transactions, 3);

            while (blockchain.addBlock())
            {
            }

           cout << "Patvirtintu: "<< blockchain.getConfirmedTransactionCount() << endl;
            cout << "Atmestu: "<< blockchain.getRejectedTransactionCount() << endl;
            cout << "Likusiu: "<< blockchain.getRemainingTransactionCount() << endl;

    }
    else if(choice == 9)
    {
vector<User> users = {
        User("UserA", "a1", 1000),
        User("UserB", "b2", 1000)
    };

    vector<Transaction> transactions = {
        Transaction("a1", "b2", 100, 1),
        Transaction("a1", "unknown", 50, 2),
        Transaction("a1", "b2", 0, 3),
        Transaction("a1", "b2", -100, 4)
    };

    Blockchain blockchain(users, transactions, 3);

    while (blockchain.addBlock())
    {
    }

    cout << "Patvirtintu: "
         << blockchain.getConfirmedTransactionCount() << endl;

    cout << "Atmestu: "
         << blockchain.getRejectedTransactionCount() << endl;

    cout << "Likusiu: "
         << blockchain.getRemainingTransactionCount() << endl;

    cout << "Balansu patikra: "
         << (blockchain.verifyBalances() ? "SEKMINGA" : "NESEKMINGA")
         << endl;
    for (const auto& [id, reason] :
     blockchain.getRejectedTransactions())
{
    cout << "Transakcija: " << id << endl;
    cout << "Priezastis: " << reason << endl;
}

    }
    else if(choice == 10)
    {
        testMerkle();
    }
    else if(choice == 11)
    {
        testInvalidPrevHash();
    }
    else if(choice == 12)
    {
        testMerkle();
        testBlockValidation();
        testInvalidPrevHash();
        testCandidateBalances();
    }
    else if(choice == 13)
    {
        testPoWPerformance();
    }
    else if(choice == 14)
    {
        
        blockchain.benchmarkTransactionSelection();
    }
    return 0;
}
vector<User> generateUsers(int numUsers) {
   vector<User> users;
   users.reserve(numUsers);
   std::mt19937 gen(42);
   std::uniform_int_distribution<> dis(100, 1000000);
    for( int i = 0; i < numUsers; i++)
    {
        string name = "User" + std::to_string(i+1);
        auto hash = gautiHash(name);
        string publicKey = hashIString(hash);
        uint64_t balance = dis(gen);
        users.push_back(User(name, publicKey, balance));    
    }
   return users;
}
vector<Transaction> generateTransactions(const vector<User>& users, int numTransactions) {
    if (numTransactions < 0 || users.size() < 2)
{
    throw std::invalid_argument("Neteisingi generavimo parametrai!");
}
    vector<Transaction> transactions;
    transactions.reserve(numTransactions);
    std::mt19937 gen(42);
    std::uniform_int_distribution<> dis(0, users.size() - 1);
    std::uniform_int_distribution<> amountDis(1, 1000);
    for( int i = 0; i < numTransactions; i++)
    {
        int senderIndex = dis(gen);
        int receiverIndex = dis(gen);
        while(receiverIndex == senderIndex)
        {receiverIndex = dis(gen);}
        string sender = users[senderIndex].getPublicKey();
        string receiver = users[receiverIndex].getPublicKey();
        int64_t amount = amountDis(gen);
        uint64_t sequence = i + 1;

        transactions.push_back(Transaction(sender, receiver, amount, sequence));
    }

    return transactions;
}
void testMerkle()
{
    vector<Transaction> transactions = {
        Transaction("a1", "b2", 10, 1),
        Transaction("a1", "b2", 20, 2),
        Transaction("a1", "b2", 30, 3),
        Transaction("a1", "b2", 40, 4),
        Transaction("a1", "b2", 50, 5)
    };

    // 1, 2, 3 ir 5 transakciju testai
    for (int n : {1, 2, 3, 5})
    {
        vector<Transaction> selected(
            transactions.begin(),
            transactions.begin() + n
        );

        string root = calculateMerkleRoot(selected);

        cout << n << " transakciju Merkle Root: "
             << root << endl;

        assert(root.size() == 64);

        if (n == 1)
        {
            assert(root == selected[0].getTransactionId());
        }
    }

    string originalRoot = calculateMerkleRoot(transactions);

    // Pakeiciame vienos transakcijos turini
    vector<Transaction> modified = transactions;
    modified[0] = Transaction("a1", "b2", 999, 1);

    assert(calculateMerkleRoot(modified) != originalRoot);

    // Pakeiciame transakciju tvarka
    vector<Transaction> swapped = transactions;
    std::swap(swapped[0], swapped[1]);

    assert(calculateMerkleRoot(swapped) != originalRoot);

    cout << "Merkle testai SEKMINGI!" << endl;
}
void testInvalidPrevHash()
{
    vector<User> users = {
        User("UserA", "a1", 1000),
        User("UserB", "b2", 1000)
    };

    Transaction tx("a1", "b2", 100, 1);

    Blockchain blockchain(users, {tx}, 3);

    Block invalidBlock(
        string(64, 'f'),  // Neteisingas prev_hash
        1234567890,
        2,
        3,
        {tx}
    );

    // Iskasame bloka su tinkamu PoW
    uint64_t attempts;

    while (!invalidBlock.mineBlockWithLimit(10000, attempts))
    {
    }

    size_t oldChainSize = blockchain.getChain().size();
    int64_t oldBalanceSum = blockchain.getFinalBalanceSum();

    assert(!blockchain.validateBlock(invalidBlock));

    assert(blockchain.getChain().size() == oldChainSize);
    assert(blockchain.getFinalBalanceSum() == oldBalanceSum);

    cout << "Neteisingo prev_hash testas SEKMINGAS!" << endl;
}
void testBlockValidation()
{
    vector<User> users = {
        User("UserA", "a1", 100),
        User("UserB", "b2", 100),
        User("UserC", "c3", 100)
    };

    Transaction tx1("a1", "b2", 70, 1);
    Transaction tx2("a1", "c3", 60, 2);

    Blockchain blockchain(users, {tx1, tx2}, 3);

    string prevHash =
        blockchain.getChain().back().calculateBlockHash();

    uint64_t timestamp = 1234567890;

    // Issaugome pradine busena
    auto initialBalances = blockchain.getBalances();

    size_t initialChainSize = blockchain.getChain().size();
    size_t initialPoolSize =
        blockchain.getRemainingTransactionCount();

    size_t initialConfirmed =
        blockchain.getConfirmedTransactionCount();

    // Pagalbine patikra
    auto checkUnchanged = [&]()
    {
        assert(blockchain.getBalances() == initialBalances);

        assert(blockchain.getChain().size() == initialChainSize);

        assert(blockchain.getRemainingTransactionCount()
               == initialPoolSize);

        assert(blockchain.getConfirmedTransactionCount()
               == initialConfirmed);
    };

    // TESTAS 1 - teisingas blokas
    Block validBlock(prevHash, timestamp, 2, 3, {tx1});

    mineTestBlock(validBlock);

    assert(blockchain.validateBlock(validBlock));
    checkUnchanged();

    cout << "1. Teisingas blokas - OK\n";

    // TESTAS 2 - neteisinga Merkle saknis
    Block invalidRoot = validBlock;

    BlockTestAccess::corruptMerkleRoot(invalidRoot);

    // Iskasame is naujo, kad PoW butu teisingas
    mineTestBlock(invalidRoot);

    assert(!blockchain.validateBlock(invalidRoot));
    checkUnchanged();

    cout << "2. Neteisinga Merkle saknis - OK\n";

    // TESTAS 3 - netinkamas PoW
    Block invalidPoW(
        prevHash, timestamp, 2, 3, {tx1}
    );

    // Parenkame antraste, kurios hash neatitinka difficulty
    while (invalidPoW.calculateBlockHash().substr(0, 3) == "000")
    {
        timestamp++;

        invalidPoW = Block(
            prevHash, timestamp, 2, 3, {tx1}
        );
    }

    assert(!blockchain.validateBlock(invalidPoW));
    checkUnchanged();

    cout << "3. Netinkamas PoW - OK\n";

    // TESTAS 4 - dubliuota transakcija bloke
    Block duplicateBlock(
        prevHash, timestamp, 2, 3, {tx1, tx1}
    );

    mineTestBlock(duplicateBlock);

    assert(!blockchain.validateBlock(duplicateBlock));
    checkUnchanged();

    cout << "4. Dubliuota transakcija - OK\n";

    // TESTAS 5 - dvi transakcijos virsija balansa
    // A turi 100, o bando issiusti 70 + 60
    Block overspendingBlock(
        prevHash, timestamp, 2, 3, {tx1, tx2}
    );

    mineTestBlock(overspendingBlock);

    assert(!blockchain.validateBlock(overspendingBlock));
    checkUnchanged();

    cout << "5. Virsytas balansas - OK\n";

    // TESTAS 6 - kandidatas negali sumazinti difficulty
    Block easyBlock(
        prevHash, timestamp, 2, 1, {tx1}
    );

    mineTestBlock(easyBlock);

    assert(!blockchain.validateBlock(easyBlock));
    checkUnchanged();

    cout << "6. Sumazintas difficulty - OK\n";

    cout << "\nVISI BLOKU VALIDACIJOS TESTAI SEKMINGI!\n";
}
void testCandidateBalances()
{
    vector<User> users = {
        User("UserA", "a1", 1000),
        User("UserB", "b2", 1000),
        User("UserC", "c3", 1000)
    };

    vector<Transaction> transactions = {
        Transaction("a1", "b2", 100, 1),
        Transaction("b2", "c3", 200, 2),
        Transaction("c3", "a1", 150, 3)
    };

    Blockchain blockchain(users, transactions, 3);

    assert(blockchain.addBlock());

    const Block& winner = blockchain.getChain().back();

    unordered_map<string, int64_t> expectedBalances = {
        {"a1", 1000},
        {"b2", 1000},
        {"c3", 1000}
    };

    // Pritaikome tik laimejusio bloko transakcijas
    for (const auto& tx : winner.getTransactions())
    {
        expectedBalances[tx.getSender()] -= tx.getAmount();
        expectedBalances[tx.getReceiver()] += tx.getAmount();
    }

    assert(blockchain.getBalances() == expectedBalances);

    assert(blockchain.getConfirmedTransactionCount()
           == winner.getTransactions().size());

    cout << "7. Kandidatu balansu testas - OK\n";
}