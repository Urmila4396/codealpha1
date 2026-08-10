#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE "accounts.dat"

typedef struct {
    int accountNumber;
    char name[50];
    double balance;
} Account;

void createAccount();
void depositMoney();
void withdrawMoney();
void checkBalance();
int findAccount(int accNum, Account *acc);
void updateAccountInFile(Account acc);

int main() {
    int choice;

    while (1) {
        printf("\n=== BANK MANAGEMENT SYSTEM ===\n");
        printf("1. Create New Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Balance Enquiry\n");
        printf("5. Exit\n");
        printf("Enter your choice (1-5): ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        switch (choice) {
            case 1: createAccount(); break;
            case 2: depositMoney(); break;
            case 3: withdrawMoney(); break;
            case 4: checkBalance(); break;
            case 5:
                printf("Thank you for using our banking system. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Please select between 1 and 5.\n");
        }
    }
    return 0;
}

void createAccount() {
    FILE *file = fopen(DATA_FILE, "ab");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    Account newAcc, temp;

    printf("\nEnter Account Number: ");
    scanf("%d", &newAcc.accountNumber);
    while (getchar() != '\n'); // Clear buffer

    if (findAccount(newAcc.accountNumber, &temp)) {
        printf("Error: Account number %d already exists.\n", newAcc.accountNumber);
        fclose(file);
        return;
    }

    printf("Enter Account Holder Name: ");
    fgets(newAcc.name, sizeof(newAcc.name), stdin);
    newAcc.name[strcspn(newAcc.name, "\n")] = '\0';

    printf("Enter Initial Deposit Amount: ");
    scanf("%lf", &newAcc.balance);

    if (newAcc.balance < 0) {
        printf("Initial deposit cannot be negative.\n");
        fclose(file);
        return;
    }

    if (fwrite(&newAcc, sizeof(Account), 1, file) != 1) {
        printf("Error writing account to file.\n");
    } else {
        printf("Account created successfully!\n");
    }

    fclose(file);
}

void depositMoney() {
    int accNum;
    double amount;
    Account acc;

    printf("\nEnter Account Number: ");
    scanf("%d", &accNum);

    if (!findAccount(accNum, &acc)) {
        printf("Account not found.\n");
        return;
    }

    printf("Account Holder: %s\n", acc.name);
    printf("Current Balance: $%.2f\n", acc.balance);
    printf("Enter amount to deposit: ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("Invalid amount. Deposit must be greater than 0.\n");
        return;
    }

    acc.balance += amount;
    updateAccountInFile(acc);

    printf("Deposit successful! New Balance: $%.2f\n", acc.balance);
}

void withdrawMoney() {
    int accNum;
    double amount;
    Account acc;

    printf("\nEnter Account Number: ");
    scanf("%d", &accNum);

    if (!findAccount(accNum, &acc)) {
        printf("Account not found.\n");
        return;
    }

    printf("Account Holder: %s\n", acc.name);
    printf("Current Balance: $%.2f\n", acc.balance);
    printf("Enter amount to withdraw: ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("Invalid amount. Withdrawal must be greater than 0.\n");
        return;
    }

    if (amount > acc.balance) {
        printf("Insufficient balance. Transaction failed.\n");
        return;
    }

    acc.balance -= amount;
    updateAccountInFile(acc);

    printf("Withdrawal successful! New Balance: $%.2f\n", acc.balance);
}

void checkBalance() {
    int accNum;
    Account acc;

    printf("\nEnter Account Number: ");
    scanf("%d", &accNum);

    if (!findAccount(accNum, &acc)) {
        printf("Account not found.\n");
        return;
    }

    printf("\n--- Account Details ---\n");
    printf("Account Number : %d\n", acc.accountNumber);
    printf("Account Holder : %s\n", acc.name);
    printf("Current Balance: $%.2f\n", acc.balance);
}

int findAccount(int accNum, Account *acc) {
    FILE *file = fopen(DATA_FILE, "rb");
    if (file == NULL) {
        return 0;
    }

    Account temp;
    int found = 0;

    while (fread(&temp, sizeof(Account), 1, file) == 1) {
        if (temp.accountNumber == accNum) {
            *acc = temp;
            found = 1;
            break;
        }
    }

    fclose(file);
    return found;
}

void updateAccountInFile(Account acc) {
    FILE *file = fopen(DATA_FILE, "rb+");
    if (file == NULL) {
        printf("Error opening file for updating.\n");
        return;
    }

    Account temp;
    while (fread(&temp, sizeof(Account), 1, file) == 1) {
        if (temp.accountNumber == acc.accountNumber) {
            fseek(file, -sizeof(Account), SEEK_CUR);
            fwrite(&acc, sizeof(Account), 1, file);
            break;
        }
    }

    fclose(file);
}
