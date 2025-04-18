#include <iostream>
#include <cstring>
#include <cctype>
#include <string>
#include <limits> // Required for numeric_limits

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
    char dob[11]; // YYYY-MM-DD
    char gender[10];
    char bloodType[20];
    int addressId;
};

struct Phone {
    int id;
    int donorId;
    char number[15];
    char type[10]; // e.g., Mobile, Home
};

struct Appointment {
    int id;
    int donorId;
    int supervisorId; // ID of the supervisor who approved/managed
    char date[11]; // YYYY-MM-DD
    char time[6]; // HH:MM
    char status[20]; // e.g., Scheduled, Approved, Completed, Cancelled
};

struct Supervisor {
    int id;
    char username[50];
    size_t passwordHash;
};

struct MedicalHistory {
    int id;
    int donorId;
    int supervisorId; // ID of the supervisor who recorded the history
    char entryDate[11]; // YYYY-MM-DD
    char hivStatus[20]; // Positive/Negative
    char syphilisStatus[20]; // Positive/Negative
    char hepatitisStatus[20]; // Positive/Negative
    float sugarLevel;
    char outcomeMessage[500]; // e.g., "Eligible for donation", "Deferred due to..."
};

struct HealthStatus {
    int id;
    int donorId;
    int supervisorId; // ID of the supervisor who recorded the status
    char donationDate[11]; // YYYY-MM-DD
    float weight;
    char bloodPressure[20]; // e.g., 120/80
    float sugarLevel; // Pre-donation check
    char startTime[6]; // HH:MM (Donation start)
    char endTime[6]; // HH:MM (Donation end)
};

// --- Global Data ---
const int MAX_RECORDS = 1000; // Define a constant for array sizes
Address addresses[MAX_RECORDS];
Donor donors[MAX_RECORDS];
Phone phones[MAX_RECORDS];
Appointment appointments[MAX_RECORDS];
Supervisor supervisors[MAX_RECORDS];
MedicalHistory medicalHistories[MAX_RECORDS];
HealthStatus healthStatuses[MAX_RECORDS];

int addressCount = 0, donorCount = 0, phoneCount = 0, appointmentCount = 0, supervisorCount = 0;
int medicalHistoryCount = 0, healthStatusCount = 0;
int nextSupervisorId = 1; // Start supervisor IDs from 1

// --- Utility Functions ---

// Function to safely read a line, handling potential errors and buffer clearing
void safeGetline(char* buffer, int size) {
    cin.getline(buffer, size);
    if (cin.fail()) { // Handle cases where input exceeds buffer size
        cin.clear(); // Clear error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the rest of the line
        buffer[size - 1] = '\0'; // Ensure null termination
        cout << "Warning: Input truncated to " << size - 1 << " characters.\n";
    } else if (strlen(buffer) == 0 && cin.eof()) {
         // Handle potential EOF scenario if needed, though less common in interactive apps
         cout << "End of input detected." << endl;
    }
}

// Validate Ethiopian phone number format (09xxxxxxxx or 07xxxxxxxx)
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

// Validate date format YYYY-MM-DD
bool validate_date(const char* date) {
    if (date == nullptr) return false;
    if (strlen(date) != 10 || date[4] != '-' || date[7] != '-') return false;
    int year = 0, month = 0, day = 0;
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3) return false; // Check if conversion works

    // Basic validation (doesn't check days in month perfectly, but is a good start)
    if (year < 1900 || year > 2100) return false; // Reasonable year range
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    // Check digits
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) {
            if (date[i] != '-') return false;
        } else {
            if (!isdigit(date[i])) return false;
        }
    }
    return true;
}

// Validate time format HH:MM (24-hour)
bool validate_time(const char* time) {
    if (time == nullptr) return false;
    if (strlen(time) != 5 || time[2] != ':') return false;
    int hours = -1, minutes = -1;

    // Attempt to parse hours and minutes
    if (sscanf(time, "%d:%d", &hours, &minutes) != 2) return false;

    // Validate digits and range
    for (int i = 0; i < 5; i++) {
        if (i == 2) {
            if (time[i] != ':') return false;
        } else {
            if (!isdigit(time[i])) return false;
        }
    }

    // Check if parsed values are within valid range
    return hours >= 0 && hours < 24 && minutes >= 0 && minutes < 60;
}


