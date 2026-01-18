#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define OWNER_NAME_LEN 50
#define MAX_HISTORY 100
#define DATA_FILE "bank_db.txt"

typedef struct {
    int id;
    char owner[OWNER_NAME_LEN];
    double balance;
    double history[MAX_HISTORY];
    int history_count;
} BankAccount;

typedef enum {
    ACTION_EXIT = 0,
    ACTION_DEPOSIT = 1,
    ACTION_WITHDRAW = 2,
    ACTION_SHOW = 3,
    ACTION_HISTORY = 4
} BankAction;

/* deklaracje funkcji */
void print_menu(void);
BankAccount* find_account(BankAccount *db, int size, int id);
void deposit(BankAccount *acc);
void withdraw(BankAccount *acc);
void show_balance(BankAccount *acc);
void show_history(BankAccount *acc);
int load_accounts(BankAccount db[], int size);
void save_accounts(BankAccount db[], int size);

int main(void) {
    BankAccount accounts[MAX_ACCOUNTS] = {0};
    int account_count = load_accounts(accounts, MAX_ACCOUNTS);
    BankAction action;
    int id;
    BankAccount *acc;

    print_menu();

    while (1) {
        printf("\nPodaj polecenie: ");
        if (scanf("%d", (int*)&action) != 1) {
            printf("Nieprawidlowe polecenie.\n");
            while(getchar() != '\n'); // czyszczenie bufora
            continue;
        }

        if (action == ACTION_EXIT) {
            save_accounts(accounts, account_count);
            printf("Koniec programu. Dane zapisane.\n");
            break;
        }

        printf("Podaj numer konta: ");
        if (scanf("%d", &id) != 1) {
            printf("Nieprawidlowy numer konta.\n");
            while(getchar() != '\n');
            continue;
        }

        acc = find_account(accounts, account_count, id);

        if (!acc) {
            if (account_count >= MAX_ACCOUNTS) {
                printf("Nie mozna utworzyc nowego konta - brak miejsca.\n");
                continue;
            }
            acc = &accounts[account_count];
            acc->id = id;
            acc->balance = 0;
            acc->history_count = 0;
            printf("Tworze nowe konto o ID: %d\n", id);
            printf("Podaj imie i nazwisko wlasciciela: ");
            scanf(" %49[^\n]", acc->owner);
            account_count++;
        }

        switch (action) {
            case ACTION_DEPOSIT:
                deposit(acc);
                break;
            case ACTION_WITHDRAW:
                withdraw(acc);
                break;
            case ACTION_SHOW:
                show_balance(acc);
                break;
            case ACTION_HISTORY:
                show_history(acc);
                break;
            default:
                printf("Nieznane polecenie.\n");
        }
    }

    return 0;
}

/* Funkcje operacyjne */

void print_menu(void) {
    printf("Symulacja aplikacji bankowej.\n");
    printf("Polecenia:\n");
    printf("0 - zakoncz program\n");
    printf("1 - wplata (DEPOSIT)\n");
    printf("2 - wyplata (WITHDRAW)\n");
    printf("3 - wyswietlenie salda (SHOW)\n");
    printf("4 - historia transakcji (HISTORY)\n");
}

BankAccount* find_account(BankAccount *db, int size, int id) {
    for (int i = 0; i < size; i++) {
        if (db[i].id == id)
            return &db[i];
    }
    return NULL;
}

void deposit(BankAccount *acc) {
    double amount;
    printf("Kwota do wplaty: ");
    if (scanf("%lf", &amount) != 1 || amount < 0) {
        printf("Bledna kwota.\n");
        while(getchar() != '\n');
        return;
    }
    acc->balance += amount;
    if (acc->history_count < MAX_HISTORY)
        acc->history[acc->history_count++] = amount;
    printf("[DEPOSIT] Wplacono %.2lf PLN\n", amount);
}

void withdraw(BankAccount *acc) {
    double amount;
    printf("Kwota do wyplaty: ");
    if (scanf("%lf", &amount) != 1 || amount < 0) {
        printf("Bledna kwota.\n");
        while(getchar() != '\n');
        return;
    }
    if (acc->balance >= amount) {
        acc->balance -= amount;
        if (acc->history_count < MAX_HISTORY)
            acc->history[acc->history_count++] = -amount;
        printf("[WITHDRAW] Wyplacono %.2lf PLN\n", amount);
    } else {
        printf("WITHDRAW ERROR: za malo srodkow na koncie %d\n", acc->id);
    }
}

void show_balance(BankAccount *acc) {
    printf("[SHOW]: konto %d (%s) saldo = %.2lf\n",
           acc->id, acc->owner, acc->balance);
}

void show_history(BankAccount *acc) {
    if (acc->history_count == 0) {
        printf("Brak historii transakcji dla konta %s\n", acc->owner);
        return;
    }

    printf("=== HISTORIA TRANSAKCJI (%s) ===\n", acc->owner);

    double running_balance = 0;
    for (int i = 0; i < acc->history_count; i++) {
        running_balance += acc->history[i];
        printf("%d. %s%.2lf (Saldo po: %.2lf)\n", i + 1,
               acc->history[i] >= 0 ? "+" : "", acc->history[i], running_balance);
    }

    printf("==========================================\n");
}

/* Funkcje plikowe */

int load_accounts(BankAccount db[], int size) {
    FILE *f = fopen(DATA_FILE, "r");
    int count = 0;
    if (!f) {
        printf("Brak pliku danych. Inicjalizacja kont 'na sztywno'.\n");
        db[0].id = 0; strcpy(db[0].owner, "Jan Kowalski"); db[0].balance = 350; db[0].history_count = 2;
        db[0].history[0] = -50; db[0].history[1] = 100;
        db[1].id = 1; strcpy(db[1].owner, "Anna Nowak"); db[1].balance = 500; db[1].history_count = 0;
        return 2;
    }

    char line[512];
    while (count < size && fgets(line, sizeof(line), f)) {
        BankAccount *acc = &db[count];
        acc->history_count = 0;

        char *token = strtok(line, "|");
        if (!token) continue;
        acc->id = atoi(token);

        token = strtok(NULL, "|");
        if (!token) continue;
        strncpy(acc->owner, token, OWNER_NAME_LEN);
        acc->owner[OWNER_NAME_LEN-1] = '\0';

        token = strtok(NULL, "|");
        if (!token) continue;
        acc->balance = atof(token);

        token = strtok(NULL, "|");
        if (!token) continue;
        acc->history_count = atoi(token);

        token = strtok(NULL, "|"); // reszta = historia
        if (token) {
            char *hist_ptr = token;
            for (int i = 0; i < acc->history_count; i++) {
                if (!hist_ptr) break;
                acc->history[i] = atof(hist_ptr);
                hist_ptr = strchr(hist_ptr, ' ');
                if (hist_ptr) hist_ptr++;
            }
        }

        count++;
    }

    fclose(f);
    printf("Wczytano %d kont z pliku.\n", count);
    return count;
}

void save_accounts(BankAccount db[], int size) {
    FILE *f = fopen(DATA_FILE, "w");
    if (!f) {
        printf("Blad zapisu pliku.\n");
        return;
    }
    for (int i = 0; i < size; i++) {
        if (db[i].id >= 0) {
            fprintf(f, "%d|%s|%.2lf|%d", db[i].id, db[i].owner, db[i].balance, db[i].history_count);
            for (int j = 0; j < db[i].history_count; j++)
                fprintf(f, " %.2lf", db[i].history[j]);
            fprintf(f, "\n");
        }
    }
    fclose(f);
}
