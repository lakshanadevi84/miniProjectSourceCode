// Advanced Bank Account Management System
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structure definition
struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// function prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);

// Extra Features
void displayAll(FILE *fPtr);
void searchAccount(FILE *fPtr);
void depositMoney(FILE *fPtr);
void withdrawMoney(FILE *fPtr);
void checkBalance(FILE *fPtr);

int main()
{
    FILE *cfPtr;
    unsigned int choice;

    // open file
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("File could not be opened.\n");
        exit(1);
    }

    while ((choice = enterChoice()) != 10)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;

        case 2:
            updateRecord(cfPtr);
            break;

        case 3:
            newRecord(cfPtr);
            break;

        case 4:
            deleteRecord(cfPtr);
            break;

        case 5:
            displayAll(cfPtr);
            break;

        case 6:
            searchAccount(cfPtr);
            break;

        case 7:
            depositMoney(cfPtr);
            break;

        case 8:
            withdrawMoney(cfPtr);
            break;

        case 9:
            checkBalance(cfPtr);
            break;

        default:
            printf("Invalid Choice\n");
        }
    }

    fclose(cfPtr);

    return 0;
}

// MENU
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n========= BANK MENU =========\n");
    printf("1 - Create text file\n");
    printf("2 - Update account\n");
    printf("3 - Add new account\n");
    printf("4 - Delete account\n");
    printf("5 - Display all accounts\n");
    printf("6 - Search account\n");
    printf("7 - Deposit money\n");
    printf("8 - Withdraw money\n");
    printf("9 - Check balance\n");
    printf("10 - Exit\n");
    printf("Enter your choice: ");

    scanf("%u", &choice);

    return choice;
}

// CREATE TEXT FILE
void textFile(FILE *readPtr)
{
    FILE *writePtr;
    struct clientData client = {0, "", "", 0.0};

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        printf("File could not be opened.\n");
    }
    else
    {
        rewind(readPtr);

        fprintf(writePtr, "%-6s%-16s%-12s%s\n",
                "Acct", "Last Name", "First Name", "Balance");

        while (fread(&client, sizeof(struct clientData), 1, readPtr))
        {
            if (client.acctNum != 0)
            {
                fprintf(writePtr, "%-6d%-16s%-12s%.2f\n",
                        client.acctNum,
                        client.lastName,
                        client.firstName,
                        client.balance);
            }
        }

        fclose(writePtr);

        printf("accounts.txt created successfully.\n");
    }
}

// UPDATE ACCOUNT
void updateRecord(FILE *fPtr)
{
    unsigned int account;
    double transaction;
    struct clientData client = {0, "", "", 0.0};

    printf("Enter account number: ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("Current Balance: %.2f\n", client.balance);

        printf("Enter amount (+ deposit / - withdraw): ");
        scanf("%lf", &transaction);

        client.balance += transaction;

        fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);

        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        printf("Updated Balance: %.2f\n", client.balance);
    }
}

// ADD NEW ACCOUNT
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};
    unsigned int accountNum;

    printf("Enter new account number (1-100): ");
    scanf("%u", &accountNum);

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists.\n");
    }
    else
    {
        printf("Enter lastname firstname balance:\n");
        scanf("%14s %9s %lf",
              client.lastName,
              client.firstName,
              &client.balance);

        client.acctNum = accountNum;

        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        printf("Account created successfully.\n");
    }
}

// DELETE ACCOUNT
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter account number to delete: ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
    }
    else
    {
        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&blankClient,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Account deleted successfully.\n");
    }
}

// DISPLAY ALL ACCOUNTS
void displayAll(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\n%-6s%-16s%-12s%s\n",
           "Acct",
           "Last Name",
           "First Name",
           "Balance");

    while (fread(&client,
                 sizeof(struct clientData),
                 1,
                 fPtr))
    {
        if (client.acctNum != 0)
        {
            printf("%-6d%-16s%-12s%.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}

// SEARCH ACCOUNT
void searchAccount(FILE *fPtr)
{
    struct clientData client;
    unsigned int account;

    printf("Enter account number to search: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("\nAccount Found\n");
        printf("Account No : %d\n", client.acctNum);
        printf("Name       : %s %s\n",
               client.firstName,
               client.lastName);
        printf("Balance    : %.2f\n", client.balance);
    }
}

// DEPOSIT MONEY
void depositMoney(FILE *fPtr)
{
    struct clientData client;
    unsigned int account;
    double amount;

    printf("Enter account number: ");
    scanf("%u", &account);

    printf("Enter deposit amount: ");
    scanf("%lf", &amount);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        client.balance += amount;

        fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);

        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        printf("Deposit successful.\n");
        printf("New Balance: %.2f\n", client.balance);
    }
}

// WITHDRAW MONEY
void withdrawMoney(FILE *fPtr)
{
    struct clientData client;
    unsigned int account;
    double amount;

    printf("Enter account number: ");
    scanf("%u", &account);

    printf("Enter withdrawal amount: ");
    scanf("%lf", &amount);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else if (amount > client.balance)
    {
        printf("Insufficient balance.\n");
    }
    else
    {
        client.balance -= amount;

        fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);

        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        printf("Withdrawal successful.\n");
        printf("Remaining Balance: %.2f\n", client.balance);
    }
}

// CHECK BALANCE
void checkBalance(FILE *fPtr)
{
    struct clientData client;
    unsigned int account;

    printf("Enter account number: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("Current Balance: %.2f\n", client.balance);
    }
}