#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#define ll long long // декларируем long long
using namespace std;

string IntFormat(int a) {
    string in = to_string(a), out; //in == a, out пустой
    for (int i = 0; i < in.length();) {
        out.push_back(in[i]);
        i++;
        if ((in.length() - i) % 3 == 0 && i != in.length()) out.push_back(',');
    }
    return out;
}

bool isInteger(string x) {// проверка строки на присутствия только цифр
    for (unsigned int i = 0; i < x.size(); i++) {
        if (48 <= x[i] && x[i] <= 57) continue;// '0' == ASCII 48 '9' == ASCII 57
        else return false;
    }
    return true;
}

int toInt(string s) { // "123" -> 123
    int x = 0;
    for (unsigned int i = 0; i < s.size(); i++) {
        if (s[i] == 13) break;
        x = x * 10;
        int y = s[i] - 48;
        x += y;
    }
    return x;
}

class BankUser {
    static int num;
    int id;
    string FullName;
    string PINCode;
    ll Balance;
    int CardNumber;
    int LastTransaction;
    int BankID;
public:
    BankUser()
    {
        this->FullName = "";
        this->PINCode = "";
        this->CardNumber = 0;
        this->Balance = 0;
        this->LastTransaction = 0;
    }
    BankUser(int bank, string name, int number, string pin, ll balance, int last) {
        this->id = num;
        this->num++;
        this->BankID = bank;
        this->FullName = name;
        this->CardNumber = number;
        this->PINCode = pin;
        this->Balance = balance;
        this->LastTransaction = last;
    }
    int getBankID()const {
        return BankID;
    }
    int size() const {
        return num;
    }
    int getid() const {
        return id;
    }
    string getFullName()const {
        return FullName;
    }
    int getCardNumber()const {
        return CardNumber;
    }
    string getPINCode()const {
        return PINCode;
    }
    ll getBalance()const {
        return Balance;
    }

    int getLastTransaction()const {
        return LastTransaction;
    }
    bool isActivated() {// проверка строки на активированность карты
        return isInteger(this->PINCode);
    }
    bool ExitMenu() {
        char ans;
        cout << endl << "Would you like to return to the main menu? (Y/N): ";
        cin >> ans;
        cout << '\n';
        if (ans == 'N' || ans == '1') return 0;
        if (ans == 'Y' || ans == '0') return 1;
        else ExitMenu();
    }
    bool ModifyPinCode();
    bool ShowBalance() {
        double currency = 432.64;
        string s = to_string((double) this->Balance / currency - (int)(this->Balance / currency));
        s = s.substr(1, s.size() - 2);
        cout << "Card owner: " << this->FullName << endl;
        cout << "Card number: " << this->CardNumber << endl;
        cout << "Current balance in KZT: " << IntFormat(this->Balance) << endl;
        cout << "Current balance in USD: " << IntFormat((int) this->Balance / currency) << s << endl;
        cout << "Last transaction: " << this->LastTransaction << " KZT" << endl;
        return ExitMenu();
    }
    bool isPinSame(string pin) {
        if (this->PINCode == pin) return true;
        else return false;
    }
    bool Withdraw();
    bool Deposit();
    bool Transfer();
};
int BankUser::num = 0;

BankUser BU[100];

int Search(int cardnumber) {
    for (BankUser bankuser : BU) {
        if (bankuser.getCardNumber() == 0) break;
        if (bankuser.getCardNumber() == cardnumber)
            return bankuser.getid();
    }
    return -1;
}

string toLine(BankUser bankuser) {
    string line;
    line = to_string(bankuser.getBankID()) + "," + bankuser.getFullName() + "," + to_string(bankuser.getCardNumber()) + "," + bankuser.getPINCode() + "," + to_string(bankuser.getBalance()) + "," + to_string(bankuser.getLastTransaction());
    return line;
}

void updateDB() {
    ofstream empty;
    empty.open("./database.csv", ofstream::out | ofstream::trunc);
    empty.close();
    fstream db;
    db.open("./database.csv", ios::out | ios::app);
    db << "Bank ID, Name, Card Number, PIN, Balance, Last Transaction" << "\n";
    for (int i = 0; i < BU[0].size(); i++) {
        db << toLine(BU[i]) << "\n";
    }
}

bool BankUser::ModifyPinCode() {
    string newPINCode;
    cout << "Enter new PIN code: " << endl;
    cin >> newPINCode;
    if (newPINCode.size() != 4 ||!isInteger(newPINCode) == true) {
        cout << endl;
        cout << "The code should be pure 4-digit code" << endl;
        return ModifyPinCode();
    }
    if (isInteger(this->PINCode) == false) cout << "The card is activated" << endl;
    else cout << "The card is modified";
    this->PINCode = newPINCode;
    updateDB();
    return ExitMenu();
}

bool BankUser::Deposit() {
    int transfer;
    cout << endl << "Current balance: " << IntFormat(this->Balance) << " KZT" << endl;
    cout << "Enter amount to put in: ";
    cin >> transfer;

    this->Balance = Balance + transfer;
    LastTransaction = transfer;
    cout << endl << "Cash is in the card successfully!" << endl;
    cout << "Remaining balance: " << IntFormat(this->Balance) << " KZT" << endl;
    updateDB();
    return ExitMenu();
}

