#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

class Account {
private:
    int accountNumber;
    string accountHolderName;
    double balance;
    string bankName;

public:
    Account(int accNum, string accName, double initialBalance, string bName)
        : accountNumber(accNum), accountHolderName(accName), balance(initialBalance), bankName(bName) {}

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposit successful. New balance: " << balance << endl;
        } else {
            cout << "Invalid deposit amount." << endl;
        }
    }

    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "Withdrawal successful. Remaining balance: " << balance << endl;
        } else {
            cout << "Invalid or insufficient amount for withdrawal." << endl;
        }
    }

    void displayDetails() const {
        cout << fixed << setprecision(2);
        cout << "\nBank: " << bankName << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Holder: " << accountHolderName << endl;
        cout << "Balance: " << balance << endl;
    }

    int getAccountNumber() const { return accountNumber; }
    string getBankName() const { return bankName; }
    string getAccountHolderName() const { return accountHolderName; }
    double getBalance() const { return balance; }
    void setBalance(double newBalance) { balance = newBalance; }
};

class Bank {
private:
    vector<Account> accounts;
    const string filename = "C:/Users/h/Pictures/me/new Task/bank_management/accounts.txt";
    void saveToFile() {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            for (const auto& acc : accounts) {
                outFile << acc.getAccountNumber() << "," 
                        << acc.getAccountHolderName() << "," 
                        << acc.getBankName() << "," 
                        << acc.getBalance() << endl;
            }
            outFile.close();
        } else {
            cout << "Error opening file for saving." << endl;
        }
    }

    void loadFromFile() {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            accounts.clear();
            string line;
            while (getline(inFile, line)) {
                stringstream ss(line);
                string accNumStr, accName, bankName, balanceStr;
                
                getline(ss, accNumStr, ',');
                getline(ss, accName, ',');
                getline(ss, bankName, ',');
                getline(ss, balanceStr, ',');

                try {
                    int accNum = stoi(accNumStr);
                    double balance = stod(balanceStr);
                    accounts.emplace_back(accNum, accName, balance, bankName);
                } catch (const invalid_argument&) {
                    cout << "Error: Invalid data in file. Skipping line." << endl;
                }
            }
            inFile.close();
        } else {
            cout << "Error opening file for loading." << endl;
        }
    }

public:
    Bank() { loadFromFile(); }
    ~Bank() { saveToFile(); }

    void createAccount() {
        int accNum;
        string accHolderName, bankName;
        double initialBalance;

        cout << "Enter Account Number: ";
        cin >> accNum;
        cin.ignore();  // Ignore newline character from previous input

        cout << "Enter Account Holder Name: ";
        getline(cin, accHolderName);

        cout << "Enter Bank Name (Islamic Bank / Jamuna Bank): ";
        getline(cin, bankName);

        if (bankName != "Islamic Bank" && bankName != "Jamuna Bank") {
            cout << "Invalid bank name! Please choose 'Islamic Bank' or 'Jamuna Bank'." << endl;
            return;
        }

        cout << "Enter Initial Balance: ";
        cin >> initialBalance;

        accounts.emplace_back(accNum, accHolderName, initialBalance, bankName);
        saveToFile();
        cout << "Account created successfully for " << accHolderName << "!\n";
    }

    void depositAmount() {
        int accNum;
        double amount;
        cout << "Enter Account Number: ";
        cin >> accNum;

        for (auto& acc : accounts) {
            if (acc.getAccountNumber() == accNum) {
                cout << "Enter amount to deposit: ";
                cin >> amount;
                acc.deposit(amount);
                saveToFile();
                return;
            }
        }
        cout << "Account not found!\n";
    }

    void withdrawAmount() {
        int accNum;
        double amount;
        cout << "Enter Account Number: ";
        cin >> accNum;

        for (auto& acc : accounts) {
            if (acc.getAccountNumber() == accNum) {
                cout << "Enter amount to withdraw: ";
                cin >> amount;
                acc.withdraw(amount);
                saveToFile();
                return;
            }
        }
        cout << "Account not found!\n";
    }

    void transferMoney() {
        int fromAcc, toAcc;
        double amount;
        cout << "Enter Your Account Number: ";
        cin >> fromAcc;
        cout << "Enter Receiver's Account Number: ";
        cin >> toAcc;
        cout << "Enter Amount to Transfer: ";
        cin >> amount;

        Account* sender = nullptr;
        Account* receiver = nullptr;

        for (auto& acc : accounts) {
            if (acc.getAccountNumber() == fromAcc) sender = &acc;
            if (acc.getAccountNumber() == toAcc) receiver = &acc;
        }

        if (!sender || !receiver) {
            cout << "Error: One or both accounts not found!" << endl;
            return;
        }

        if (sender->getBalance() < amount) {
            cout << "Insufficient funds!" << endl;
            return;
        }

        sender->setBalance(sender->getBalance() - amount);
        receiver->setBalance(receiver->getBalance() + amount);
        saveToFile();
        cout << "Transfer Successful!\n";
    }

    void displayAccount() {
        int accNum;
        cout << "Enter Account Number: ";
        cin >> accNum;

        for (const auto& acc : accounts) {
            if (acc.getAccountNumber() == accNum) {
                acc.displayDetails();
                return;
            }
        }
        cout << "Account not found!\n";
    }

    void listAllAccounts() {
        if (accounts.empty()) {
            cout << "No accounts to display." << endl;
            return;
        }
        cout << "\nAll Accounts:\n";
        for (const auto& acc : accounts) {
            acc.displayDetails();
            cout << "-----------------------" << endl;
        }
    }

    void displayFileContents() {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                cout << line << endl;
            }
            inFile.close();
        } else {
            cout << "Error opening file: " << filename << endl;
        }
    }
};

int main() {
    Bank bank;
    int choice;

    do {
        cout << "\nBank Management System\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit Amount\n";
        cout << "3. Withdraw Amount\n";
        cout << "4. Transfer Money\n";
        cout << "5. Display Account Details\n";
        cout << "6. List All Accounts\n";
        cout << "7. Display File Contents\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: bank.createAccount(); break;
            case 2: bank.depositAmount(); break;
            case 3: bank.withdrawAmount(); break;
            case 4: bank.transferMoney(); break;
            case 5: bank.displayAccount(); break;
            case 6: bank.listAllAccounts(); break;
            case 7: bank.displayFileContents(); break;
            case 8: cout << "Exiting..." << endl; break;
            default: cout << "Invalid choice! Try again." << endl;
        }
    } while (choice != 8);

    return 0;
}
