#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <vector>
using namespace std;

// ==================== CONSTANTS ====================
const int TABLE_SIZE = 100;
const int MAX_WEAK_PASSWORDS = 30;
const int MAX_SUGGESTIONS = 20;

// ==================== SIMPLE OUTPUT FUNCTIONS ====================
void printLine() {
    cout << "------------------------------------------------------------\n";
}

void printTitle(string text) {
    printLine();
    cout << "                 " << text << endl;
    printLine();
}

void pressEnter() {
    cout << "\nPress Enter to continue...";
    cin.get();
}

// ==================== UTILITY FUNCTIONS ====================
string toLower(string str) {
    for (int i = 0; i < str.length(); i++) {
        str[i] = tolower(str[i]);
    }
    return str;
}

string maskPassword(string pwd) {
    if (pwd.length() <= 4) {
        return "***";
    }
    return pwd.substr(0, 2) + "***" + pwd.substr(pwd.length() - 2);
}

int stringToNumber(string text) {
    if (text.length() == 0) {
        return -1;
    }

    for (int i = 0; i < text.length(); i++) {
        if (!isdigit(text[i])) {
            return -1;
        }
    }

    return atoi(text.c_str());
}

// ==================== SUGGESTION STRUCTURE ====================
struct SuggestionItem {
    string text;
    int priority; // 1 = Critical, 2 = High, 3 = Medium, 4 = Low
};

// ==================== LINKED LIST NODE FOR HASH TABLE ====================
struct HashNode {
    string password;
    string breachSource;
    int breachYear;
    HashNode* next;

    HashNode(string pwd, string source, int year) {
        password = pwd;
        breachSource = source;
        breachYear = year;
        next = NULL;
    }
};

// ==================== QUEUE ====================
class Queue {
private:
    string arr[10];
    int front;
    int rear;
    int count;

public:
    Queue() {
        front = 0;
        rear = -1;
        count = 0;
    }

    void enqueue(string item) {
        if (count == 10) {
            front = (front + 1) % 10;
            count--;
        }

        rear = (rear + 1) % 10;
        arr[rear] = item;
        count++;
    }

    void display() {
        if (count == 0) {
            cout << "No check history yet.\n";
            return;
        }

        cout << "Last " << count << " passwords checked:\n";
        for (int i = 0; i < count; i++) {
            string pwd = arr[(front + i) % 10];
            cout << i + 1 << ". " << maskPassword(pwd) << endl;
        }
    }
};

// ==================== HASH TABLE ====================
class HashTable {
private:
    HashNode** table;
    int size;
    int totalEntries;

    int hashFunction(string key) {
        int hash = 0;

        for (int i = 0; i < key.length(); i++) {
            hash = (hash * 31 + key[i]) % size;
        }

        return hash;
    }

public:
    HashTable(int tableSize) {
        size = tableSize;
        totalEntries = 0;
        table = new HashNode*[size];

        for (int i = 0; i < size; i++) {
            table[i] = NULL;
        }
    }

    ~HashTable() {
        for (int i = 0; i < size; i++) {
            HashNode* current = table[i];

            while (current != NULL) {
                HashNode* temp = current;
                current = current->next;
                delete temp;
            }
        }

        delete[] table;
    }

    void insert(string password, string source, int year) {
        if (search(password) != NULL) {
            return;
        }

        int index = hashFunction(password);
        HashNode* newNode = new HashNode(password, source, year);

        newNode->next = table[index];
        table[index] = newNode;
        totalEntries++;
    }

    HashNode* search(string password) {
        int index = hashFunction(password);
        HashNode* current = table[index];

        while (current != NULL) {
            if (current->password == password) {
                return current;
            }
            current = current->next;
        }

        return NULL;
    }

    int getTotalEntries() {
        return totalEntries;
    }

    void displayStats() {
        int maxChain = 0;
        int emptyBuckets = 0;

        for (int i = 0; i < size; i++) {
            int chainLength = 0;
            HashNode* current = table[i];

            while (current != NULL) {
                chainLength++;
                current = current->next;
            }

            if (chainLength > maxChain) {
                maxChain = chainLength;
            }

            if (chainLength == 0) {
                emptyBuckets++;
            }
        }

        printTitle("HASH TABLE STATS");
        cout << "Total breached passwords: " << totalEntries << endl;
        cout << "Hash table size: " << size << endl;
        cout << "Empty buckets: " << emptyBuckets << endl;
        cout << "Maximum chain length: " << maxChain << endl;
        printLine();
    }
};

