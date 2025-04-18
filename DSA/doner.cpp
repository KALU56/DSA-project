#include <iostream>
#include <cstring>
#include <cctype>

using namespace std;

// Data Structures
struct Address {
    int id;
    char city[50];
    char region[50];
    char wereda[50];
    char subcity[50];
};

struct Donor {
    int id;
    char firstName[50];
    char lastName[50];
    char dob[11];
    char gender[10];
    char bloodType[20];
    int addressId;
};

struct Phone {
    int id;
    int donorId;
    char number[15];
    char type[10];
};

struct Appointment {
    int id;
    int donorId;
    int supervisorId;
    char date[11];
    char time[6];
    char status[20];
};

struct Supervisor {
    int id;
    char username[50];
    size_t passwordHash;
};

// Global Data
Address addresses[1000];
Donor donors[1000];
Phone phones[1000];
Appointment appointments[1000];
Supervisor supervisors[1000];

int addressCount = 0, donorCount = 0, phoneCount = 0, appointmentCount = 0, supervisorCount = 0;

// Utility Functions
bool validate_phone(const char* phone) {
    if (strlen(phone) != 10) return false;
    if (strncmp(phone, "09", 2) != 0 && strncmp(phone, "07", 2) != 0) return false;
    for (int i = 2; i < 10; i++) if (!isdigit(phone[i])) return false;
    return true;
}

bool validate_date(const char* date) {
    if (strlen(date) != 10 || date[4] != '-' || date[7] != '-') return false;
    for (int i = 0; i < 10; i++) if ((i != 4 && i != 7) && !isdigit(date[i])) return false;
    return true;
}

bool validate_time(const char* time) {
    if (strlen(time) != 5 || time[2] != ':') return false;
    for (int i = 0; i < 5; i++) if (i != 2 && !isdigit(time[i])) return false;
    int hours = atoi(string(time).substr(0, 2).c_str());
    int minutes = atoi(string(time).substr(3, 2).c_str());
    return hours >= 0 && hours < 24 && minutes >= 0 && minutes < 60;
}

bool donor_exists(int id) {
    for (int i = 0; i < donorCount; i++) if (donors[i].id == id) return true;
    return false;
}

size_t custom_hash(const char* str) {
    size_t hash = 5381;
    int c;
    while ((c = *str++)) hash = ((hash << 5) + hash) + c;
    return hash;
}

// Core Functions
void donor_registration() {
    Address addr;
    addr.id = ++addressCount;

    cout << "\n--- Donor Registration ---\n";
    cout << "City: "; cin.getline(addr.city, 50);
    cout << "Region: "; cin.getline(addr.region, 50);
    cout << "Wereda (optional): "; cin.getline(addr.wereda, 50);
    cout << "Subcity (optional): "; cin.getline(addr.subcity, 50);
    addresses[addressCount - 1] = addr;

    Donor donor;
    donor.id = ++donorCount;
    donor.addressId = addr.id;

    cout << "First Name: "; cin.getline(donor.firstName, 50);
    cout << "Last Name: "; cin.getline(donor.lastName, 50);

    do {
        cout << "Date of Birth (YYYY-MM-DD): ";
        cin.getline(donor.dob, 11);
    } while (!validate_date(donor.dob));

    cout << "Gender (Male/Female/Other): "; cin.getline(donor.gender, 10);

    const char* validBloodTypes[] = {"A+", "O-", "B+", "AB-", "I don't know"};
    bool valid = false;
    do {
        cout << "Blood Type (A+/O-/B+/AB-/I don't know): ";
        cin.getline(donor.bloodType, 20);
        for (const auto& type : validBloodTypes) {
            if (strcmp(type, donor.bloodType) == 0) {
                valid = true;
                break;
            }
        }
    } while (!valid);

    donors[donorCount - 1] = donor;

    char choice;
    do {
        Phone phone;
        phone.id = ++phoneCount;
        phone.donorId = donor.id;
        strcpy(phone.type, "Mobile");

        do {
            cout << "Phone Number (09/07xxxxxxxx): ";
            cin.getline(phone.number, 15);
        } while (!validate_phone(phone.number));

        phones[phoneCount - 1] = phone;

        cout << "Add another phone number? (y/n): ";
        cin >> choice; cin.ignore();
    } while (tolower(choice) == 'y');

    cout << "\nRegistration Complete! Your DonorID: " << donor.id << "\n\n";
}

void schedule_appointment() {
    int donor_id;
    cout << "Enter DonorID: "; cin >> donor_id; cin.ignore();
    if (!donor_exists(donor_id)) {
        cout << "Invalid DonorID!\n";
        return;
    }

    Appointment appt;
    appt.id = ++appointmentCount;
    appt.donorId = donor_id;
    appt.supervisorId = 0;
    strcpy(appt.status, "Scheduled");

    do {
        cout << "Date (YYYY-MM-DD): ";
        cin.getline(appt.date, 11);
    } while (!validate_date(appt.date));

    do {
        cout << "Time (HH:MM): ";
        cin.getline(appt.time, 6);
    } while (!validate_time(appt.time));

    appointments[appointmentCount - 1] = appt;
    cout << "\nAppointment Scheduled Successfully!\n\n";
}

int main() {
    int option;
    do {
        cout << "1. Register Donor\n";
        cout << "2. Schedule Appointment\n";
        cout << "0. Exit\n";
        cout << "Select option: ";
        cin >> option;
        cin.ignore();

        switch (option) {
            case 1: donor_registration(); break;
            case 2: schedule_appointment(); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid option!\n";
        }
    } while (option != 0);
    return 0;
}