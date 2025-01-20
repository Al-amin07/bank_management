#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <fstream>
using namespace std;

class Account
{
private:
    int accountNumber;
    string accountHolderName;
    double balance;

public:
    Account(int accNum, string accName, double initialBalance)
    {
        accountNumber = accNum;
        accountHolderName = accName;
        balance = initialBalance;
    }

    void deposit(double amount)
    {
        if (amount > 0)
        {
            balance += amount;
            cout << "Deposit successful. New balance: " << balance << endl;
        }
        else
        {
            cout << "Invalid deposit amount." << endl;
        }
    }

    void withdraw(double amount)
    {
        if (amount > 0 && amount <= balance)
        {
            balance -= amount;
            cout << "Withdrawal successful. Remaining balance: " << balance << endl;
        }
        else
        {
            cout << "Invalid or insufficient amount for withdrawal." << endl;
        }
    }

    void displayDetails() const
    {
        cout << fixed << setprecision(2);
        cout << "\nAccount Number: " << accountNumber << endl;
        cout << "Account Holder: " << accountHolderName << endl;
        cout << "Balance: " << balance << endl;
    }

    int getAccountNumber() const
    {
        return accountNumber;
    }

    string getAccountHolderName() const
    {

        return accountHolderName;
    }

    double getBalance() const
    {
        return balance;
    }
};

class Bank
{
private:
    vector<Account> accounts;
    const string filename = "C:/Users/h/Desktop/C++ Codes/aaa/accounts.txt";

    void saveToFile()
    {

        cout << "Saving accounts to file: " << filename << endl;
        ofstream outFile(filename);
        if (outFile.is_open())
        {
            for (const auto &acc : accounts)
            {

                outFile << acc.getAccountNumber() << "," << acc.getAccountHolderName() << "," << acc.getBalance() << endl;
            }
            outFile.close();
            cout << "Accounts saved successfully to " << filename << endl; // Debug statement
        }
        else
        {
            cout << "Error opening file: " << filename << endl; // Debug statement
        }
    }

    void loadFromFile()
    {
        cout << "Loading accounts from file: " << filename << endl; // Debug statement
        ifstream inFile(filename);
        if (inFile.is_open())
        {
            accounts.clear();
            int accNum;
            string accName;
            double balance;
            string line;
            while (getline(inFile, line))
            {
                size_t pos1 = line.find(',');
                size_t pos2 = line.rfind(',');
                accNum = stoi(line.substr(0, pos1));
                accName = line.substr(pos1 + 1, pos2 - pos1 - 1);
                balance = stod(line.substr(pos2 + 1));
                accounts.emplace_back(accNum, accName, balance);
            }
            inFile.close();
            cout << "Accounts loaded successfully from " << filename << endl; // Debug statement
        }
        else
        {
            cout << "Error opening file: " << filename << endl; // Debug statement
        }
    }

public:
    Bank()
    {
        loadFromFile();
    }

    ~Bank()
    {
        saveToFile();
    }

    void createAccount()
    {
        int accNum;
        string accName;
        double initialBalance;

        cout << "Enter Account Number: ";
        cin >> accNum;
        cout << "Enter Account Holder Name: ";
        cin.ignore();
        getline(cin, accName);
        cout << "Enter Initial Balance: ";
        cin >> initialBalance;

        cout << "Save before the file" << endl; // Corrected debug statement
        accounts.emplace_back(accNum, accName, initialBalance);
        saveToFile();                                        // Save immediately after creating the account
        cout << "Account created and saved successfully!\n"; // Debug statement
    }

    void depositAmount()
    {
        int accNum;
        double amount;
        cout << "Enter Account Number: ";
        cin >> accNum;

        for (auto &acc : accounts)
        {
            if (acc.getAccountNumber() == accNum)
            {
                cout << "Enter amount to deposit: ";
                cin >> amount;
                acc.deposit(amount);
                saveToFile();                                     // Save after deposit
                cout << "Deposit made and saved successfully!\n"; // Debug statement
                return;
            }
        }
        cout << "Account not found!\n";
    }

    void withdrawAmount()
    {
        int accNum;
        double amount;
        cout << "Enter Account Number: ";
        cin >> accNum;

        for (auto &acc : accounts)
        {
            if (acc.getAccountNumber() == accNum)
            {
                cout << "Enter amount to withdraw: ";
                cin >> amount;
                acc.withdraw(amount);
                saveToFile();                                        // Save after withdrawal
                cout << "Withdrawal made and saved successfully!\n"; // Debug statement
                return;
            }
        }
        cout << "Account not found!\n";
    }

    void displayAccount()
    {
        int accNum;
        cout << "Enter Account Number: ";
        cin >> accNum;

        for (const auto &acc : accounts)
        {
            if (acc.getAccountNumber() == accNum)
            {
                acc.displayDetails();
                return;
            }
        }
        cout << "Account not found!\n";
    }

    void listAllAccounts()
    {
        if (accounts.empty())
        {
            cout << "No accounts to display." << endl;
            return;
        }

        cout << "\nAll Accounts:\n";

        sort(accounts.begin(), accounts.end(), [](const Account &a, const Account &b)
             { return a.getAccountHolderName() < b.getAccountHolderName(); });

        for (const auto &acc : accounts)
        {
            acc.displayDetails();
            cout << "-----------------------" << endl;
        }
    }

    void displayFileContents()
    {
        ifstream inFile(filename);
        if (inFile.is_open())
        {
            string line;
            while (getline(inFile, line))
            {
                cout << line << endl;
            }
            inFile.close();
        }
        else
        {
            cout << "Error opening file: " << filename << endl;
        }
    }
};

int main()
{
    Bank bank;
    int choice;

    do
    {
        cout << "\nBank Management System\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit Amount\n";
        cout << "3. Withdraw Amount\n";
        cout << "4. Display Account Details\n";
        cout << "5. List All Accounts\n";
        cout << "6. Display File Contents\n"; // New menu option
        cout << "7. Exit\n";                  // Updated exit option
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            bank.createAccount();
            break;
        case 2:
            bank.depositAmount();
            break;
        case 3:
            bank.withdrawAmount();
            break;
        case 4:
            bank.displayAccount();
            break;
        case 5:
            bank.listAllAccounts();
            break;
        case 6:
            bank.displayFileContents(); // Call the new method
            break;
        case 7:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice! Try again." << endl;
        }
    } while (choice != 7); // Updated exit condition

    return 0;
}
