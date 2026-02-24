#include <iostream>
#include <string>
#include <limits>

using namespace std;

const string APPLIANCES_FILE = "appliances.txt";
const string BILLING_FILE    = "billing_summary.txt";
const int MAX_APPLIANCES = 100;

struct Appliance {
    string name;
    double watts;
    double hours;
};

double dailyKwh(const Appliance& a) {
    return (a.watts / 1000.0) * a.hours;
}

string trim(const string& s) {
    int start = 0;
    while (start < (int)s.size() &&
          (s[start] == ' ' || s[start] == '\t' || s[start] == '\r' || s[start] == '\n')) {
        start++;
    }

    int end = (int)s.size() - 1;
    while (end >= 0 &&
          (s[end] == ' ' || s[end] == '\t' || s[end] == '\r' || s[end] == '\n')) {
        end--;
    }

    if (end < start) return "";
    return s.substr(start, end - start + 1);
}

void clearBadInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt(const string& prompt) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        cout << "Invalid number. Try again.\n";
        clearBadInput();
    }
}

double readDouble(const string& prompt) {
    double x;
    while (true) {
        cout << prompt;
        if (cin >> x) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        cout << "Invalid number. Try again.\n";
        clearBadInput();
    }
}

string readNonEmptyLine(const string& prompt) {
    while (true) {
        cout << prompt;
        string s;
        getline(cin, s);
        s = trim(s);
        if (s != "") return s;
        cout << "Input must not be empty. Try again.\n";
    }
}

double readPositiveDouble(const string& prompt) {
    while (true) {
        double v = readDouble(prompt);
        if (v > 0) return v;
        cout << "Value must be greater than zero. Try again.\n";
    }
}

double readHours(const string& prompt) {
    while (true) {
        double h = readDouble(prompt);
        if (h >= 0 && h <= 24) return h;
        cout << "Usage hours must be between 0 and 24. Try again.\n";
    }
}

void printHeader(const string& title) {
    cout << "\n====================================================\n";
    cout << title << "\n";
    cout << "====================================================\n";
}

void showMenu() {
    cout << "\n==================== MAIN MENU ====================\n";
    cout << "1. Register electrical appliance\n";
    cout << "2. View all registered appliances\n";
    cout << "3. Search appliance by name\n";
    cout << "4. Load and energy calculation + billing\n";
    cout << "5. Save appliances to file\n";
    cout << "6. Exit\n";
    cout << "===================================================\n";
}

void registerAppliance(Appliance appliances[], int& count) {
    printHeader("Register Appliance");

    if (count >= MAX_APPLIANCES) {
        cout << "Cannot add more appliances (limit reached).\n";
        return;
    }

    Appliance a;
    a.name = readNonEmptyLine("Appliance name: ");
    a.watts = readPositiveDouble("Power rating (watts, > 0): ");
    a.hours = readHours("Daily usage hours (0 - 24): ");

    appliances[count] = a;
    count++;

    cout << "Appliance registered.\n";
}

int main() {
    Appliance appliances[MAX_APPLIANCES];
    int count = 0;

    printHeader("Electrical Load Monitoring & Billing System");
    cout << "Registered appliances: " << count << "\n";

    while (true) {
        showMenu();
        int option = readInt("Choose an option (1-6): ");

        switch (option) {
            case 1:
                registerAppliance(appliances, count);
                break;

            case 2:
                printHeader("View Appliances");
                cout << "This feature will be implemented in the next part.\n";
                break;

            case 3:
                printHeader("Search Appliance");
                cout << "This feature will be implemented in a later part.\n";
                break;

            case 4:
                printHeader("Billing");
                cout << "This feature will be implemented in a later part.\n";
                break;

            case 5:
                printHeader("Save Appliances");
                cout << "This feature will be implemented in a later part.\n";
                break;

            case 6:
                printHeader("Exit");
                cout << "Goodbye!\n";
                return 0;

            default:
                cout << "Invalid choice. Please choose between 1 and 6.\n";
                break;
        }
    }

    return 0;
}