// Check if a donor with the given ID exists
bool donor_exists(int id) {
    for (int i = 0; i < donorCount; i++) {
        if (donors[i].id == id) return true;
    }
    return false;
}

// Simple string hashing function (djb2)
size_t custom_hash(const char* str) {
    size_t hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

// Function to safely read an integer, with basic validation
int getIntegerInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.good()) { // Check if input is a valid integer
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the buffer
            return value;
        } else {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        }
    }
}

// Function to safely read a float, with basic validation
float getFloatInput(const string& prompt) {
    float value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.good()) { // Check if input is a valid float
             cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the buffer
            return value;
        } else {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
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
    addr.id = ++addressCount; // Pre-increment for unique ID starting from 1

    cout << "\n--- Donor Registration ---\n";
    cout << "City: "; safeGetline(addr.city, 50);
    cout << "Region: "; safeGetline(addr.region, 50);
    cout << "Wereda (optional, press Enter to skip): "; safeGetline(addr.wereda, 50);
    cout << "Subcity (optional, press Enter to skip): "; safeGetline(addr.subcity, 50);
    addresses[addressCount - 1] = addr;

    Donor donor;
    donor.id = ++donorCount; // Pre-increment for unique ID starting from 1
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

    // Add validation for Gender input if desired
    cout << "Gender (Male/Female/Other): "; safeGetline(donor.gender, 10);

    // Input validation for Blood Type
    const char* validBloodTypes[] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-", "I don't know"};
    const int numBloodTypes = sizeof(validBloodTypes) / sizeof(validBloodTypes[0]);
    bool valid = false;
    do {
        cout << "Blood Type (Options: A+, A-, B+, B-, AB+, AB-, O+, O-, I don't know): ";
        safeGetline(donor.bloodType, 20);
        valid = false; // Reset flag for each attempt
        for (int i = 0; i < numBloodTypes; ++i) {
             // Use strcasecmp for case-insensitive comparison if needed (requires <strings.h>)
             // if (strcasecmp(validBloodTypes[i], donor.bloodType) == 0) {
            if (strcmp(validBloodTypes[i], donor.bloodType) == 0) {
                valid = true;
                break;
            }
        }
        if (!valid) {
             cout << "Invalid blood type entered. Please choose from the list.\n";
        }
    } while (!valid);

    donors[donorCount - 1] = donor;

    // Add Phone Numbers
    char choice;
    do {
        if (phoneCount >= MAX_RECORDS) {
            cout << "Maximum phone number limit reached.\n";
            break;
        }
        Phone phone;
        phone.id = ++phoneCount; // Pre-increment
        phone.donorId = donor.id;
        // Set default type or ask user
        strcpy(phone.type, "Mobile"); // Defaulting to Mobile

        do {
            cout << "Phone Number (Format: 09xxxxxxxx or 07xxxxxxxx): ";
            safeGetline(phone.number, 15);
             if (!validate_phone(phone.number)) {
                cout << "Invalid phone number format. Please use 09xxxxxxxx or 07xxxxxxxx.\n";
            }
        } while (!validate_phone(phone.number));

        phones[phoneCount - 1] = phone;

        cout << "Add another phone number? (y/n): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer after reading char

    } while (tolower(choice) == 'y');

    cout << "\nRegistration Complete! Your Donor ID: " << donor.id << "\n\n";
}

