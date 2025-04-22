#include <iostream>
#include <cstring>
#include <cctype>
#include <string>
#include <limits>
#include <cstdio> // Required for sscanf

using namespace std;

// --- Data Structures ---
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
    size_t passwordHash; // Added for password storage
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

struct MedicalHistory {
    int id;
    int donorId;
    int supervisorId;
    char entryDate[11];
    char hivStatus[20];
    char syphilisStatus[20];
    char hepatitisStatus[20];
    float sugarLevel;
    char outcomeMessage[500];
};

struct HealthStatus {
    int id;
    int donorId;
    int supervisorId;
    char donationDate[11];
    float weight;
    char bloodPressure[20];
    float sugarLevel;
    char startTime[6];
    char endTime[6];
};

// --- Global Data ---
const int MAX_RECORDS = 1000;
Address addresses[MAX_RECORDS];
Donor donors[MAX_RECORDS];
Phone phones[MAX_RECORDS];
Appointment appointments[MAX_RECORDS];
Supervisor supervisors[MAX_RECORDS];
MedicalHistory medicalHistories[MAX_RECORDS];
HealthStatus healthStatuses[MAX_RECORDS];

int addressCount = 0, donorCount = 0, phoneCount = 0, appointmentCount = 0, supervisorCount = 0;
int medicalHistoryCount = 0, healthStatusCount = 0;
int nextSupervisorId = 1;

// --- Utility Functions ---

void safeGetline(char* buffer, int size) {
    cin.getline(buffer, size);
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        buffer[size - 1] = '\0';
        cout << "Warning: Input truncated to " << size - 1 << " characters.\n";
    } else if (strlen(buffer) == 0 && cin.eof()) {
        cout << "End of input detected." << endl;
    }
}

bool validate_phone(const char* phone) {
    if (phone == nullptr) return false;
    size_t len = strlen(phone);
    if (len != 10) return false;
    if (strncmp(phone, "09", 2) != 0 && strncmp(phone, "07", 2) != 0) return false;
    for (size_t i = 2; i < len; i++) {
        if (!isdigit(phone[i])) return false;
    }
    return true;
}

bool validate_date(const char* date) {
    if (date == nullptr) return false;
    if (strlen(date) != 10 || date[4] != '-' || date[7] != '-') return false;
    int year, month, day;
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3) return false;

    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) {
            if (date[i] != '-') return false;
        } else {
            if (!isdigit(date[i])) return false;
        }
    }
    return true;
}

bool validate_time(const char* time) {
    if (time == nullptr) return false;
    if (strlen(time) != 5 || time[2] != ':') return false;
    int hours, minutes;

    if (sscanf(time, "%d:%d", &hours, &minutes) != 2) return false;

    for (int i = 0; i < 5; i++) {
        if (i == 2) {
            if (time[i] != ':') return false;
        } else {
            if (!isdigit(time[i])) return false;
        }
    }

    return hours >= 0 && hours < 24 && minutes >= 0 && minutes < 60;
}

bool donor_exists(int id) {
    for (int i = 0; i < donorCount; i++) {
        if (donors[i].id == id) return true;
    }
    return false;
}

size_t custom_hash(const char* str) {
    size_t hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

int getIntegerInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.good()) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