bool BankUser::Withdraw() {
    int transfer;
    cout << endl << "Current balance: " << IntFormat(this->Balance) << " KZT" << endl;
    if (this->Balance == 0) {
        cout << "You cannot withdraw the money" << endl << endl;
        return 1;
    }
    cout << "Enter amount to withdraw: ";
    cin >> transfer;

    if (transfer > this->Balance) {
        cout << "The balance is insufficient" << endl;

        return Withdraw();
    }
    else {
        cout << endl << "Cash withdrawn successfully!" << endl;
        this->Balance = Balance - transfer;
        LastTransaction = -transfer;
        cout << "Remaining balance: " << IntFormat(this->Balance) << " KZT" << endl;
        updateDB();
        return ExitMenu();
    }
}

bool BankUser::Transfer(){
    string snum;
    int toNumber, sum;
    cout << endl << "--Commission for fund transfers over 20,000 KZT is 500 KZT--" << endl;
    cout << endl << "Beneficiarys card number: ";
    cin >> snum;// проверка cardnumber
    if (!isInteger(snum)) {
        cout << "The card must contain only numbers" << endl;
        return Transfer();
    }
    toNumber = toInt(snum);
    int sID = Search(toNumber);
    if (sID == -1) { 
        cout << "\nThe card is not found." << endl;
        char ans;
        cout << endl << "Do you want to continue the transaction? (Y/N): ";
        cin >> ans;
        if (ans == 'N' || ans == '1') return 1;
        if (ans == 'Y' || ans == '0') return Transfer();
        return 0;
    }
    while (!BU[sID].isActivated()) {
        cout << "The card is not activated." << endl << endl;
        cout << "Beneficiary’s card number: ";
        cin >> toNumber;
    }
    cout << "Transfer amount (in KZT): ";
    cin >> sum;
    if (sum < 0) {
        cout << endl << "Transfer amount must exceed 0 KZT" << endl;
        return Transfer();
    }

    if (this->BankID != BU[sID].BankID) sum = sum + 200;

    if (sum > 20000) sum = sum + 500;

    if (this->Balance < sum) {
        cout << "The amount is not enough taking into account the commissions";
        char ans;
        cout << endl << "Do you want to continue the transaction? (Y/N): ";
        cin >> ans;
        if (ans == 'N' || ans == '1') return 1;
        if (ans == 'Y' || ans == '0') return Transfer();
        return 0;
    }

    cout << "Transfer successful!" << endl;
    this->Balance -= sum;
    if (this->BankID != BU[sID].BankID)  sum = sum - 200;
    if (sum > 20500) sum = sum - 500;
    BU[sID].Balance = BU[sID].Balance + sum;
    updateDB();
    return ExitMenu();
}

void Error() {
    cout << endl << "The card number or PIN code is incorrect. Try again!" << endl;
}

int Login() {
    int number;
    string pin, snum;
    cout << "Card number (6 digits): ";
    cin >> snum;
    if (!isInteger(snum)) {
        cout << endl << "The card must contain only numbers" << endl;
        return Login();
    }
    number = toInt(snum);
    int id = Search(number);
    if (id == -1 || -!(100000 <= number && number <= 999999)) {
        Error();
        return -1;
    }
    cout << "PIN code (4 digits): ";
    cin >> pin;
    if (pin.size() != 4) {
        Error();
        return -1;
    }
    if (BU[id].isPinSame(pin)) {
        if (!isInteger(pin)) {
            cout << "The card is new and has to be activated";
            BU[id].ModifyPinCode();
        }
        return id;
    }
    Error();
    return -1;
}

void output(string text, int space) {
    for (int i = 1; i <= space; i++) {
        cout << '\t';
    }
    cout << text;
}

int main() {
    int count = 0;
    fstream db;
    db.open("./database.csv", ios::in);
    string line, word;
    vector<string> row;
    getline(db, line);
    while (getline(db, line)) {
        row.clear();
        stringstream ss(line);

        while (getline(ss, word, ',')) row.push_back(word);
        BU[count] = BankUser(toInt(row[0]), row[1], toInt(row[2]), row[3], toInt(row[4]), toInt(row[5]));
        count++;
    }

    while (1) {
        string title = "Welcome to the Bank Nice_Try";

        output(title, 1);
        cout << endl;

        int ID = -1;

        while (1) {
            ID = Login();
            if (ID == -1) continue;          
            else {
                cout << endl;
                break;
            }
        }

        while (1) {           
            cout << "Select one of the following options: " << endl << endl;

            output("1. Withdraw Cash", 0);
            output("2. Deposit Cash", 2);
            cout << endl;
            output("3. Modify PIN Code", 0);
            output("4. Show Balance", 2);
            cout << endl;
            output("5. Transfer Between Accounts", 0);
            output("6. End Session", 1);
            cout << endl;

            bool sucsess;
            int option;

            cout << endl << "Enter option number: ";
            cin >> option;

            if (option == 1) sucsess = BU[ID].Withdraw();
            else if (option == 2) sucsess = BU[ID].Deposit();
            else if (option == 3) sucsess = BU[ID].ModifyPinCode();
            else if (option == 4) sucsess = BU[ID].ShowBalance();         
            else if (option == 5) sucsess = BU[ID].Transfer();
            else {
                cout << "Goodbye!" << endl;
                break;
            }
            if (!sucsess == true) break;
        }
    }
}