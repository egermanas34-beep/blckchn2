# blckchn2

## Projekto aprašymas

Projekto tikslas – C++ programavimo kalba realizuoti supaprastintą blokų grandinę, kurioje generuojami vartotojai ir transakcijos, formuojami blokai, vykdomas Proof-of-Work (PoW) algoritmas, tikrinamas duomenų teisingumas ir atnaujinami vartotojų balansai.

Programoje naudojama pirmosios praktinės užduoties metu sukurta 256 bitų maišos funkcija.

## v0.1

### 1. Programos veikimas

Programa atlieka šiuos veiksmus:

1. Sugeneruoja 1000 vartotojų su unikaliais viešaisiais identifikatoriais ir pradiniais balansais.
2. Sugeneruoja 10000 transakcijų tarp atsitiktinai parinktų vartotojų.
3. Sukuria pradinį Genesis bloką.
4. Atrenka iki 100 tinkamų transakcijų vienam blokui.
5. Apskaičiuoja transakcijų bendrą maišą (`tx_root`).
6. Vykdo Proof-of-Work algoritmą, ieškodama tinkamos `nonce` reikšmės.
7. Patikrina iškastą bloką ir jo transakcijas.
8. Sėkmingai patvirtinus bloką, atnaujina vartotojų balansus ir pašalina patvirtintas transakcijas iš fondo.
9. Kartojant procesą suformuoja blokų grandinę ir pateikia galutinę suvestinę.

### 2. Projekto struktūra

Programa realizuota taikant objektinio programavimo principus.

- `User` – saugo vartotojo vardą, viešąjį identifikatorių ir pradinį balansą.
- `Transaction` – saugo transakcijos duomenis ir apskaičiuoja jos ID.
- `Block` – saugo bloko antraštę bei transakcijas, apskaičiuoja maišas ir vykdo PoW.
- `Blockchain` – valdo grandinę, transakcijų fondą, validaciją ir vartotojų balansus.
- `Hash` – pirmosios užduoties maišos algoritmo realizacija.
  Klasių deklaracijos saugomos `include/` aplanke, o realizacijos – `src/` aplanke.

### 3. Vartotojų ir transakcijų generavimas

Sugeneruojama 1000 vartotojų, kurių pradiniai balansai yra intervale nuo 100 iki 1 000 000 vienetų.
Vartotojų viešieji identifikatoriai (`publicKey`) apskaičiuojami taikant maišos funkciją jų vardams.
Sugeneruojama 10000 transakcijų. Kiekviena transakcija turi:

- `sender` – siuntėjo identifikatorių;
- `receiver` – gavėjo identifikatorių;
- `amount` – pervedamą sumą;
- `sequence` – unikalų transakcijos numerį;
- `transaction_id` – apskaičiuotą transakcijos maišą.
  Transakcijos ID apskaičiuojamas pagal formulę:
  `H(sender|receiver|amount|sequence)`
  Laukai jungiami nurodyta tvarka, naudojant `|` skirtuką. Skaičiai užrašomi dešimtaine forma.
  Atsitiktinių duomenų generavimui naudojamas `std::mt19937` su fiksuotu `seed = 42`.

### 4. Blokų struktūra ir maišos skaičiavimas

Kiekvienas blokas turi šešis antraštės laukus:

- `prev_hash`
- `timestamp`
- `version`
- `tx_root`
- `nonce`
- `difficulty`
  Be antraštės, bloke saugomas transakcijų sąrašas.
  v0.1 versijoje `tx_root` apskaičiuojamas sujungiant visų bloko transakcijų ID jų eilės tvarka ir pritaikant maišos funkciją.
  Bloko hash apskaičiuojamas iš visų šešių antraštės laukų:
  `H(prev_hash|timestamp|version|tx_root|nonce|difficulty)`
  Grandinė pradedama Genesis bloku, kurio `prev_hash` sudaro 64 nuliai. Genesis blokas neturi transakcijų ir jam netaikomas PoW.

### 5. Proof-of-Work

Blokų kasimui naudojamas `difficulty = 3`.
Kasimo algoritmas pradeda nuo `nonce = 0` ir didina jo reikšmę tol, kol randama tinkama maiša.
Kiekvienam iškastam blokui pateikiama:

- Rastas `nonce`;
- Bandymų skaičius;
- Kasimo laikas;
- Bloko hash.

### 6. Transakcijų ir blokų validacija