// ==================== PASSWORD REPORT ====================
class PasswordReport {
private:
    string password;
    int score;

    string strengths[10];
    int strengthCount;

    string weaknesses[10];
    int weaknessCount;

    SuggestionItem suggestions[MAX_SUGGESTIONS];
    int suggestionCount;

    bool isBreached;
    HashNode* breachInfo;

    string getTimeToCrack() {
        int finalScore = getScore();

        if (finalScore >= 80) return "100+ years";
        if (finalScore >= 60) return "1-5 years";
        if (finalScore >= 40) return "1-6 months";
        if (finalScore >= 20) return "1-30 days";
        return "Seconds";
    }

    string getRating() {
        int finalScore = getScore();

        if (finalScore >= 80) return "EXCELLENT";
        if (finalScore >= 60) return "GOOD";
        if (finalScore >= 40) return "FAIR";
        if (finalScore >= 20) return "WEAK";
        return "CRITICAL";
    }

    string getPriorityLabel(int priority) {
        if (priority == 1) return "[CRITICAL]";
        if (priority == 2) return "[HIGH]";
        if (priority == 3) return "[MEDIUM]";
        if (priority == 4) return "[LOW]";
        return "[INFO]";
    }

public:
    PasswordReport(string pwd = "") {
        password = pwd;
        score = 0;
        strengthCount = 0;
        weaknessCount = 0;
        suggestionCount = 0;
        isBreached = false;
        breachInfo = NULL;
    }

    void addStrength(string s) {
        if (strengthCount < 10) {
            strengths[strengthCount] = s;
            strengthCount++;
        }
    }

    void addWeakness(string w) {
        if (weaknessCount < 10) {
            weaknesses[weaknessCount] = w;
            weaknessCount++;
        }
    }

    void addSuggestionWithPriority(string text, int priority) {
        if (suggestionCount < MAX_SUGGESTIONS) {
            suggestions[suggestionCount].text = text;
            suggestions[suggestionCount].priority = priority;
            suggestionCount++;
        }
    }

    void addScore(int points) {
        score = score + points;
    }

    void deductScore(int points) {
        score = score - points;
    }

    void setBreached(HashNode* info) {
        isBreached = true;
        breachInfo = info;
    }

    int getScore() {
        int finalScore = score;

        if (isBreached == true) {
            finalScore = finalScore - 30;
        }

        if (finalScore < 0) {
            finalScore = 0;
        }

        if (finalScore > 100) {
            finalScore = 100;
        }

        return finalScore;
    }

    bool isPasswordBreached() {
        return isBreached;
    }

    HashNode* getBreachInfo() {
        return breachInfo;
    }

    void sortSuggestionsByPriority() {
        for (int i = 0; i < suggestionCount - 1; i++) {
            for (int j = 0; j < suggestionCount - i - 1; j++) {
                if (suggestions[j].priority > suggestions[j + 1].priority) {
                    SuggestionItem temp = suggestions[j];
                    suggestions[j] = suggestions[j + 1];
                    suggestions[j + 1] = temp;
                }
            }
        }
    }

    void display() {
        printTitle("PASSWORD BREACH REPORT");

        cout << "Password: " << maskPassword(password) << endl;
        cout << "Length: " << password.length() << " characters" << endl;
        printLine();

        if (strengthCount > 0) {
            cout << "STRENGTHS:\n";
            for (int i = 0; i < strengthCount; i++) {
                cout << "  + " << strengths[i] << endl;
            }
            printLine();
        }

        if (weaknessCount > 0) {
            cout << "WEAKNESSES:\n";
            for (int i = 0; i < weaknessCount; i++) {
                cout << "  - " << weaknesses[i] << endl;
            }
            printLine();
        }

        if (suggestionCount > 0) {
            cout << "SUGGESTIONS:\n";
            for (int i = 0; i < suggestionCount; i++) {
                cout << "  " << getPriorityLabel(suggestions[i].priority) << " ";
                cout << suggestions[i].text << endl;
            }
            printLine();
        }

        cout << "Security Score: " << getScore() << "/100" << endl;
        cout << "Strength: " << getRating() << endl;
        cout << "Estimated Time To Crack: " << getTimeToCrack() << endl;
        printLine();

        if (isBreached == true && breachInfo != NULL) {
            cout << "BREACH ALERT!\n";
            cout << "This password was found in: " << breachInfo->breachSource << endl;
            cout << "Breach year: " << breachInfo->breachYear << endl;
            cout << "Action Required: Change this password immediately.\n";
        } else if (getScore() < 40) {
            cout << "WARNING: This password is weak.\n";
            cout << "You should create a stronger password.\n";
        } else {
            cout << "SAFE: This password was not found in the breach database.\n";
        }

        printLine();
    }
};