void schedule_appointment() {
     if (appointmentCount >= MAX_RECORDS) {
        cout << "Maximum appointment limit reached.\n";
        return;
    }

    int donor_id = getIntegerInput("Enter Donor ID to schedule appointment: ");

    if (!donor_exists(donor_id)) {
        cout << "Donor ID " << donor_id << " not found.\n";
        return;
    }

    Appointment appt;
    appt.id = ++appointmentCount; // Pre-increment
    appt.donorId = donor_id;
    appt.supervisorId = 0; // Supervisor ID not assigned yet
    strcpy(appt.status, "Scheduled"); // Initial status

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

void viewMedicalHistory() {
    int donorId = getIntegerInput("Enter your Donor ID to view Medical History: ");

    if (!donor_exists(donorId)) {
        cout << "Donor ID " << donorId << " not found.\n";
        return;
    }

    cout << "\n--- Medical History for Donor ID: " << donorId << " ---\n";
    bool found = false;
    for (int i = 0; i < medicalHistoryCount; ++i) {
        if (medicalHistories[i].donorId == donorId) {
            found = true;
            const MedicalHistory& mh = medicalHistories[i]; // Use const reference
            cout << "Record ID: " << mh.id << "\n";
            cout << "Entry Date: " << mh.entryDate << "\n";
            cout << "Recorded by Supervisor ID: " << mh.supervisorId << "\n";
            cout << "HIV Status: " << mh.hivStatus << "\n";
            cout << "Syphilis Status: " << mh.syphilisStatus << "\n";
            cout << "Hepatitis Status: " << mh.hepatitisStatus << "\n";
            cout << "Sugar Level: " << mh.sugarLevel << "\n";
            cout << "Outcome Message: " << mh.outcomeMessage << "\n";
            cout << "-----------------------------------------\n";
        }
    }
    if (!found) {
        cout << "No medical history records found for this Donor ID.\n";
    }
     cout << endl; // Add a newline for better spacing
}

void viewHealthStatus() {
    int donorId = getIntegerInput("Enter your Donor ID to view Health Status: ");

    if (!donor_exists(donorId)) {
        cout << "Donor ID " << donorId << " not found.\n";
        return;
    }

    cout << "\n--- Health Status Records for Donor ID: " << donorId << " ---\n";
    bool found = false;
    for (int i = 0; i < healthStatusCount; ++i) {
        if (healthStatuses[i].donorId == donorId) {
            found = true;
            const HealthStatus& hs = healthStatuses[i]; // Use const reference
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
    cout << endl; // Add a newline for better spacing
}

int supervisorLogin() {
    char username[50];
    char password[50];
    cout << "\n--- Supervisor Login ---\n";
    cout << "Username: ";
    safeGetline(username, 50);
    cout << "Password: ";
    // Consider implementing a more secure password input method (e.g., hiding input)
    // This is basic for demonstration.
    safeGetline(password, 50);

    size_t inputHash = custom_hash(password);

    for (int i = 0; i < supervisorCount; ++i) {
        if (strcmp(supervisors[i].username, username) == 0 && supervisors[i].passwordHash == inputHash) {
            cout << "Login successful. Welcome, " << username << "!\n";
            return supervisors[i].id; // Return supervisor ID on successful login
        }
    }
    cout << "Invalid username or password.\n";
    return -1; // Return -1 indicating failed login
}

void viewDonors() {
    cout << "\n--- List of Registered Donors ---\n";
    if (donorCount == 0) {
        cout << "No donors registered yet.\n";
        return;
    }

    for (int i = 0; i < donorCount; ++i) {
        const Donor& d = donors[i]; // Use const reference
        cout << "Donor ID: " << d.id << "\n";
        cout << "Name: " << d.firstName << " " << d.lastName << "\n";
        cout << "DOB: " << d.dob << "\n";
        cout << "Gender: " << d.gender << "\n";
        cout << "Blood Type: " << d.bloodType << "\n";

        // Find and display address
        Address addr;
        bool addrFound = false;
        for (int j = 0; j < addressCount; ++j) {
            if (addresses[j].id == d.addressId) {
                addr = addresses[j];
                addrFound = true;
                break;
            }
        }
        if (addrFound) {
             cout << "Address (ID " << addr.id << "): " << addr.city
                  << (strlen(addr.region) > 0 ? ", " : "") << addr.region
                  << (strlen(addr.wereda) > 0 ? ", Wereda: " : "") << addr.wereda
                  << (strlen(addr.subcity) > 0 ? ", Subcity: " : "") << addr.subcity << "\n";
        } else {
            cout << "Address: Not Found (ID: " << d.addressId << ")\n";
        }


        // Find and display phone numbers
        cout << "Phone Numbers: ";
        bool hasPhone = false;
        for (int j = 0; j < phoneCount; ++j) {
            if (phones[j].donorId == d.id) {
                cout << phones[j].number << " (" << phones[j].type << ") ";
                hasPhone = true;
            }
        }
        if (!hasPhone) cout << "None";
        cout << "\n-------------------------------------\n";
    }
    cout << endl;
}

void sendMedicalHistory(int supervisorId) {
     if (medicalHistoryCount >= MAX_RECORDS) {
        cout << "Maximum medical history record limit reached.\n";
        return;
    }

    MedicalHistory mh;
    mh.id = ++medicalHistoryCount; // Pre-increment
    mh.supervisorId = supervisorId;

    cout << "\n--- Record Medical History ---\n";

    mh.donorId = getIntegerInput("Enter Donor ID: ");

    if (!donor_exists(mh.donorId)) {
        cout << "Donor ID " << mh.donorId << " not found. Aborting.\n";
        medicalHistoryCount--; // Decrement count as record is not saved
        return;
    }

    do {
        cout << "Entry Date (YYYY-MM-DD): ";
        safeGetline(mh.entryDate, 11);
        if (!validate_date(mh.entryDate)) {
            cout << "Invalid date format. Please use YYYY-MM-DD.\n";
        }
    } while (!validate_date(mh.entryDate));

    // Add more robust validation (e.g., only allow "Positive" or "Negative")
    cout << "HIV Status (Positive/Negative): ";
    safeGetline(mh.hivStatus, 20);

    cout << "Syphilis Status (Positive/Negative): ";
    safeGetline(mh.syphilisStatus, 20);

    cout << "Hepatitis Status (Positive/Negative): ";
    safeGetline(mh.hepatitisStatus, 20);

    mh.sugarLevel = getFloatInput("Sugar Level: ");

    cout << "Outcome Message (e.g., Eligible, Deferred): ";
    safeGetline(mh.outcomeMessage, 500);

    medicalHistories[medicalHistoryCount - 1] = mh;
    cout << "Medical history recorded successfully for Donor ID " << mh.donorId << ".\n\n";
}

void sendHealthStatus(int supervisorId) {
     if (healthStatusCount >= MAX_RECORDS) {
        cout << "Maximum health status record limit reached.\n";
        return;
    }

    HealthStatus hs;
    hs.id = ++healthStatusCount; // Pre-increment
    hs.supervisorId = supervisorId;

    cout << "\n--- Record Donation Health Status ---\n";

    hs.donorId = getIntegerInput("Enter Donor ID: ");

    if (!donor_exists(hs.donorId)) {
        cout << "Donor ID " << hs.donorId << " not found. Aborting.\n";
         healthStatusCount--; // Decrement count as record is not saved
        return;
    }

    do {
        cout << "Donation Date (YYYY-MM-DD): ";
        safeGetline(hs.donationDate, 11);
         if (!validate_date(hs.donationDate)) {
            cout << "Invalid date format. Please use YYYY-MM-DD.\n";
        }
    } while (!validate_date(hs.donationDate));

    hs.weight = getFloatInput("Weight (kg): ");

    cout << "Blood Pressure (e.g., 120/80): ";
    safeGetline(hs.bloodPressure, 20); // Add validation if needed

    hs.sugarLevel = getFloatInput("Sugar Level (Pre-donation): ");


    do {
        cout << "Donation Start Time (HH:MM): ";
        safeGetline(hs.startTime, 6);
        if (!validate_time(hs.startTime)) {
            cout << "Invalid time format. Please use HH:MM.\n";
        }
    } while (!validate_time(hs.startTime));

    do {
        cout << "Donation End Time (HH:MM): ";
        safeGetline(hs.endTime, 6);
        if (!validate_time(hs.endTime)) {
            cout << "Invalid time format. Please use HH:MM.\n";
        }
        // Optional: Add check if end time is after start time
    } while (!validate_time(hs.endTime));

    healthStatuses[healthStatusCount - 1] = hs;
    cout << "Health status recorded successfully for Donor ID " << hs.donorId << ".\n\n";
}

void approveAppointment(int supervisorId) {
    cout << "\n--- Approve Appointments ---\n";
    cout << "List of Scheduled Appointments:\n";
    bool foundScheduled = false;
    for (int i = 0; i < appointmentCount; ++i) {
        if (strcmp(appointments[i].status, "Scheduled") == 0) {
            foundScheduled = true;
            const Appointment& a = appointments[i]; // Use const reference
            cout << "Appointment ID: " << a.id << "\n";
            cout << "Donor ID: " << a.donorId << "\n";
            // Optionally display donor name here by looking up Donor ID
            cout << "Date: " << a.date << " Time: " << a.time << "\n";
            cout << "Status: " << a.status << "\n";
            cout << "----------------------------\n";
        }
    }

    if (!foundScheduled) {
        cout << "No appointments currently scheduled for approval.\n\n";
        return;
    }

    int apptId = getIntegerInput("Enter Appointment ID to approve (or 0 to cancel): ");

    if (apptId == 0) {
        cout << "Approval cancelled.\n";
        return;
    }


    bool found = false;
    for (int i = 0; i < appointmentCount; ++i) {
        if (appointments[i].id == apptId) {
             if (strcmp(appointments[i].status, "Scheduled") == 0) {
                strcpy(appointments[i].status, "Approved");
                appointments[i].supervisorId = supervisorId; // Record which supervisor approved
                cout << "Appointment ID " << apptId << " approved by Supervisor ID " << supervisorId << ".\n\n";
                found = true;
                break;
             } else {
                 cout << "Appointment ID " << apptId << " is not in 'Scheduled' status (Current status: " << appointments[i].status << ").\n\n";
                 found = true; // Found it, but wrong status
                 break;
             }
        }
    }

    if (!found) {
        cout << "Appointment ID " << apptId << " not found.\n\n";
    }
}


void supervisorMenu(int supervisorId) {
    int option;
    do {
        cout << "\n--- Supervisor Menu (Logged in as Supervisor ID: " << supervisorId << ") ---\n";
        cout << "1. View Donors\n";
        cout << "2. Record Medical History\n";
        cout << "3. Record Donation Health Status\n";
        cout << "4. Approve Appointment\n";
        cout << "5. Back to Main Menu\n";

        option = getIntegerInput("Select option: ");


        switch (option) {
            case 1:
                viewDonors();
                break;
            case 2:
                sendMedicalHistory(supervisorId);
                break;
            case 3:
                sendHealthStatus(supervisorId);
                break;
            case 4:
                approveAppointment(supervisorId);
                break;
            case 5:
                cout << "Returning to Main Menu...\n";
                break; // Exit the loop, return to main
            default:
                cout << "Invalid option! Please try again.\n";
        }
    } while (option != 5);
}

void donorMenu() {
    int option;
    do {
        cout << "\n--- Donor Menu ---\n";
        cout << "1. Register as a New Donor\n";
        cout << "2. Schedule Donation Appointment\n";
        cout << "3. View My Medical History\n";
        cout << "4. View My Health Status Records\n";
        cout << "5. Back to Main Menu\n";
        cout << "6. Exit Application\n";

        option = getIntegerInput("Select option: ");

        switch (option) {
            case 1:
                donor_registration();
                break;
            case 2:
                schedule_appointment();
                break;
            case 3:
                viewMedicalHistory();
                break;
            case 4:
                viewHealthStatus();
                break;
            case 5:
                 cout << "Returning to Main Menu...\n";
                 // The loop condition will handle returning
                break;
            case 6:
                cout << "Exiting Blood Donation System. Goodbye!\n";
                exit(0); // Terminate the program
            default:
                cout << "Invalid option! Please try again.\n";
        }
        // No need for explicit return on case 5, loop condition handles it
    } while (option != 5); // Loop continues until option 5 is chosen
}

int main() {
    // Initialize default supervisor(s) - replace with secure creation/loading later
    if (supervisorCount < MAX_RECORDS) {
         supervisors[supervisorCount++] = {nextSupervisorId++, "admin", custom_hash("password123")}; // Example 1
    }
     if (supervisorCount < MAX_RECORDS) {
         supervisors[supervisorCount++] = {nextSupervisorId++, "sup1", custom_hash("sup1pass")}; // Example 2
    }

    cout << "--- Welcome to the Blood Donation Management System ---\n";

    int mainOption;
    do {
        cout << "\n--- Main Menu ---\n";
        cout << "1. Donor Portal\n";
        cout << "2. Supervisor Portal\n";
        cout << "3. Exit\n";

        mainOption = getIntegerInput("Select portal: ");

        switch (mainOption) {
            case 1:
                donorMenu();
                break;
            case 2:
                { // Create a scope for supervisorId
                    int supervisorId = supervisorLogin();
                    if (supervisorId != -1) { // Check for successful login
                        supervisorMenu(supervisorId);
                    }
                } // supervisorId goes out of scope here
                break;
            case 3:
                cout << "Exiting Blood Donation System. Goodbye!\n";
                break; // Exit the main loop
            default:
                cout << "Invalid option! Please choose 1, 2, or 3.\n";
        }
    } while (mainOption != 3); // Loop until user chooses to exit

    return 0; // Indicate successful execution
}