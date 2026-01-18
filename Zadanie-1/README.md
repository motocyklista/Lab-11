# Program: "System bankowy 2.0 - Refaktoryzacja i Agregacja"
Program w języku C, który symuluje podstawowe operacje bankowe na 100 kontach. Użytkownik może wpłacać środki (DEPOSIT), wypłacać środki (WITHDRAW), sprawdzać saldo (SHOW) oraz przeglądać historię transakcji (HISTORY) dla wybranego konta. Program powstał jako modyfikacja zadania z laboratorium 8, z funkcjami wyciągniętymi poza `main`.


---

## Informacje techniczne

* **Język:** C
* **Edytor:** Microsoft Visual Studio Code
* **Kompilator:** Clang (`clang -Wall`)
* **System:** Windows

> 💡 Program skompilowany przy użyciu kompilatora Clang. Korzystałem z edytora MS Visual Studio Code.

---

## Kompilacja

Aby skompilować program, w terminalu uruchom:

```bash 
clang -Wall bank.c -o bank.exe
```

---

## Uruchomienie programu

Po kompilacji uruchom plik wykonywalny:

```bash
bank.exe
```

Program poprosi o podanie poleceń numerycznych:

* `0` – zakończenie programu
* `1` – wpłata (`DEPOSIT`)
* `2` – wypłata (`WITHDRAW`)
* `3` – wyświetlenie salda (`SHOW`)
* `4` – wyświetlenie historii transakcji (`HISTORY`)

Dla poleceń 1 i 2 należy podać numer konta (0–99) oraz kwotę (≥0).
Dla poleceń 3 i 4 należy podać numer konta.

Program wypisuje komunikaty o powodzeniu operacji lub błędach, np. za mało środków lub niepoprawny numer konta.

---

### Przykłady działania

```
C:\Users\X\Desktop\homework>bank.exe
Podaj polecenie: 1
Podaj numer konta: 94
Tworze nowe konto o ID 94
Podaj imie i nazwisko wlasciciela: Piotr K
Kwota do wplaty: 100
[DEPOSIT] Wplacono 100.00 PLN
```

```
C:\Users\X\Desktop\homework>bank.exe
Podaj polecenie: 2
Podaj numer konta: 94
Kwota do wyplaty: 10
[WITHDRAW] Wyplacono 10.00 PLN
```

```
C:\Users\X\Desktop\homework>bank.exe
Podaj polecenie: 3
Podaj numer konta: 94
[SHOW]: konto 94 (Piotr K) saldo = 90.00
```

```
C:\Users\X\Desktop\homework>bank.exe
Podaj polecenie: 4
Podaj numer konta: 94
=== HISTORIA TRANSAKCJI (Piotr K) ===
1. +100.00 (Saldo po: 100.00)
2. -10.00 (Saldo po: 90.00)
==========================================
```

---

## Działanie programu

1. Program prosi o podanie numeru polecenia (0-4)..
2. W zależności od polecenia:

   * `1` – wczytuje numer konta i kwotę, dokonuje wpłaty.
   * `2` – wczytuje numer konta i kwotę, dokonuje wypłaty jeśli środki wystarczą.
   * `3` – wczytuje numer konta i wyświetla saldo.
   * `4` - wczytuje numer konta i wyświetla historię transakcji.
3. Program sprawdza poprawność numeru konta oraz kwoty.
4. Po każdej operacji wypisuje komunikat o wyniku.
5. Pętla powtarza się do momentu wybrania polecenia 0, które kończy program i zapisuje dane do pliku bank_db.txt.
---

## 👤 Autor

* **Imię / Pseudonim:** Piotr
* **Rok:** 2025
* **Środowisko:** Visual Studio Code + Clang