// ==================== BREACH MONITOR ====================
class BreachMonitor {
private:
    string commonWeakPasswords[MAX_WEAK_PASSWORDS];
    int weakPasswordCount;

    HashTable* breachedPasswords;
    Queue checkHistory;

    int totalChecks;
    int breachedCount;
    int safeCount;

    void loadCommonWeakPasswords() {
        string weakList[] = {
            "password", "123456", "qwerty", "admin", "welcome",
            "letmein", "monkey", "dragon", "master", "login",
            "passw0rd", "123123", "football", "iloveyou", "admin123"
        };

        weakPasswordCount = 15;

        for (int i = 0; i < weakPasswordCount; i++) {
            commonWeakPasswords[i] = weakList[i];
        }

        cout << "Loaded " << weakPasswordCount << " common weak passwords.\n";
    }

    void loadBreachedPasswordsFromFile() {
        ifstream file("breached.txt");

        if (!file.is_open()) {
            cout << "breached.txt not found. Creating default file...\n";

            ofstream newFile("breached.txt");
            newFile << "password123,RockYou2021,2021\n";
            newFile << "qwerty123,Collection #1,2019\n";
            newFile << "admin2020,Apify Breach,2020\n";
            newFile << "letmein2023,DarkWeb Scrape,2023\n";
            newFile << "welcome123,LinkedIn Breach,2016\n";
            newFile.close();

            breachedPasswords->insert("password123", "RockYou2021", 2021);
            breachedPasswords->insert("qwerty123", "Collection #1", 2019);
            breachedPasswords->insert("admin2020", "Apify Breach", 2020);
            breachedPasswords->insert("letmein2023", "DarkWeb Scrape", 2023);
            breachedPasswords->insert("welcome123", "LinkedIn Breach", 2016);
            return;
        }

        string line;
        int count = 0;

        while (getline(file, line)) {
            if (line.length() == 0) {
                continue;
            }

            stringstream ss(line);
            string password;
            string breachSource;
            string yearStr;

            getline(ss, password, ',');
            getline(ss, breachSource, ',');
            getline(ss, yearStr, ',');

            if (password.length() > 0 && yearStr.length() > 0) {
                int year = atoi(yearStr.c_str());
                breachedPasswords->insert(password, breachSource, year);
                count++;
            }
        }

        file.close();
        cout << "Loaded " << count << " breached passwords from breached.txt.\n";
    }

    bool searchInWeakPasswordArray(string password) {
        string lowerPassword = toLower(password);

        for (int i = 0; i < weakPasswordCount; i++) {
            if (commonWeakPasswords[i] == lowerPassword) {
                return true;
            }
        }

        return false;
    }

    void checkLength(string password, PasswordReport& report) {
        int len = password.length();

        if (len >= 12) {
            report.addStrength("12 or more characters (+30)");
            report.addScore(30);
        } else if (len >= 8) {
            report.addStrength("8 to 11 characters (+20)");
            report.addScore(20);
        } else {
            report.addWeakness("Password is too short");
            report.addSuggestionWithPriority("Use at least 12 characters", 4);
        }
    }

    void checkComplexity(string password, PasswordReport& report) {
        bool hasUpper = false;
        bool hasLower = false;
        bool hasDigit = false;
        bool hasSpecial = false;

        string specialChars = "!@#$%^&*()";

        for (int i = 0; i < password.length(); i++) {
            char c = password[i];

            if (isupper(c)) {
                hasUpper = true;
            } else if (islower(c)) {
                hasLower = true;
            } else if (isdigit(c)) {
                hasDigit = true;
            } else if (specialChars.find(c) != string::npos) {
                hasSpecial = true;
            }
        }

        int complexity = 0;
        if (hasUpper) complexity++;
        if (hasLower) complexity++;
        if (hasDigit) complexity++;
        if (hasSpecial) complexity++;

        if (complexity == 4) {
            report.addStrength("Uses uppercase, lowercase, numbers, and symbols (+30)");
            report.addScore(30);
        } else if (complexity == 3) {
            report.addStrength("Uses 3 different character types (+20)");
            report.addScore(20);
        } else if (complexity == 2) {
            report.addWeakness("Only 2 character types used (+10)");
            report.addScore(10);
            report.addSuggestionWithPriority("Add more character types", 3);
        } else {
            report.addWeakness("Only 1 character type used");
            report.addSuggestionWithPriority("Use uppercase, lowercase, numbers, and symbols", 3);
        }
    }

