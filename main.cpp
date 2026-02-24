#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <fstream>
#include <sstream>

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

string toLowerSimple(string s) {
    for (int i = 0; i < (int)s.size(); i++) {
        char c = s[i];
        if (c >= 'A' && c <= 'Z') s[i] = char(c - 'A' + 'a');
    }
    return s;
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
    a.name  = readNonEmptyLine("Appliance name: ");
    a.watts = readPositiveDouble("Power rating (watts, > 0): ");
    a.hours = readHours("Daily usage hours (0 - 24): ");

    appliances[count] = a;
    count++;

    cout << "Appliance registered.\n";
}

void viewAppliances(const Appliance appliances[], int count) {
    printHeader("All Registered Appliances");

    if (count == 0) {
        cout << "No appliances registered yet.\n";
        return;
    }

    cout << left
         << setw(4)  << "#"
         << setw(25) << "Name"
         << setw(12) << "Watts"
         << setw(12) << "Hours/day"
         << setw(12) << "kWh/day"
         << "\n";

    cout << "----------------------------------------------------\n";
    cout << fixed << setprecision(2);

    for (int i = 0; i < count; i++) {
        cout << left
             << setw(4)  << (i + 1)
             << setw(25) << appliances[i].name
             << setw(12) << appliances[i].watts
             << setw(12) << appliances[i].hours
             << setw(12) << dailyKwh(appliances[i])
             << "\n";
    }
}

void searchAppliance(const Appliance appliances[], int count) {
    printHeader("Search Appliance By Name");

    if (count == 0) {
        cout << "No appliances registered yet.\n";
        return;
    }

    string query = readNonEmptyLine("Enter name to search: ");
    query = toLowerSimple(trim(query));

    bool found = false;
    cout << fixed << setprecision(2);

    for (int i = 0; i < count; i++) {
        string nameLower = toLowerSimple(appliances[i].name);
        if (nameLower.find(query) != string::npos) {
            if (!found) cout << "Matches:\n";
            cout << "- " << appliances[i].name
                 << " | " << appliances[i].watts << " W"
                 << " | " << appliances[i].hours << " hrs/day"
                 << " | " << dailyKwh(appliances[i]) << " kWh/day\n";
            found = true;
        }
    }

    if (!found) cout << "No appliance found.\n";
}

bool saveAppliances(const Appliance appliances[], int count) {
    ofstream fout(APPLIANCES_FILE.c_str());
    if (!fout.is_open()) return false;

    for (int i = 0; i < count; i++) {
        fout << appliances[i].name << "|"
             << appliances[i].watts << "|"
             << appliances[i].hours << "\n";
    }

    fout.close();
    return true;
}

bool loadAppliances(Appliance appliances[], int& count) {
    count = 0;

    ifstream fin(APPLIANCES_FILE.c_str());
    if (!fin.is_open()) {
        return false;
    }

    string line;
    while (getline(fin, line)) {
        line = trim(line);
        if (line == "") continue;

        int p1 = (int)line.find('|');
        if (p1 == -1) continue;
        int p2 = (int)line.find('|', p1 + 1);
        if (p2 == -1) continue;

        string name = trim(line.substr(0, p1));
        string wattsStr = trim(line.substr(p1 + 1, p2 - (p1 + 1)));
        string hoursStr = trim(line.substr(p2 + 1));

        double w = 0.0, h = 0.0;
        {
            stringstream sw(wattsStr);
            if (!(sw >> w)) continue;
        }
        {
            stringstream sh(hoursStr);
            if (!(sh >> h)) continue;
        }

        if (name != "" && w > 0 && h >= 0 && h <= 24) {
            if (count < MAX_APPLIANCES) {
                appliances[count].name = name;
                appliances[count].watts = w;
                appliances[count].hours = h;
                count++;
            }
        }
    }

    fin.close();
    return true;
}

int main() {
    Appliance appliances[MAX_APPLIANCES];
    int count = 0;

    bool loaded = loadAppliances(appliances, count);

    printHeader("Electrical Load Monitoring & Billing System");
    if (loaded) {
        cout << "Loaded appliances: " << count << "\n";
    } else {
        cout << "No previous appliances file found. Starting fresh.\n";
    }

    while (true) {
        showMenu();
        int option = readInt("Choose an option (1-6): ");

        switch (option) {
            case 1:
                registerAppliance(appliances, count);
                break;

            case 2:
                viewAppliances(appliances, count);
                break;

            case 3:
                searchAppliance(appliances, count);
                break;

            case 4:
                printHeader("Billing");
                cout << "This feature will be implemented in the next part.\n";
                break;

            case 5:
                printHeader("Save Appliances");
                if (saveAppliances(appliances, count)) {
                    cout << "Appliances saved to " << APPLIANCES_FILE << "\n";
                } else {
                    cout << "Failed to save appliances.\n";
                }
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