float getFloatInput(const string& prompt) {
    float value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.good()) {
             cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// --- Core Functions ---

void donor_registration() {
    if (donorCount >= MAX_RECORDS || addressCount >= MAX_RECORDS) {
        cout << "Maximum record limit reached. Cannot register more donors.\n";
        return;
    }

    Address addr;
    addr.id = ++addressCount;

    cout << "\n--- Donor Registration ---\n";
    cout << "City: "; safeGetline(addr.city, 50);
    cout << "Region: "; safeGetline(addr.region, 50);
    cout << "Wereda (optional, press Enter to skip): "; safeGetline(addr.wereda, 50);
    cout << "Subcity (optional, press Enter to skip): "; safeGetline(addr.subcity, 50);
    addresses[addressCount - 1] = addr;

    Donor donor;
    donor.id = ++donorCount;
    donor.addressId = addr.id;

    cout << "First Name: "; safeGetline(donor.firstName, 50);
    cout << "Last Name: "; safeGetline(donor.lastName, 50);

    do {
        cout << "Date of Birth (YYYY-MM-DD): ";
        safeGetline(donor.dob, 11);
        if (!validate_date(donor.dob)) {
            cout << "Invalid date format. Please use YYYY-MM-DD.\n";
        }
    } while (!validate_date(donor.dob));

    cout << "Gender (Male/Female/Other): "; safeGetline(donor.gender, 10);

    const char* validBloodTypes[] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-", "I don't know"};
    const int numBloodTypes = sizeof(validBloodTypes) / sizeof(validBloodTypes[0]);
    bool valid = false;
    do {
        cout << "Blood Type (Options: A+, A-, B+, B-, AB+, AB-, O+, O-, I don't know): ";
        safeGetline(donor.bloodType, 20);
        valid = false;
        for (int i = 0; i < numBloodTypes; ++i) {
            if (strcmp(validBloodTypes[i], donor.bloodType) == 0) {
                valid = true;
                break;
            }
        }
        if (!valid) {
             cout << "Invalid blood type entered. Please choose from the list.\n";
        }
    } while (!valid);

    // Password setup
    char password[50];
    char confirmPassword[50];
    do {
        cout << "Set your password: ";
        safeGetline(password, 50);
        cout << "Confirm password: ";
        safeGetline(confirmPassword, 50);
        if (strcmp(password, confirmPassword) != 0) {
            cout << "Passwords do not match. Please try again.\n";
        }
    } while (strcmp(password, confirmPassword) != 0);
    donor.passwordHash = custom_hash(password);

    donors[donorCount - 1] = donor;

    // Add Phone Numbers
    char choice;
    do {
        if (phoneCount >= MAX_RECORDS) {
            cout << "Maximum phone number limit reached.\n";
            break;
        }
        Phone phone;
        phone.id = ++phoneCount;
        phone.donorId = donor.id;
        strcpy(phone.type, "Mobile");

        do {
            cout << "Phone Number (Format: 09xxxxxxxx or 07xxxxxxxx): ";
            safeGetline(phone.number, 15);
            if (!validate_phone(phone.number)) {
                cout << "Invalid phone number format. Please use 09xxxxxxxx or 07xxxxxxxx.\n";
                continue;
            }
            // Check if phone number is already registered
            bool exists = false;
            for (int j = 0; j < phoneCount -1; ++j) { // check existing phones before the current one
                if (strcmp(phones[j].number, phone.number) == 0) {
                    exists = true;
                    break;
                }
            }
            if (exists) {
                cout << "This phone number is already registered. Please use a different number.\n";
                continue;
            }
            break;
        } while (true);

        phones[phoneCount - 1] = phone;

        cout << "Add another phone number? (y/n): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

    } while (tolower(choice) == 'y');

    cout << "\nRegistration Complete! Your Donor ID: " << donor.id << "\n\n";
}

int donorLogin() {
    char phoneNumber[15];
    char password[50];

    cout << "\n--- Donor Login ---\n";
    cout << "Phone Number: ";
    safeGetline(phoneNumber, 15);
    cout << "Password: ";
    safeGetline(password, 50);

    size_t inputHash = custom_hash(password);

    for (int i = 0; i < phoneCount; ++i) {
        if (strcmp(phones[i].number, phoneNumber) == 0) {
            int donorId = phones[i].donorId;
            for (int j = 0; j < donorCount; ++j) {
                if (donors[j].id == donorId) {
                    if (donors[j].passwordHash == inputHash) {
                        cout << "Login successful. Welcome, " << donors[j].firstName << "!\n";
                        return donorId;
                    } else {
                        cout << "Incorrect password.\n";
                        return -1;
                    }
                }
            }
        }
    }
    cout << "Phone number not found.\n";
    return -1;
}

void schedule_appointment(int donorId) {
    if (appointmentCount >= MAX_RECORDS) {
        cout << "Maximum appointment limit reached.\n";
        return;
    }

    Appointment appt;
    appt.id = ++appointmentCount;
    appt.donorId = donorId;
    appt.supervisorId = 0;
    strcpy(appt.status, "Scheduled");

    do {
        cout << "Appointment Date (YYYY-MM-DD): ";
        safeGetline(appt.date, 11);
         if (!validate_date(appt.date)) {
            cout << "Invalid date format. Please use YYYY-MM-DD.\n";
        }
    } while (!validate_date(appt.date));

    do {
        cout << "Appointment Time (HH:MM - 24-hour format): ";
        safeGetline(appt.time, 6);
        if (!validate_time(appt.time)) {
            cout << "Invalid time format. Please use HH:MM (e.g., 14:30).\n";
        }
    } while (!validate_time(appt.time));

    appointments[appointmentCount - 1] = appt;
    cout << "\nAppointment Scheduled Successfully! Appointment ID: " << appt.id << "\n\n";
}

void viewMedicalHistory(int donorId) {
    cout << "\n--- Medical History for Donor ID: " << donorId << " ---\n";
    bool found = false;
    for (int i = 0; i < medicalHistoryCount; ++i) {
        if (medicalHistories[i].donorId == donorId) {
            found = true;
            const MedicalHistory& mh = medicalHistories[i];
            cout << "Record ID: " << mh.id << "\n";
            cout << "Entry Date: " << mh.entryDate << "\n";
            cout << "Recorded by Supervisor ID: " << mh.supervisorId << "\n";
            cout << "HIV Status: " << mh.hivStatus << "\n";
            cout << "Syphilis Status: " << mh.syphilisStatus << "\n";
            cout << "Hepatitis Status: " << mh.hepatitisStatus << "\n";
            cout << "Sugar Level: " << mh.sugarLevel << "\n";
            cout << "OutcomeMessage: " << mh.outcomeMessage << "\n";
            cout << "-----------------------------------------\n";
        }
    }
    if (!found) {
        cout << "No medical history records found for this Donor ID.\n";
    }
    cout << endl;
}

void viewHealthStatus(int donorId) {
    cout << "\n--- Health Status Records for Donor ID: " << donorId << " ---\n";
    bool found = false;
    for (int i = 0; i < healthStatusCount; ++i) {
        if (healthStatuses[i].donorId == donorId) {
            found = true;
            const HealthStatus& hs = healthStatuses[i];
            cout << "Record ID: " << hs.id << "\n";
            cout << "Donation Date: " << hs.donationDate << "\n";
            cout << "Recorded by Supervisor ID: " << hs.supervisorId << "\n";
            cout << "Weight: " << hs.weight << " kg\n";
            cout << "Blood Pressure: " << hs.bloodPressure << "\n";
            cout << "Sugar Level (Pre-donation): " << hs.sugarLevel << "\n";
            cout << "Donation Start Time: " << hs.startTime << "\n";
            cout << "Donation End Time: " << hs.endTime << "\n";
            cout << "-----------------------------------------\n";
        }
    }
    if (!found) {
        cout << "No health status records found for this Donor ID.\n";
    }
    cout << endl;
}

void donorMenu(int donorId) {
    int option;
    do {
        cout << "\n--- Donor Menu ---\n";
        cout << "Logged in as Donor ID: " << donorId << "\n";
        cout << "1. Schedule Donation Appointment\n";
        cout << "2. View My Medical History\n";
        cout << "3. View My Health Status Records\n";
        cout << "4. Back to Main Menu\n";
        cout << "5. Exit Application\n";

        option = getIntegerInput("Select option: ");

        switch (option) {
            case 1:
                schedule_appointment(donorId);
                break;
            case 2:
                viewMedicalHistory(donorId);
                break;
            case 3:
                viewHealthStatus(donorId);
                break;
            case 4:
                cout << "Returning to Main Menu...\n";
                return;
            case 5:
                cout << "Exiting Blood Donation System. Goodbye!\n";
                exit(0);
            default:
                cout << "Invalid option! Please try again.\n";
        }
    } while (option != 4);
}

void donorPortal() {
    int option;
    do {
        cout << "\n--- Donor Portal ---\n";
        cout << "1. Login\n";
        cout << "2. Sign Up\n";
        cout << "3. Back to Main Menu\n";

        option = getIntegerInput("Select option: ");

        switch (option) {
            case 1: {
                int donorId = donorLogin();
                if (donorId != -1) {
                    donorMenu(donorId);
                }
                break; // Break out of the switch case after login attempt
            }
            case 2:
                donor_registration();
                break;
            case 3:
                cout << "Returning to Main Menu...\n";
                return; // Exit the donorPortal function
            default:
                cout << "Invalid option! Please try again.\n";
        }
    } while (option != 3); // Loop until the user chooses to go back to the main menu
}

// Placeholder for supervisor functions (add implementations later)
void supervisorLogin() {
    cout << "\n--- Supervisor Login ---\n";
    cout << "Supervisor login is not yet implemented.\n";
}

void supervisorMenu(int supervisorId) {
    cout << "\n--- Supervisor Menu ---\n";
    cout << "Supervisor menu is not yet implemented.\n";
}

void supervisorPortal() {
    int option;
    do {
        cout << "\n--- Supervisor Portal ---\n";
        cout << "1. Login\n";
        cout << "2. Back to Main Menu\n"; // Option to add a new supervisor? Or handled by admin?

        option = getIntegerInput("Select option: ");

        switch (option) {
            case 1: {
                // Implement supervisor login logic here
                // For now, just a placeholder
                supervisorLogin();
                // After successful login, call supervisorMenu()
                // int supervisorId = supervisorLogin();
                // if (supervisorId != -1) {
                //     supervisorMenu(supervisorId);
                // }
                break;
            }
            case 2:
                cout << "Returning to Main Menu...\n";
                return;
            default:
                cout << "Invalid option! Please try again.\n";
        }
    } while (option != 2);
}


// --- Main Menu ---
int main() {
    int choice;

    // Add a default supervisor for testing (optional)
    if (supervisorCount == 0) {
        supervisors[supervisorCount].id = nextSupervisorId++;
        strcpy(supervisors[supervisorCount].username, "admin");
        supervisors[supervisorCount].passwordHash = custom_hash("admin123");
        supervisorCount++;
        cout << "Default supervisor 'admin' created with password 'admin123'.\n";
    }


    do {
        cout << "\n--- Blood Donation System Main Menu ---\n";
        cout << "1. Donor Portal\n";
        cout << "2. Supervisor Portal\n";
        cout << "3. Exit\n";

        choice = getIntegerInput("Enter your choice: ");

        switch (choice) {
            case 1:
                donorPortal();
                break;
            case 2:
                supervisorPortal();
                break;
            case 3:
                cout << "Exiting Blood Donation System. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);

    return 0;
}