    void updateStats(bool breached) {
        totalChecks++;

        if (breached == true) {
            breachedCount++;
        } else {
            safeCount++;
        }
    }

public:
    BreachMonitor() {
        weakPasswordCount = 0;
        totalChecks = 0;
        breachedCount = 0;
        safeCount = 0;

        breachedPasswords = new HashTable(TABLE_SIZE);

        loadCommonWeakPasswords();
        loadBreachedPasswordsFromFile();
    }

    ~BreachMonitor() {
        delete breachedPasswords;
    }

    PasswordReport checkSinglePassword(string password) {
        PasswordReport report(password);

        checkLength(password, report);
        checkComplexity(password, report);

        if (searchInWeakPasswordArray(password) == true) {
            report.addWeakness("This is a common weak password (-20)");
            report.addSuggestionWithPriority("Avoid common words like password, admin, or qwerty", 2);
            report.addSuggestionWithPriority("Use a unique password", 2);
            report.deductScore(20);
        }

        HashNode* breach = breachedPasswords->search(password);

        if (breach != NULL) {
            report.addWeakness("Found in breach database (-30)");
            report.addSuggestionWithPriority("Change this password immediately", 1);
            report.addSuggestionWithPriority("Never reuse this password", 1);
            report.addSuggestionWithPriority("Use a password manager", 1);
            report.setBreached(breach);
        }

        report.sortSuggestionsByPriority();
        checkHistory.enqueue(password);
        updateStats(report.isPasswordBreached());

        return report;
    }

    void checkMultiplePasswords() {
        printTitle("BULK PASSWORD CHECK");

        cout << "Create a file named my_passwords.txt.\n";
        cout << "Put one password on each line.\n\n";

        ifstream file("my_passwords.txt");

        if (!file.is_open()) {
            cout << "my_passwords.txt not found. Creating sample file...\n";

            ofstream sample("my_passwords.txt");
            sample << "password123\n";
            sample << "mySecurePass@2024\n";
            sample << "qwerty123\n";
            sample << "CorrectHorseBatteryStaple\n";
            sample.close();

            cout << "Sample file created. Add your passwords and run again.\n";
            return;
        }

        string password;
        int total = 0;
        int breached = 0;
        int weak = 0;
        int strong = 0;

        vector<string> breachedList;
        vector<string> weakList;

        while (getline(file, password)) {
            if (password.length() == 0) {
                continue;
            }

            total++;
            PasswordReport report = checkSinglePassword(password);
            int score = report.getScore();

            cout << total << ". " << maskPassword(password) << " -> ";

            if (report.isPasswordBreached() == true) {
                cout << "BREACHED in " << report.getBreachInfo()->breachSource << endl;
                breached++;
                breachedList.push_back(password);
            } else if (score < 40) {
                cout << "WEAK, Score: " << score << "/100" << endl;
                weak++;
                weakList.push_back(password);
            } else {
                cout << "SAFE, Score: " << score << "/100" << endl;
                strong++;
            }
        }

        file.close();

        printTitle("BULK CHECK SUMMARY");
        cout << "Total checked: " << total << endl;
        cout << "Breached passwords: " << breached << endl;
        cout << "Weak passwords: " << weak << endl;
        cout << "Strong/Safe passwords: " << strong << endl;
        printLine();

        if (breached > 0) {
            cout << "Change these breached passwords immediately:\n";
            for (int i = 0; i < breachedList.size() && i < 10; i++) {
                cout << "  - " << maskPassword(breachedList[i]) << endl;
            }
            printLine();
        }

        if (weak > 0) {
            cout << "These passwords are weak:\n";
            for (int i = 0; i < weakList.size() && i < 5; i++) {
                cout << "  - " << maskPassword(weakList[i]) << endl;
            }
            printLine();
        }
    }

