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

// Data structures
struct Address {
    string addressID;
    string city;
    string region;
    string wereda;
    string subcity;
};

struct Phone {
    string phoneID;
    string donorID;
    string phoneNumber;
    string phoneType;
};

struct Appointment {
    string appointmentID;
    string donorID;
    string supervisorID;
    string appointmentDate;
    string appointmentTime;
    string status;
};

struct MedicalHistory {
    string historyID;
    string donorID;
    string supervisorID;
    bool HIV;
    bool syphilis;
    bool hepatitisB;
    bool hepatitisC;
    float sugarLevel;
    string outcomeDetails;
    string dateRecorded;
};

struct HealthState {
    string healthStateID;
    string donorID;
    string supervisorID;
    float weight;
    float sugarLevel;
    string bloodPressure;
    float temperature;
    int pulse;
    string dateChecked;
    string timeChecked;
};

// Donor functions
void donor_registration() {
    string donorID = generate_uuid();
    ofstream donorFile("donor_registration.txt", ios::app);
    ofstream phoneFile("donor_phones.txt", ios::app);
    ofstream addressFile("donor_addresses.txt", ios::app);

    cout << "\n--- Donor Registration ---" << endl;

    string firstName, lastName, gender, bloodType;
    string dob;
    Address address;
    vector<Phone> phones;

    cout << "First Name: "; getline(cin, firstName);
    cout << "Last Name: "; getline(cin, lastName);
    cout << "Date of Birth (YYYY-MM-DD): "; getline(cin, dob);
    cout << "Gender (Male/Female/Other): "; getline(cin, gender);
    cout << "Blood Type (A+, O−, B+, AB−, or 'Unknown'): "; getline(cin, bloodType);

    address.addressID = generate_uuid();
    addressFile << "AddressID: " << address.addressID << "\n";
    addressFile << "City: "; cout << "City: "; getline(cin, address.city); addressFile << address.city << "\n";
    addressFile << "Region: "; cout << "Region: "; getline(cin, address.region); addressFile << address.region << "\n";
    addressFile << "Wereda (optional): "; getline(cin, address.wereda); addressFile << address.wereda << "\n";
    addressFile << "Subcity (optional): "; getline(cin, address.subcity); addressFile << address.subcity << "\n\n";

    int phoneCount = 0;
    while (true) {
        Phone p;
        p.phoneID = generate_uuid();
        p.donorID = donorID;

        cout << "Enter phone number: ";
        getline(cin, p.phoneNumber);
        while (!validate_phone(p.phoneNumber)) {
            cout << "Invalid phone number. Try again: ";
            getline(cin, p.phoneNumber);
        }

        cout << "Phone type (Mobile/Home/Work): ";
        getline(cin, p.phoneType);

        phones.push_back(p);
        phoneCount++;

        string more;
        cout << "Add another phone? (y/n): ";
        getline(cin, more);
        if (more != "y" && more != "Y") break;
    }

    if (phoneCount < 1) {
        cout << "At least one phone number is required.\n";
        return;
    }

    // Save donor
    donorFile << "DonorID: " << donorID << endl;
    donorFile << "FirstName: " << firstName << endl;
    donorFile << "LastName: " << lastName << endl;
    donorFile << "DOB: " << dob << endl;
    donorFile << "Gender: " << gender << endl;
    donorFile << "BloodType: " << bloodType << endl;
    donorFile << "AddressID: " << address.addressID << "\n\n";

    // Save phones
    for (const auto& p : phones) {
        phoneFile << "PhoneID: " << p.phoneID << endl;
        phoneFile << "DonorID: " << p.donorID << endl;
        phoneFile << "PhoneNumber: " << p.phoneNumber << endl;
        phoneFile << "PhoneType: " << p.phoneType << "\n\n";
    }

    cout << "✅ Registration complete!\n";
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

// Appointment management
void schedule_appointment() {
    string appointmentID = generate_uuid();
    ofstream appointmentFile("appointments.txt", ios::app);

    string donorID, supervisorID, appointmentDate, appointmentTime, status = "Scheduled";

    cout << "\nEnter DonorID: "; getline(cin, donorID);
    cout << "Enter SupervisorID: "; getline(cin, supervisorID);
    cout << "Enter Appointment Date (YYYY-MM-DD): "; getline(cin, appointmentDate);
    cout << "Enter Appointment Time (HH:MM): "; getline(cin, appointmentTime);

    appointmentFile << "AppointmentID: " << appointmentID << endl;
    appointmentFile << "DonorID: " << donorID << endl;
    appointmentFile << "SupervisorID: " << supervisorID << endl;
    appointmentFile << "AppointmentDate: " << appointmentDate << endl;
    appointmentFile << "AppointmentTime: " << appointmentTime << endl;
    appointmentFile << "Status: " << status << "\n\n";

    cout << "✅ Appointment scheduled!\n";
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
