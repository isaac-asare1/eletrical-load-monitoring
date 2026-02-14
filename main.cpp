#include <iostream>
#include <string>
#include <limits>

using namespace std;

// File names 
const string APPLIANCES_FILE = "appliances.txt";
const string BILLING_FILE    = "billing_summary.txt";

// Max appliances 
const int MAX_APPLIANCES = 100;

struct Appliance {
    string name;
    double watts;
    double hours;
};

// Calculates daily energy usage in kWh for one appliance
double dailyKwh(const Appliance& a) {
    return (a.watts / 1000.0) * a.hours;
}


// Trims leading/trailing whitespace
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

// Clears bad input from cin
void clearBadInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Reads an int safely
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

// Reads a double safely
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

// Reads a non-empty line (for names)
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

// Reads a positive double (> 0)
double readPositiveDouble(const string& prompt) {
    while (true) {
        double v = readDouble(prompt);
        if (v > 0) return v;
        cout << "Value must be greater than zero. Try again.\n";
    }
}

// Reads hours in range 0..24
double readHours(const string& prompt) {
    while (true) {
        double h = readDouble(prompt);
        if (h >= 0 && h <= 24) return h;
        cout << "Usage hours must be between 0 and 24. Try again.\n";
    }
}

void showMenu() {
    cout << "\n==================== MAIN MENU ====================\n";
    cout << "1. Register electrical appliance (coming soon)\n";
    cout << "2. View all registered appliances (coming soon)\n";
    cout << "3. Search appliance by name (coming soon)\n";
    cout << "4. Load and energy calculation + billing (coming soon)\n";
    cout << "5. Save appliances to file (coming soon)\n";
    cout << "6. Exit\n";
    cout << "===================================================\n";
}

int main() {
    cout << "Electrical Load Monitoring & Billing System\n";
    cout << "-------------------------------------------\n";

    while (true) {
        showMenu();
        int option = readInt("Choose an option (1-6): ");

        switch (option) {
            case 1: {
                cout << "\n--- Register Appliance (demo input only) ---\n";
                Appliance a;
                a.name  = readNonEmptyLine("Appliance name: ");
                a.watts = readPositiveDouble("Power rating (watts, > 0): ");
                a.hours = readHours("Daily usage hours (0 - 24): ");

                cout << "\nCaptured:\n";
                cout << "Name: " << a.name << "\n";
                cout << "Watts: " << a.watts << "\n";
                cout << "Hours/day: " << a.hours << "\n";
                cout << "kWh/day: " << dailyKwh(a) << "\n";
                break;
            }

            case 2:
                cout << "View appliances feature is not implemented yet.\n";
                break;

            case 3:
                cout << "Search appliance feature is not implemented yet.\n";
                break;

            case 4:
                cout << "Billing feature is not implemented yet.\n";
                break;

            case 5:
                cout << "Save feature is not implemented yet.\n";
                break;

            case 6:
                cout << "Goodbye!\n";
                return 0;

            default:
                cout << "Invalid choice. Please choose between 1 and 6.\n";
                break;
        }
    }

    return 0;
}