    string generateStrongPassword() {
        srand(time(0));

        string words[] = {
            "Horse", "Battery", "Staple", "Correct", "Purple",
            "Tiger", "Eagle", "Phoenix", "Thunder", "Shadow"
        };

        string special = "!@#$%&";
        string numbers = "0123456789";

        string password = "";
        password = password + words[rand() % 10];
        password = password + words[rand() % 10];
        password = password + special[rand() % 6];
        password = password + numbers[rand() % 10];
        password = password + numbers[rand() % 10];

        return password;
    }

    void showBreachStats() {
        breachedPasswords->displayStats();

        cout << "Sample breached passwords:\n";
        string sampleBreaches[] = {"password123", "qwerty123", "admin2020", "letmein2023"};

        for (int i = 0; i < 4; i++) {
            HashNode* found = breachedPasswords->search(sampleBreaches[i]);

            if (found != NULL) {
                cout << "  - " << sampleBreaches[i] << " found in " << found->breachSource << endl;
            }
        }

        printLine();
    }

    void showHistory() {
        printTitle("CHECK HISTORY");
        checkHistory.display();
        printLine();
    }

    void showStats() {
        printTitle("MY STATISTICS");

        cout << "Total checks performed: " << totalChecks << endl;
        cout << "Breached passwords found: " << breachedCount << endl;
        cout << "Safe passwords: " << safeCount << endl;

        if (totalChecks > 0) {
            float breachPercent = (float)breachedCount / totalChecks * 100;
            cout << "Breach rate: " << breachPercent << "%" << endl;
        }

        if (breachedCount > 0) {
            cout << "Recommendation: Change your breached passwords immediately.\n";
        }

        printLine();
    }

    void showRecommendations() {
        printTitle("SECURITY RECOMMENDATIONS");

        cout << "1. Use different passwords for different accounts.\n";
        cout << "2. Enable 2-Factor Authentication.\n";
        cout << "3. Never share passwords with anyone.\n";
        cout << "4. Avoid names, birthdays, and common words.\n";
        cout << "5. Use 12 or more characters.\n";
        cout << "6. Mix uppercase, lowercase, numbers, and symbols.\n";
        cout << "7. Use a password manager.\n";
        cout << "8. Never reuse old passwords.\n";
        cout << "9. Change breached passwords immediately.\n";
        cout << "10. Keep your recovery email secure.\n";

        printLine();
    }
};

// ==================== MAIN MENU ====================
void showMenu() {
    printTitle("SECUREPASS - BREACH MONITOR");
    cout << "1. Check Single Password\n";
    cout << "2. Check Multiple Passwords From File\n";
    cout << "3. Generate Strong Password\n";
    cout << "4. View Breach Database Stats\n";
    cout << "5. View Check History\n";
    cout << "6. View My Statistics\n";
    cout << "7. Security Recommendations\n";
    cout << "8. Exit\n";
    printLine();
    cout << "Enter your choice: ";
}

int main() {
    BreachMonitor monitor;
    string choiceText;
    string password;

    while (true) {
        showMenu();
        getline(cin, choiceText);

        int choice = stringToNumber(choiceText);

        if (choice == 1) {
            cout << "\nEnter password to check: ";
            getline(cin, password);

            if (password.length() == 0) {
                cout << "Password cannot be empty.\n";
            } else {
                PasswordReport report = monitor.checkSinglePassword(password);
                report.display();
            }

            pressEnter();
        } else if (choice == 2) {
            monitor.checkMultiplePasswords();
            pressEnter();
        } else if (choice == 3) {
            printTitle("GENERATED STRONG PASSWORD");
            cout << "Password: " << monitor.generateStrongPassword() << endl;
            printLine();
            pressEnter();
        } else if (choice == 4) {
            monitor.showBreachStats();
            pressEnter();
        } else if (choice == 5) {
            monitor.showHistory();
            pressEnter();
        } else if (choice == 6) {
            monitor.showStats();
            pressEnter();
        } else if (choice == 7) {
            monitor.showRecommendations();
            pressEnter();
        } else if (choice == 8) {
            printTitle("GOODBYE");
            cout << "Thank you for using SecurePass. Stay secure!\n";
            printLine();
            break;
        } else {
            cout << "Invalid choice. Please enter a number from 1 to 8.\n";
            pressEnter();
        }
    }

    return 0;
}