Prieš patvirtinant transakciją tikrinama, ar egzistuoja siuntėjas ir gavėjas, ar jie skirtingi, ar suma teigiama, ar transakcijos ID teisingas ir ar mokėjimas nebuvo patvirtintas anksčiau.
Taip pat tikrinamas lėšų pakankamumas ir galimas balanso perpildymas.
Atrenkant transakcijas naudojama laikina balansų kopija. Tikrieji balansai atnaujinami tik po sėkmingos viso bloko validacijos.
Bloko validavimo metu tikrinamas ankstesnio bloko hash, versija, sudėtingumas, transakcijų skaičius, `tx_root`, PoW ir transakcijų teisingumas.

### 7. Testavimas

Atlikti šie bandymai:
| Testas | Rezultatas |
| ----------------------------------------------- | ---------- |
| 1000 vartotojų ir 10000 transakcijų generavimas | Sėkmingas |
| Vieno bloko PoW | Sėkmingas |
| Genesis bloko sukūrimas | Sėkmingas |
| 100 blokų kasimas | Sėkmingas |
| Blokų tarpusavio ryšių patikra | Sėkminga |
| Nepakankamas vartotojo balansas | Sėkmingas |
| Dubliuotos transakcijos ID | Sėkmingas |
| Nepilnas blokas (205 transakcijos) | Sėkmingas |
| Bendros lėšų sumos išsaugojimas | Sėkmingas |

### 8. Galutiniai rezultatai

Paleidus programą su 1000 vartotojų ir 10000 transakcijų, gauti rezultatai:
| Rodiklis | Reikšmė |
| ----------------------- | --------: |
| Vartotojų skaičius | 1000 |
| Sugeneruotų transakcijų | 10000 |
| Iškastų blokų | 100 |
| Visų blokų su Genesis | 101 |
| Patvirtintų transakcijų | 10000 |
| Atmestų transakcijų | 0 |
| Likusių transakcijų | 0 |
| Pradinė lėšų suma | 499296148 |
| Galutinė lėšų suma | 499296148 |
| Bendras vykdymo laikas | 1.44757 s |
Visų blokų nuorodos į ankstesnius blokus atitiko apskaičiuotas maišas, o bendra lėšų suma išliko nepakitusi.

### 9. Kompiliavimas ir paleidimas

Programa parašyta C++17 kalba.
Kompiliavimas naudojant `g++` iš pagrindinio projekto aplanko:

```bash
g++ -std=c++17 -O2 -Iinclude src/*.cpp -o blockchain.exe
```

Paleidimas Windows PowerShell terminale:

```powershell
.\blockchain.exe
```

Mažiems bandymams galima pakeisti generuojamų vartotojų ir transakcijų skaičių `main.cpp` faile.

### 10. Atkuriamumas

Generuojant vartotojus ir transakcijas naudojamas fiksuotas atsitiktinių skaičių generatoriaus `seed = 42`.
Kasimo sudėtingumas – 3 pradiniai šešioliktainiai nuliai.
Bloko laiko žymai naudojamas vykdymo metu gaunamas laikas, todėl skirtingų paleidimų blokų hash, `nonce` ir kasimo laikai gali skirtis.

### 11. DI naudojimas

Kuriant projektą buvo naudojamas ChatGPT kaip pagalbinis įrankis C++ klasių struktūrai aptarti, kodui peržiūrėti, klaidoms nustatyti ir programos veikimui paaiškinti.
Programos komponentai buvo kuriami ir testuojami palaipsniui. DI pasiūlymai buvo peržiūrimi ir tikrinami praktiniais bandymais.

### 12. Naudojama maišos funkcija

