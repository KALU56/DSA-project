#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <ctime>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

// Utility functions
string generate_uuid() {
    static int counter = 0;
    return to_string(time(nullptr)) + "-" + to_string(counter++);
}

bool validate_phone(const string& phone) {
    regex pattern("(09|07)\\d{8}");
    return regex_match(phone, pattern);
}

bool validate_email(const string& email) {
    regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return regex_match(email, pattern);
}

// Donor functions
void donor_registration() {
    ofstream file("donor_registration.txt", ios::app);
    
    cout << "\nRegistration Form:" << endl;
    string donor_id = generate_uuid();
    cout << "Enter your first name: ";
    string first_name; getline(cin, first_name);
    
    // Add other input fields and validation...
    
    file << "DonorID: " << donor_id << endl;
    file << "FirstName: " << first_name << endl;
    // Write other fields...
    
    cout << "Thank you for registering!" << endl;
}

void donor_menu() {
    while(true) {
        cout << "\nDonor Menu:\n1. Register\n2. Schedule Appointment\n3. Check Status\n4. Exit\nChoice: ";
        int choice; cin >> choice; cin.ignore();
        
        switch(choice) {
            case 1: donor_registration(); break;
            // Add other cases...
            case 4: return;
            default: cout << "Invalid choice!" << endl;
        }
    }
}

// Supervisor functions
void view_donors() {
    ifstream file("donor_registration.txt");
    string line;
    cout << "\nRegistered Donors:" << endl;
    while(getline(file, line)) {
        if(line.find("DonorID:") != string::npos) {
            cout << line << endl;
        }
    }
}

void supervisor_menu() {
    while(true) {
        cout << "\nSupervisor Menu:\n1. View Donors\n2. Approve Appointments\n3. Exit\nChoice: ";
        int choice; cin >> choice; cin.ignore();
        
        switch(choice) {
            case 1: view_donors(); break;
            // Add other cases...
            case 3: return;
            default: cout << "Invalid choice!" << endl;
        }
    }
}

// Main menu
int main() {
    while(true) {
        cout << "\nMain Menu:\n1. Donor\n2. Supervisor\n3. Exit\nChoice: ";
        int choice; cin >> choice; cin.ignore();
        
        switch(choice) {
            case 1: donor_menu(); break;
            case 2: supervisor_menu(); break;
            case 3: return 0;
            default: cout << "Invalid choice!" << endl;
        }
    }
}