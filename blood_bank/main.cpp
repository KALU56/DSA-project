#include <iostream>
#include <string>
#include <cctype>  // for isdigit
#include <cstdlib> // for system()
using namespace std;

// Global variables for demo purposes
string registeredUsername = "";
string registeredPassword = "";

// Utility to validate phone number
bool isValidPhone(const string& phone) {
    if (phone.length() != 10) return false;
    if (phone.substr(0, 2) != "09" && phone.substr(0, 2) != "07") return false;
    for (int i = 0; i < phone.length(); i++) {
        if (!isdigit(phone[i])) return false;
    }
    return true;
}

// Donor registration function
void registerDonor() {
    string firstname, lastname, city, region, kebele, woreda;
    string gender, phone, username, password, confirmPassword;
    string bloodType, email;
    int age;

    cin.ignore(); // Flush input buffer
    cout << "\n=== Donor Registration ===\n";
    cout << "First Name: "; getline(cin, firstname);
    cout << "Last Name: "; getline(cin, lastname);

    // Age validation
    do {
        cout << "Age (18 - 54): ";
        cin >> age;
        if (age < 18 || age > 54)
            cout << "Invalid age. Must be between 18 and 54.\n";
    } while (age < 18 || age > 54);
    cin.ignore();

    cout << "City: "; getline(cin, city);
    cout << "Region: "; getline(cin, region);
    cout << "Kebele: "; getline(cin, kebele);
    cout << "Woreda: "; getline(cin, woreda);

    // Gender validation
    do {
        cout << "Gender (male/female): ";
        getline(cin, gender);
        if (gender != "male" && gender != "female")
            cout << "Invalid gender. Please enter 'male' or 'female'.\n";
    } while (gender != "male" && gender != "female");

    // Phone validation
    do {
        cout << "Phone (10 digits, starts with 09 or 07): ";
        getline(cin, phone);
        if (!isValidPhone(phone))
            cout << "Invalid phone number format.\n";
    } while (!isValidPhone(phone));

    cout << "Username: "; getline(cin, username);

    // Password confirmation
    do {
        cout << "Password: "; getline(cin, password);
        cout << "Confirm Password: "; getline(cin, confirmPassword);
        if (password != confirmPassword)
            cout << "Passwords do not match. Try again.\n";
    } while (password != confirmPassword);

    cout << "Blood Type (optional – A+/A-/B+/B-/AB+/AB-/O+/O-/I don't know): ";
    getline(cin, bloodType);

    cout << "Email (optional type 'I don't have' if none): ";
    getline(cin, email);

    // Save credentials (demo)
    registeredUsername = username;
    registeredPassword = password;

    cout << "\n Donor registered successfully!\n";
}
// Placeholder function for post-login features
void makeAppointment() {
    cout << "\n📅 Appointment scheduled successfully (demo).\n";
}

void viewMedicalHealth() {
    cout << "\n🩺 Medical Health: No issues reported (demo).\n";
}

void viewHealthStatus() {
    cout << "\n❤️ Health Status: You are eligible for donation (demo).\n";
}
void makeAppointment() {
    string date, time, message;

    cin.ignore(); // flush input buffer
    cout << "\n=== Make Appointment ===\n";
    cout << "Enter Date (e.g. 2025-05-20): ";
    getline(cin, date);

    cout << "Enter Time (e.g. 10:30 AM): ";
    getline(cin, time);

    cout << "Enter Message (optional): ";
    getline(cin, message);

    cout << "\n📅 Appointment successfully created!\n";
    cout << "Date: " << date << "\n";
    cout << "Time: " << time << "\n";
    cout << "Message: " << message << "\n";
}


void donorLoggedInMenu(const string& username) {
    int choice;
    while (true) {
        cout << "\n=== Donor Dashboard ===\n";
        cout << "Welcome, " << username << "!\n";
        cout << "1. Make Appointment\n";
        cout << "2. View Medical Health\n";
        cout << "3. View Health Status\n";
        cout << "4. Logout (Back to Donor Menu)\n";
        cout << "5. Exit\n";
        cout << "Choose: ";
        cin >> choice;

        switch (choice) {
            case 1:
                makeAppointment();
                break;
            case 2:
                viewMedicalHealth();
                break;
            case 3:
                viewHealthStatus();
                break;
            case 4:
                return; // Go back to donor menu
            case 5:
                cout << "\nThank you for using the system. Goodbye!\n";
                exit(0); // Exit the entire program
            default:
                cout << "Invalid choice. Try again.\n";
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();

#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
}

// Donor login function (updated)
void loginDonor() {
    string uname, pass;
    cin.ignore();
    cout << "\n=== Donor Login ===\n";
    cout << "Username: "; getline(cin, uname);
    cout << "Password: "; getline(cin, pass);

    if (uname == registeredUsername && pass == registeredPassword) {
        cout << "\n✅ Login successful! Welcome, " << uname << "!\n";
        donorLoggedInMenu(uname);
    } else {
        cout << "\n❌ Invalid username or password.\n";
    }
}

// Donor menu
void donorMenu() {
    int choice;
    while (true) {
        cout << "\n--- Donor Menu ---\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Back to Main Menu\n";
        cout << "Choose: ";
        cin >> choice;

        switch (choice) {
            case 1:
                registerDonor();
                break;
            case 2:
                loginDonor();
                break;
            case 3:
                return;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
}

// Placeholder for supervisor menu
void supervisorMenu() {
    cout << "\n[Supervisor Section]\n(Coming soon...)\n";
}

// Main function
int main() {
    int choice;

    while (true) {
        cout << "\n==========================\n";
        cout << "     Welcome to System    \n";
        cout << "==========================\n";
        cout << "1. Donor\n";
        cout << "2. Supervisor\n";
        cout << "3. Exit\n";
        cout << "Please select an option (1-3): ";
        cin >> choice;

        switch (choice) {
            case 1:
                donorMenu();
                break;
            case 2:
                supervisorMenu();
                break;
            case 3:
                cout << "\nThank you for using the system. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Try again.\n";
        }

        cout << "\nPress Enter to return to the main menu...";
        cin.ignore();
        cin.get();

#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
}
