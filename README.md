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