Projektas naudoja pirmosios praktinės užduoties metu sukurtą 256 bitų maišos funkciją.
**Pirmosios užduoties GitHub nuoroda:** (https://github.com/egermanas34-beep/blockchain_technologies.git)

**Naudojama maišos algoritmo versija:** buvo naudojama v0.3 DI patobulinta versija

## v0.2 – Merkle medis, išplėstinė validacija ir blokų kandidatų konkurencija

### 1. Versijos aprašymas

v0.2 versijoje išplėstas ankstesnėje versijoje sukurtos blokų grandinės funkcionalumas. Įgyvendintas Merkle medis, papildyti blokų validacijos testai ir realizuota penkių blokų kandidatų konkurencijos simuliacija.
Pagrindiniai pakeitimai:

- Ankstesnis `tx_root` skaičiavimas pakeistas dvejetainiu Merkle medžiu.
- Realizuotas blokų kasimas su ribotu bandymų skaičiumi.
- Kiekvienam naujam grandinės aukščiui sudaromi penki blokų kandidatai.
- Kandidatai tikrinami nuosekliai, naudojant atskiras laikinas balansų būsenas.
- Išrenkamas vienas laimėtojas, kurio blokas papildomai validuojamas.
- Išplėstas transakcijų ir blokų teisingumo testavimas.

### 2. Merkle medžio realizacija

v0.1 versijoje `tx_root` buvo apskaičiuojamas sujungiant visų bloko transakcijų ID ir pritaikant maišos funkciją.
v0.2 versijoje realizuotas dvejetainis Merkle medis.
Jo veikimo principas:

1. Merkle medžio lapai sudaromi iš transakcijų ID jų eilės tvarka bloke.
2. Maišų HEX reikšmės dekoduojamos į baitus.
3. Du gretimi mazgai sujungiami ir apskaičiuojama jų bendra maiša: `H(left_bytes || right_bytes)`.
4. Jeigu lygyje yra nelyginis mazgų skaičius, paskutinis mazgas dubliuojamas.
5. Procesas kartojamas, kol lieka viena maiša – Merkle šaknis.

Jeigu bloke yra viena transakcija, jos ID tampa Merkle šaknimi. Tuščiam Genesis blokui naudojama tuščios eilutės maiša.
Merkle medžio skaičiavimui sukurti atskiri `merkle.h` ir `merkle.cpp` failai.
Funkcija `calculateMerkleRoot()` naudojama tiek formuojant blokus, tiek tikrinant jų teisingumą.

### 3. Penkių blokų kandidatų konkurencija

Ankstesnėje versijoje kiekvienam grandinės aukščiui buvo formuojamas vienas blokas.
v0.2 versijoje realizuota penkių kandidatų konkurencijos simuliacija.

#### Kandidatų sudarymas

Kiekvienam naujam grandinės aukščiui sukuriami penki kandidatai. Visi jie naudoja tą patį `prev_hash`, laiko žymą ir sudėtingumo lygį (`difficulty = 3`).
Kiekvienas kandidatas gali turėti iki 100 transakcijų.
Transakcijos atrenkamos naudojant skirtingas atsitiktinių skaičių generatoriaus pradines reikšmes (`seed`), todėl kandidatų transakcijų rinkiniai ir jų eiliškumas gali skirtis.
Kandidatų transakcijos gali persidengti, tačiau vieno kandidato bloke negali būti dubliuotų transakcijų.

#### Ribotas Proof-of-Work kasimas

Sukurtas naujas metodas `mineBlockWithLimit()`, leidžiantis kandidatui atlikti ribotą maišos skaičiavimo bandymų skaičių.
Naudojama tokia strategija:

- Pirmajame etape kiekvienam kandidatui skiriama 1000 bandymų.
- Jeigu nė vienas kandidatas neranda tinkamo hash, bandymų biudžetas dvigubinamas.
- Kiekviename naujame etape tęsiama nuo dar netikrinto `nonce`.
- Didžiausias paieškos etapų skaičius – 8.
- Kasimo sudėtingumas paieškos metu nemažinamas.

#### Laimėtojo pasirinkimas

Pasibaigus etapui, kuriame bent vienas kandidatas rado tinkamą hash, išrenkamas laimėtojas.
Laimi kandidatas, kuriam prireikė mažiausiai bandymų. Jeigu bandymų skaičius vienodas, pasirenkamas mažesnio indekso kandidatas.
Prieš įtraukiant laimėtojo bloką į grandinę, jis papildomai tikrinamas naudojant `isValidBlock()`.
Tik sėkmingai patvirtinus bloką atnaujinami vartotojų balansai ir transakcijų fondas.
Nelaimėjusių kandidatų transakcijos neturi poveikio patvirtintai grandinės būsenai.

### 4. Išplėstinė validacija

v0.2 versijoje naudojami trys transakcijų tikrinimo etapai:
**1. Pirminė validacija**
Prieš įtraukiant transakciją į fondą tikrinami jos dalyviai, suma, transakcijos ID ir pasikartojimai.
**2. Kandidato formavimas**
Atrenkant transakcijas naudojama laikina vartotojų balansų kopija. Transakcijos tikrinamos ir pritaikomos jų numatyta tvarka bloke, todėl neleidžiama išleisti daugiau lėšų, nei turi siuntėjas.
**3. Galutinė bloko validacija**
Prieš įtraukiant bloką į grandinę tikrinami:

- Ankstesnio bloko hash (`prev_hash`);
- Bloko versija ir sudėtingumas;
- Merkle šaknis (`tx_root`);
- Proof-of-Work rezultatas;
- Transakcijų ID ir pasikartojimai;
- Transakcijų vykdymo galimybė pagal balansus.
  Netinkamas blokas atmetamas nekeičiant patvirtintų vartotojų balansų, grandinės ir transakcijų fondo.

### 5. Testavimas

#### Merkle medžio testai

Merkle medžio skaičiavimas išbandytas su 1, 2, 3 ir 5 transakcijomis.
Visais atvejais gautos 64 šešioliktainių simbolių ilgio Merkle šaknys.
Papildomai patikrinta:

- Vienos transakcijos atveju Merkle šaknis sutampa su jos ID.
- Pakeitus transakcijos turinį ir perskaičiavus jos ID, Merkle šaknis pasikeičia.
- Sukeitus dvi skirtingas transakcijas vietomis, Merkle šaknis pasikeičia.

Visi atlikti testai praėjo sėkmingai.

#### Blokų validacijos testai

| Testas                           | Rezultatas      |
| -------------------------------- | --------------- |
| Teisingo bloko validacija        | Sėkmingas       |
| Neteisinga Merkle šaknis         | Blokas atmestas |
| Netinkamas PoW                   | Blokas atmestas |
| Dubliuota transakcija bloke      | Blokas atmestas |
| Viršytas siuntėjo balansas       | Blokas atmestas |
| Sumažintas bloko sudėtingumas    | Blokas atmestas |
| Neteisingas `prev_hash`          | Blokas atmestas |
| Penkių kandidatų balansų patikra | Sėkminga        |

Taip pat patikrinta, kad validacijos metu netinkami blokai nepakeičia patvirtintų balansų, blokų skaičiaus, likusių ir patvirtintų transakcijų skaičių.

### 6. Kandidatų kasimo rezultatai

Vieno atlikto bandymo metu gauti šie rezultatai:

| Kandidatas | Bandymų skaičius | Kasimo laikas (s) | Rastas PoW |
| ---------- | ---------------: | ----------------: | ---------- |
| 1          |             3000 |         0.0114442 | Ne         |
| 2          |             3000 |         0.0085706 | Ne         |
| 3          |             3000 |         0.0083528 | Ne         |
| 4          |             3000 |         0.0047028 | Ne         |
| 5          |             2806 |         0.0067660 | Taip       |

**Laimėtojas – 5 kandidatas.**
Laimėjusio bloko hash:
`00033a86b58900aa2b6741db43dea4bc600c195caef3ec3ee2de46864b98ce08`
Maiša prasideda trimis nuliais, todėl atitinka nustatytą `difficulty = 3` reikalavimą.
Kasimo laikas gali skirtis priklausomai nuo kompiuterio apkrovos, bloko antraštės ir kitų vykdymo sąlygų.

### 7. Konkurencijos simuliacijos apribojimai

Realizuota penkių blokų kandidatų sistema imituoja konkurenciją dėl naujo bloko, tačiau nėra tikras decentralizuotas konsensuso mechanizmas.
Visi kandidatai kuriami ir kasami viename procese, nuosekliai vykdant jų PoW paiešką.
Programoje nenaudojamas P2P tinklas, nepriklausomi tinklo mazgai, blokų perdavimo protokolai ar grandinės šakų persitvarkymo mechanizmai.
Tai mokomoji simuliacija, skirta suprasti blokų atrankos, validacijos ir Proof-of-Work veikimą.

### 8. v0.2 rezultatas

v0.2 versijoje išsaugotas ankstesnės blokų grandinės funkcionalumas ir sėkmingai integruotas Merkle medis bei penkių kandidatų konkurencijos mechanizmas.
Atlikti bandymai parodė, kad netinkami blokai atmetami, kandidatai gali būti kasami naudojant ribotą bandymų skaičių, o patvirtintą grandinės būseną keičia tik laimėjusio kandidato transakcijos.

## v0.3 – Blokų grandinės optimizavimas naudojant DI

### 1. Versijos aprašymas

v0.3 versijoje buvo siekiama pagerinti ankstesnės v0.2 versijos našumą, išsaugant blokų grandinės funkcionalumą ir duomenų apdorojimo teisingumą.
Optimizavimui naudotas DI įrankis **ChatGPT (GPT-6)**.
Pasirinktos dvi optimizavimo kryptys:

1. Proof-of-Work (PoW) kasimo ciklo optimizavimas.
2. Transakcijų atrankos į blokų kandidatus optimizavimas.
   Abu pasiūlymai buvo pritaikyti programoje, patikrintas jų teisingumas ir atlikti našumo matavimai.

### 2. Pirmasis optimizavimas – PoW kasimo ciklas

#### Problema

v0.2 versijoje kiekvieno PoW bandymo metu buvo iš naujo formuojama visa bloko antraštės eilutė:
`prev_hash|timestamp|version|tx_root|nonce|difficulty`
Kadangi kasimo metu keičiasi tik `nonce`, kitų antraštės laukų pakartotinis sujungimas kiekviename bandyme sukelia nereikalingų operacijų.

#### DI pasiūlytas sprendimas

Optimizuotoje `mineBlockWithLimit()` funkcijoje nekintamos antraštės dalys suformuojamos vieną kartą prieš pradedant kasimą.
Naudojami du kintamieji:

- `prefix` – nekintama antraštės dalis iki `nonce`;
- `suffix` – nekintama dalis po `nonce`.
  Kiekvieno bandymo metu naudojamas tas pats `std::string` buferis, išsaugoma nekintama pradžia ir pridedamos tik aktualios reikšmės.
  Taip pat PoW sąlygos tikrinimui vietoje `substr()` panaudotas `compare()` metodas.
  Pakeitimai nekeičia bloko antraštės serializacijos, naudojamos maišos funkcijos ar sudėtingumo reikalavimo.

#### Našumo testas

Abu antraštės formavimo algoritmai palyginti naudojant identiškus duomenis ir tas pačias `nonce` reikšmes.
Testavimo sąlygos:

- 100 000 maišos skaičiavimų vienam bandymui.
- 5 matavimai.
- Ta pati bloko antraštė.
- Ta pati maišos funkcija.
- C++17, `g++`, optimizavimas `-O2`.
- Matavimo metu nespаusdinama kiekvieno bandymo informacija.

#### Rezultatai

| Bandymas                   |     v0.2 (s) |     v0.3 (s) |
| -------------------------- | -----------: | -----------: |
| 1                          |     0.250160 |     0.117897 |
| 2                          |     0.151605 |     0.105886 |
| 3                          |     0.134356 |     0.126133 |
| 4                          |     0.177739 |     0.129936 |
| 5                          |     0.155289 |     0.120026 |
| **Vidurkis**               | **0.173830** | **0.119976** |
| **Standartinis nuokrypis** | **0.045380** | **0.009219** |

**Vykdymo laiko sumažėjimas – 30,98 %.**
Optimizuotas PoW antraštės formavimo algoritmas šiame teste veikė maždaug 1,45 karto sparčiau.
Papildomai apskaičiuotos abiejų algoritmų rezultatų kontrolinės sumos sutapo.
**Išvada:** nekintamų bloko antraštės dalių paruošimas ir pakartotinis eilutės buferio naudojimas sumažino PoW bandymams reikalingą laiką.

### 3. Antrasis optimizavimas – transakcijų atranka

#### Problema

v0.2 versijoje `selectTransactions()` funkcija sukurdavo viso transakcijų fondo kopiją ir ją sumaišydavo naudodama `std::shuffle()`.
Kai fonde yra 10 000 transakcijų, kiekvienam blokų kandidatui reikėdavo kopijuoti visus `Transaction` objektus.
Kadangi viename objekte saugomos kelios eilutės ir kiti laukai, toks kopijavimas didino atminties naudojimą bei vykdymo laiką.

#### DI pasiūlytas sprendimas

v0.3 versijoje transakcijų objektų kopijavimas pakeistas jų indeksų maišymu.
Naudojamas `vector<size_t>`, kuriame saugomi transakcijų indeksai.
Indeksai užpildomi naudojant `std::iota()`, o vėliau sumaišomi su `std::shuffle()`.
Atrankos metu transakcijos pasiekiamos pagal indeksą, naudojant nuorodą į fonde esantį objektą.
Taip išvengiama viso transakcijų fondo kopijavimo. Į galutinį blokų kandidatą kopijuojamos tik atrinktos transakcijos, kurių gali būti daugiausia 100.
Transakcijų validavimo ir laikinų balansų atnaujinimo logika išliko nepakitusi.

#### Našumo testas

Palyginti v0.2 ir v0.3 transakcijų atrankos algoritmai.
Testavimo sąlygos:

- 1000 vartotojų.
- 10 000 transakcijų fondas.
- 5 blokų kandidatai.
- 10 atrankos ciklų vienam matavimui.
- Iš viso 50 atrankų per vieną matavimą.
- 5 matavimai.
- Tie patys atsitiktinių skaičių generatoriaus `seed`.
- Ta pati transakcijų validavimo logika.
  Prieš matavimus papildomai patikrinta, ar abu algoritmai su tais pačiais `seed` atrenka tas pačias transakcijas ta pačia tvarka.

#### Rezultatai

| Bandymas                   |     v0.2 (s) |     v0.3 (s) |
| -------------------------- | -----------: | -----------: |
| 1                          |     0.265748 |     0.019383 |
| 2                          |     0.239709 |     0.024113 |
| 3                          |     0.254521 |     0.017724 |
| 4                          |     0.236686 |     0.022662 |
| 5                          |     0.256311 |     0.019197 |
| **Vidurkis**               | **0.250595** | **0.020616** |
| **Standartinis nuokrypis** | **0.012141** | **0.002661** |

**Vykdymo laiko sumažėjimas – 91,77 %.**
Optimizuotas atrankos algoritmas šiame teste veikė maždaug 12,16 karto sparčiau.
Abu algoritmai atrinko tokį patį transakcijų skaičių ir tas pačias transakcijas ta pačia tvarka.
**Išvada:** transakcijų objektų kopijavimo pakeitimas jų indeksų maišymu reikšmingai pagerino atrankos našumą.

### 4. Bendras optimizavimų palyginimas

| Rodiklis              | PoW kasimas | Transakcijų atranka |
| --------------------- | ----------: | ------------------: |
| v0.2 vidutinis laikas |  0.173830 s |          0.250595 s |
| v0.3 vidutinis laikas |  0.119976 s |          0.020616 s |
| Laiko sumažėjimas     |     30,98 % |             91,77 % |
| Teisingumo patikra    |    Sėkminga |            Sėkminga |

Abu optimizavimai parodė teigiamus rezultatus.
Didžiausias pagerėjimas pasiektas transakcijų atrankoje, kurioje buvo pašalintas nereikalingas didelio `Transaction` objektų vektoriaus kopijavimas.
Svarbu pažymėti, kad pateikti rezultatai apibūdina **atskirų algoritmų našumą**, o ne bendrą visos blokų grandinės programos pagreitėjimą.
Testuose v0.2 algoritmai buvo atkurti dabartinės programos testavimo funkcijose ir palyginti su optimizuotais v0.3 metodais.

### 5. Programos teisingumo tikrinimas

Po optimizavimų pakartotinai atlikti ankstesni testai.
Patikrinta:

- Merkle šaknų skaičiavimas su 1, 2, 3 ir 5 transakcijomis.
- Teisingo bloko validavimas.
- Neteisingos Merkle šaknies atmetimas.
- Netinkamo PoW atmetimas.
- Dubliuotos transakcijos atmetimas.
- Nepakankamo balanso atvejis.
- Sumažinto `difficulty` atmetimas.
- Neteisingo `prev_hash` atmetimas.
- Penkių blokų kandidatų kasimas.
- Tik laimėjusio kandidato poveikis vartotojų balansams.
  Visi pakartotinai paleisti testai praėjo sėkmingai.
  Našumo testuose papildomai patikrintas PoW rezultatų kontrolinių sumų sutapimas ir abiejų transakcijų atrankos algoritmų rezultatų vienodumas.

### 6. DI naudojimo aprašymas

**Naudotas įrankis:** ChatGPT (GPT-6).
DI buvo pateiktos anksčiau realizuotos PoW kasimo ir transakcijų atrankos funkcijos bei užduoties reikalavimai.
Pagrindinės DI užklausų temos:

1. Kaip optimizuoti PoW kasimo ciklą išsaugant tą pačią bloko maišos reikšmę?
2. Kaip sumažinti transakcijų objektų kopijavimą formuojant blokų kandidatus?
3. Kaip patikrinti v0.2 ir v0.3 algoritmų našumą tomis pačiomis sąlygomis?
   **Priimti DI pasiūlymai:**

- Pakartotinai naudoti nekintamas bloko antraštės dalis ir eilutės buferį.
- Maišyti transakcijų indeksus, o ne visus transakcijų objektus.
- Atlikti atskirus algoritmų teisingumo ir našumo testus.
  Pakeitimai buvo integruoti į projektą ir patikrinti praktiniais bandymais.
  Abu optimizavimo pasiūlymai pasiteisino pagal atliktų testų rezultatus. Šiame etape neprireikė atmesti nė vieno iš dviejų pasirinktų pasiūlymų.
  DI taip pat buvo naudojamas ankstesnėse v0.1 ir v0.2 projekto kūrimo stadijose, todėl v0.2 laikoma ankstesne programos versija, o ne versija, sukurta visiškai be DI pagalbos.

### 7. v0.3 išvados

Naudojant DI pasiūlymus pavyko patobulinti du blokų grandinės programos algoritmus.
PoW optimizavimas sumažino testuojamo skaičiavimo laiką 30,98 %, o transakcijų atrankos optimizavimas – 91,77 %.
Atlikti teisingumo testai parodė, kad optimizavimai nesutrikdė tikrintų blokų grandinės funkcijų.
Gauti rezultatai parodo, kad net ir nekeičiant pagrindinio algoritmo galima pagerinti programos našumą sumažinant nereikalingas eilutės formavimo, atminties paskirstymo ir objektų kopijavimo operacijas.
**Apribojimas:** šiame palyginime išmatuotos dvi atskiros algoritmo dalys. Visos programos vykdymo laiko pagreitėjimui įvertinti reikėtų papildomo pilnų v0.2 ir v0.3 versijų palyginimo.

## v0.4 – Lygiagretus blokų kasimas naudojant OpenMP

### 1. Versijos aprašymas

v0.4 versijoje papildytas ankstesnėje v0.3 versijoje realizuotas blokų kasimo mechanizmas, pritaikant lygiagretų programavimą su **OpenMP**.
Pagrindinis tikslas – pagreitinti penkių blokų kandidatų kasimą, išsaugant ankstesnės versijos funkcionalumą ir blokų grandinės teisingumą.
Ankstesnėje versijoje penki kandidatai buvo kasami nuosekliai, o v0.4 versijoje jie gali būti kasami vienu metu, naudojant kelias procesoriaus gijas.

### 2. Lygiagretaus kasimo įgyvendinimas

Lygiagretinimas įgyvendintas `Blockchain::addBlock()` funkcijoje, naudojant OpenMP direktyvą:

```cpp
#pragma omp parallel for schedule(static)
for (int i = 0; i < candidateCount; i++)
{
    // Kiekviena gija kasa atskirą blokų kandidatą
}
```

Kiekviena gija savarankiškai vykdo jai priskirto kandidato PoW kasimą, naudodama jau optimizuotą `mineBlockWithLimit()` funkciją.
Siekiant išvengti duomenų lenktynių, kiekviena gija keičia tik savo kandidato duomenis ir atitinkamus rezultatų masyvų elementus.
Papildomai `vector<bool>` pakeistas į `vector<int>`, nes `vector<bool>` elementai saugomi specialiu bitų pavidalu ir gali sukelti problemų lygiagrečiai keičiant jų reikšmes.
Laimėtojas išrenkamas tik pasibaigus einamajam lygiagretaus kasimo raundui, kai visos gijos užbaigia darbą.
**Išsaugota ankstesnė kandidatų konkurencijos logika:**

- Kiekvienam blokui sukuriami 5 kandidatai.
- Kiekvienas kandidatas turi atskirai atrinktas transakcijas.
- Kasimas vykdomas raundais, didinant bandymų biudžetą.
- Laimėtojas nustatomas pagal mažiausią atliktų PoW bandymų skaičių.
- Tik laimėjęs blokas validuojamas ir pridedamas prie grandinės.
- Vartotojų balansai atnaujinami tik patvirtinus laimėjusį bloką.

### 3. Našumo testavimas

Siekiant įvertinti OpenMP efektyvumą, sukurta atskira `benchmarkParallelMining()` funkcija.
Testas palygina nuoseklų ir lygiagretų blokų kasimą, naudojant skirtingą gijų skaičių.
**Testavimo sąlygos:**

- Blokų kandidatų skaičius – 5.
- PoW sudėtingumas (`difficulty`) – 3.
- Naudojamas gijų skaičius – 1, 2 ir 5.
- Kiekvienai konfigūracijai atlikta 10 matavimų.
- Prieš matavimus atlikti apšilimo bandymai.
- Visoms konfigūracijoms naudojami identiški pradiniai blokų kandidatai.
- Gijų konfigūracijų testavimo tvarka keičiama tarp bandymų.
- Kompiliuojama naudojant C++17, `-O2` ir `-fopenmp`.

Kiekvienas matavimas pradedamas nuo tų pačių kandidatų pradinių būsenų, todėl skirtingos gijų konfigūracijos sprendžia identišką PoW užduotį.
Testas nekeičia blokų grandinės būsenos, vartotojų balansų ar transakcijų fondo.

#### Našumo rezultatai

| Rodiklis                   |   1 gija |  2 gijos |  5 gijos |
| -------------------------- | -------: | -------: | -------: |
| Vidutinis laikas (s)       | 0.005377 | 0.003691 | 0.002586 |
| Standartinis nuokrypis (s) | 0.000948 | 0.000677 | 0.001003 |
| Pagreitėjimas              |    1.00x |    1.46x |    2.08x |
| Laiko sumažėjimas          |        – |  31.36 % |  51.91 % |

Gauti rezultatai rodo, kad didinant naudojamų gijų skaičių vidutinė blokų kandidatų kasimo trukmė sumažėjo.
Naudojant 2 gijas pasiektas maždaug **1,46 karto pagreitėjimas**, o naudojant 5 gijas – **2,08 karto pagreitėjimas**, palyginti su nuosekliu kasimu.
Didžiausias našumo pagerėjimas pasiektas naudojant 5 gijas – vidutinis kasimo laikas sumažėjo apie **51,91 %**.
Nors penki kandidatai gali būti kasami lygiagrečiai, penkių kartų pagreitėjimas nepasiektas. Tam įtakos gali turėti gijų valdymo ir sinchronizavimo sąnaudos bei nevienoda kandidatų kasimo trukmė.

### 4. Teisingumo testavimas

Našumo teste papildomai tikrinama, ar skirtingas gijų skaičius nekeičia kasimo rezultato.
Kiekvieno matavimo metu palyginami:

- Laimėjusio kandidato indeksas.
- Laimėjusio bloko maišos reikšmė.
  Visuose matavimuose gauti identiški rezultatai.
  Programos išvestis:

```text
Visu testu laimetojas ir bloko hash sutapo!
```

Taip pat pakartotinai paleisti ankstesni blokų grandinės teisingumo testai. Visi testai praėjo sėkmingai.
Tai patvirtina, kad atliktuose bandymuose lygiagretus kasimas nepakeitė blokų grandinės veikimo rezultatų.

### 5. DI naudojimas

**Naudotas įrankis:** ChatGPT (GPT-6).
DI buvo naudojamas analizuojant galimybę papildyti ankstesnę blokų grandinės versiją lygiagrečiu kasimu.
Priimti DI pasiūlymai:

1. Naudoti OpenMP penkių nepriklausomų blokų kandidatų kasimui.
2. Pakeisti `vector<bool>` į `vector<int>`, siekiant išvengti galimų duomenų lenktynių.
3. Atskirai matuoti bendrą kandidatų kasimo trukmę.
4. Sukurti automatizuotą našumo testą, palyginantį 1, 2 ir 5 gijų konfigūracijas.
5. Tikrinti, ar skirtingas gijų skaičius nekeičia laimėjusio bloko rezultato.
   Pasiūlyti pakeitimai buvo pritaikyti programoje ir patikrinti testais.

### 6. Kompiliavimas

Kadangi v0.4 versijoje naudojamas OpenMP, kompiliuojant būtina pridėti `-fopenmp` parametrą.

```bash
g++ -std=c++17 -O2 -fopenmp -Iinclude src/*.cpp -o blockchain.exe
```

Programos paleidimas:

```bash
./blockchain.exe
```

### 7. Išvados

v0.4 versijoje sėkmingai įgyvendintas lygiagretus blokų kandidatų kasimas naudojant OpenMP. Atliktas našumo testas parodė, kad naudojant 5 gijas pasiektas **2,08 karto pagreitėjimas**, palyginti su nuosekliu kasimu. Visi atlikti teisingumo testai praėjo sėkmingai, o skirtingų gijų konfigūracijų kasimo rezultatai sutapo. Eksperimentas parodė, kad lygiagretus nepriklausomų blokų kandidatų kasimas gali sumažinti PoW vykdymo laiką.
