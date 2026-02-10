#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;


// ------------------------------
const string APPLIANCES_FILE = "appliances.txt";
const string BILLING_FILE    = "billing_summary.txt";

// ------------------------------
// Data Model
// ------------------------------
struct Appliance {
    string name;   // must not be empty
    double watts;  // > 0
    double hours;  // 0..24 (daily usage)

    double dailyKwh() const {
        return (watts / 1000.0) * hours;
    }
};

// ------------------------------
// Small helpers
// ------------------------------
static inline string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

void clearBadInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt(const string& prompt) {
    while (true) {
        cout << prompt;
        int x;
        if (cin >> x) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        cout << "Invalid number. Try again.\n";
        clearBadInput();
    }
}

double readDouble(const string& prompt) {
    while (true) {
        cout << prompt;
        double x;
        if (cin >> x) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        cout << "Invalid number. Try again.\n";
        clearBadInput();
    }
}

double readDoubleInRange(const string& prompt, double minVal, double maxVal) {
    while (true) {
        double v = readDouble(prompt);
        if (v >= minVal && v <= maxVal) return v;
        cout << "Value must be between " << minVal << " and " << maxVal << ". Try again.\n";
    }
}

double readPositiveDouble(const string& prompt) {
    while (true) {
        double v = readDouble(prompt);
        if (v > 0) return v;
        cout << "Value must be greater than 0. Try again.\n";
    }
}

string readNonEmptyLine(const string& prompt) {
    while (true) {
        cout << prompt;
        string s;
        getline(cin, s);
        s = trim(s);
        if (!s.empty()) return s;
        cout << "Input must not be empty. Try again.\n";
    }
}

// Case-insensitive compare (simple)
string toLowerStr(string s) {
    for (char& c : s) c = (char)tolower((unsigned char)c);
    return s;
}

// ------------------------------
// File I/O: load + save appliances
// Format per line:
// name|watts|hours
// ------------------------------
bool loadAppliances(vector<Appliance>& appliances) {
    appliances.clear();

    ifstream fin(APPLIANCES_FILE);
    if (!fin.is_open()) {
        // File may not exist on first run; that's okay.
        return false;
    }

    string line;
    while (getline(fin, line)) {
        line = trim(line);
        if (line.empty()) continue;

        // Expect name|watts|hours
        stringstream ss(line);
        string name, wattsStr, hoursStr;

        if (!getline(ss, name, '|')) continue;
        if (!getline(ss, wattsStr, '|')) continue;
        if (!getline(ss, hoursStr, '|')) continue;

        name = trim(name);
        wattsStr = trim(wattsStr);
        hoursStr = trim(hoursStr);

        // Basic safety parse
        try {
            double w = stod(wattsStr);
            double h = stod(hoursStr);

            // Only load valid records
            if (!name.empty() && w > 0 && h >= 0 && h <= 24) {
                appliances.push_back({name, w, h});
            }
        } catch (...) {
            // Skip malformed lines
            continue;
        }
    }
    fin.close();
    return true;
}


void printHeader(const string& title) {
    cout << "\n===============================================================\n";
    cout << title << "\n";
    cout << "===============================================================\n";
}

void registerAppliance(vector<Appliance>& appliances) {
    printHeader("Register Appliance");

    Appliance a;
    a.name  = readNonEmptyLine("Appliance name: ");
    a.watts = readPositiveDouble("Power rating (watts, > 0): ");
    a.hours = readDoubleInRange("Daily usage time (hours, 0 - 24): ", 0, 24);

    appliances.push_back(a);

    if (saveAppliances(appliances)) {
        cout << "✅ Appliance registered and saved.\n";
    } else {
        cout << "⚠️ Registered, but failed to save to file.\n";
    }
}


// Main program
// ------------------------------
int main() {
    vector<Appliance> appliances;

    // Load saved appliance data at startup
    loadAppliances(appliances);

    cout << "\n\nElectrical Load Monitoring & Billing System\n";
    cout << "Loaded appliances: " << appliances.size() << "\n";

    while (true) {
        
        int option = readInt("Choose an option (1-6): ");

        switch (option) {
            case 1:
                registerAppliance(appliances);
                break;
            case 2:
                //view appliances
                break;
            case 3:
                //search appliances
                break;
                case 4:
                //billing menu
                break;
                case 5:
                //save appliances
                break;
            case 6:
               //exit

            default:
                // Gracefully handle invalid menu choices
                cout << "Invalid choice. Please choose between 1 and 6.\n";
                break;
        }
    }
}
