#include <iostream>
#include <cstdlib> 
#include "donor/donor.h"
#include "supervisor/supervisor.h"
using namespace std;

void exitSystem() {
    cout << "\nThank you for using the system. Goodbye!\n";
}

int main() {
    int choice;

    while (true) {
        cout << "==========================" << endl;
        cout << "     Welcome to System    " << endl;
        cout << "==========================" << endl;
        cout << "1. Donor" << endl;
        cout << "2. Supervisor" << endl;
        cout << "3. Exit" << endl;
        cout << "Please select an option (1-3): ";
        cin >> choice;

        switch (choice) {
            case 1:
                donor();         // Call donor function
                break;
            case 2:
                supervisor();    // Call supervisor function
                break;
            case 3:
                exitSystem();    // Exit
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }

        cout << "\nPress Enter to go back to the main menu..." << endl;
        cin.ignore();
        cin.get();
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    return 0;
}
