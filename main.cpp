#include <iostream>
#include <string>
#include <limits>

using namespace std;

// File names 
const string APPLIANCES_FILE = "appliances.txt";
const string BILLING_FILE    = "billing_summary.txt";

// Max appliances (will be used later)
const int MAX_APPLIANCES = 100;

// Clears bad input from cin
void clearBadInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Reads an int safely (keeps asking until correct)
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
            case 1:
                cout << "Register appliance feature is not implemented yet.\n";
                break;

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
