// ===============================================================
// electrical-load-monitoring/main.cpp
// Console-based Electrical Load Monitoring + Billing (C++)
// Data storage: text files using fstream
//
// Files used:
// - appliances.txt         (stores appliance records)
// - billing_summary.txt    (stores billing summaries; appended)
//
// NOTE:
// - Usage hours are treated as DAILY usage hours (0..24).
// - Energy (kWh) = (Watts / 1000) * Hours
// ===============================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

// ------------------------------
// Config: file names
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

bool saveAppliances(const vector<Appliance>& appliances) {
    ofstream fout(APPLIANCES_FILE, ios::trunc);
    if (!fout.is_open()) return false;

    for (const auto& a : appliances) {
        fout << a.name << "|" << a.watts << "|" << a.hours << "\n";
    }
    fout.close();
    return true;
  }

// ------------------------------
// UI features
// ------------------------------
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

void viewAppliances(const vector<Appliance>& appliances) {
    printHeader("All Registered Appliances");

    if (appliances.empty()) {
        cout << "No appliances registered yet.\n";
        return;
    }

    cout << left
         << setw(4)  << "#"
         << setw(25) << "Name"
         << setw(12) << "Watts(W)"
         << setw(12) << "Hours/day"
         << setw(12) << "kWh/day"
         << "\n";

    cout << string(65, '-') << "\n";

    cout << fixed << setprecision(2);
    for (size_t i = 0; i < appliances.size(); i++) {
        const auto& a = appliances[i];
        cout << left
             << setw(4)  << (i + 1)
             << setw(25) << a.name
             << setw(12) << a.watts
             << setw(12) << a.hours
             << setw(12) << a.dailyKwh()
             << "\n";
    }
}

void searchAppliance(const vector<Appliance>& appliances) {
    printHeader("Search Appliance By Name");

    if (appliances.empty()) {
        cout << "No appliances registered yet.\n";
        return;
    }

    string query = readNonEmptyLine("Enter name to search: ");
    string q = toLowerStr(trim(query));

    bool found = false;
    cout << fixed << setprecision(2);

    for (const auto& a : appliances) {
        string n = toLowerStr(a.name);
        if (n.find(q) != string::npos) {
            if (!found) {
                cout << "Matches:\n";
                cout << "- " << a.name
                     << " | " << a.watts << " W"
                     << " | " << a.hours << " hrs/day"
                     << " | " << a.dailyKwh() << " kWh/day\n";
            } else {
                cout << "- " << a.name
                     << " | " << a.watts << " W"
                     << " | " << a.hours << " hrs/day"
                     << " | " << a.dailyKwh() << " kWh/day\n";
            }
            found = true;
        }
    }

    if (!found) {
        cout << "No appliance found matching: " << query << "\n";
    }
}

double totalDailyKwh(const vector<Appliance>& appliances) {
    double total = 0.0;
    for (const auto& a : appliances) total += a.dailyKwh();
    return total;
}

void appendBillingSummaryToFile(const string& summaryText) {
    ofstream fout(BILLING_FILE, ios::app);
    if (!fout.is_open()) {
        cout << "⚠️ Could not open billing_summary.txt to save.\n";
        return;
    }
    fout << summaryText << "\n";
    fout.close();
}

void billingMenu(const vector<Appliance>& appliances) {
    printHeader("Billing Calculation");

    if (appliances.empty()) {
        cout << "No appliances registered. Register appliances first.\n";
        return;
    }

    // Tariff must be positive
    double tariff = readPositiveDouble("Enter electricity tariff per kWh (positive): ");

    // Compute totals
    double dailyKwh = totalDailyKwh(appliances);
    double dailyCost = dailyKwh * tariff;

    // Optional monthly estimate (30 days)
    double monthlyKwh = dailyKwh * 30.0;
    double monthlyCost = dailyCost * 30.0;

    // Display summary
    cout << fixed << setprecision(2);
    cout << "\n--- Detailed Billing Summary ---\n";
    cout << "Tariff: " << tariff << " per kWh\n";
    cout << "Total daily energy: " << dailyKwh << " kWh\n";
    cout << "Total daily cost:  " << dailyCost << "\n";
    cout << "Estimated 30-day energy: " << monthlyKwh << " kWh\n";
    cout << "Estimated 30-day cost:  " << monthlyCost << "\n";

    // Ask to save summary
    cout << "\nSave this billing summary to billing_summary.txt? (y/n): ";
    char choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 'y' || choice == 'Y') {
        // Build a nice text block
        ostringstream out;
        out << "================ BILLING SUMMARY ================\n";
        out << "Tariff: " << fixed << setprecision(2) << tariff << " per kWh\n";
        out << "Appliances count: " << appliances.size() << "\n";
        out << "Total daily energy: " << dailyKwh << " kWh\n";
        out << "Total daily cost:  " << dailyCost << "\n";
        out << "Estimated 30-day energy: " << monthlyKwh << " kWh\n";
        out << "Estimated 30-day cost:  " << monthlyCost << "\n";
        out << "=================================================\n";

        appendBillingSummaryToFile(out.str());
        cout << "✅ Saved to billing_summary.txt\n";
    } else {
        cout << "Not saved.\n";
    }
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

// ------------------------------
// Main program
// ------------------------------
int main() {
    vector<Appliance> appliances;

    // Load saved appliance data at startup
    loadAppliances(appliances);

    cout << "\n\nElectrical Load Monitoring & Billing System\n";
    cout << "Loaded appliances: " << appliances.size() << "\n";

    while (true) {
        showMenu();
        int option = readInt("Choose an option (1-6): ");

        switch (option) {
            case 1:
                registerAppliance(appliances);
                break;
            case 2:
                viewAppliances(appliances);
                break;
            case 3:
                searchAppliance(appliances);
                break;
            case 4:
                billingMenu(appliances);
                break;
            case 5:
                if (saveAppliances(appliances)) {
                    cout << "✅ Appliances saved to appliances.txt\n";
                } else {
                    cout << "⚠️ Failed to save appliances.\n";
                }
                break;
            case 6:
                // Save before exit (safe)
                saveAppliances(appliances);
                cout << "Goodbye!\n";
                return 0;

            default:
                // Gracefully handle invalid menu choices
                cout << "Invalid choice. Please choose between 1 and 6.\n";
                break;
        }
    }
}
