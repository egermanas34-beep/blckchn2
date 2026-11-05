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
