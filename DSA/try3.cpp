#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <functional>

using namespace std;

// Utility functions
vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int get_last_id(const string& filename) {
    ifstream file(filename);
    if (!file) return 0;

    string line;
    int max_id = 0;
    getline(file, line); // Skip header
    while (getline(file, line)) {
        vector<string> fields = split(line, '|');
        if (!fields.empty()) {
            int current_id = stoi(fields[0]);
            if (current_id > max_id) max_id = current_id;
        }
    }
    return max_id;
}

bool validate_phone(const string& phone) {
    regex pattern("(09|07)\\d{8}");
    return regex_match(phone, pattern);
}

bool validate_date(const string& date) {
    regex pattern(R"(\d{4}-\d{2}-\d{2})");
    return regex_match(date, pattern);
}

bool validate_time(const string& time) {
    regex pattern(R"(\d{2}:\d{2})");
    if (!regex_match(time, pattern)) return false;
    int hours = stoi(time.substr(0, 2));
    int minutes = stoi(time.substr(3, 2));
    return hours >= 0 && hours < 24 && minutes >= 0 && minutes < 60;
}

// Donor functions
bool donor_exists(int donor_id) {
    ifstream file("donors.txt");
    if (!file) return false;

    string line;
    getline(file, line); // Skip header
    while (getline(file, line)) {
        vector<string> fields = split(line, '|');
        if (stoi(fields[0]) == donor_id) return true;
    }
    return false;
}

void donor_registration() {
    ofstream addr_file("addresses.txt", ios::app);
    ofstream donor_file("donors.txt", ios::app);
    ofstream phone_file("phones.txt", ios::app);

    // Write headers if files are new
    if (addr_file.tellp() == 0) addr_file << "AddressID|City|Region|Wereda|Subcity\n";
    if (donor_file.tellp() == 0) donor_file << "DonorID|FirstName|LastName|DateOfBirth|Gender|BloodType|AddressID\n";
    if (phone_file.tellp() == 0) phone_file << "PhoneID|DonorID|PhoneNumber|PhoneType\n";

    // Address information
    int address_id = get_last_id("addresses.txt") + 1;
    string city, region, wereda, subcity;
    cout << "\n--- Donor Registration ---\n";
   

    // Donor information
    int donor_id = get_last_id("donors.txt") + 1;
    string first_name, last_name, dob, gender, blood_type;
    cout << "First Name: "; getline(cin, first_name);
    cout << "Last Name: "; getline(cin, last_name);
    
    do {
        cout << "Date of Birth (YYYY-MM-DD): "; getline(cin, dob);
    } while (!validate_date(dob));
    
    cout << "Gender (Male/Female/Other): "; getline(cin, gender);
    
    vector<string> valid_blood = {"A+", "O-", "B+", "AB-", "I don't know"};
    while (find(valid_blood.begin(), valid_blood.end(), blood_type) == valid_blood.end()) {
        cout << "Blood Type (A+/O-/B+/AB-/I don't know): "; getline(cin, blood_type);
    }
    
    donor_file << donor_id << "|" << first_name << "|" << last_name << "|" << dob << "|"
              << gender << "|" << blood_type << "|" << address_id << "\n";

    // Phone numbers
    vector<string> phones;
    string phone;
    do {
        cout << "Phone Number (09/07xxxxxxxx): "; getline(cin, phone);
        while (!validate_phone(phone)) {
            cout << "Invalid phone! Re-enter: "; getline(cin, phone);
        }
        phones.push_back(phone);
        cout << "Add another? (y/n): ";
        char c; cin >> c; cin.ignore();
        if (tolower(c) != 'y') break;
    } while (true);

    for (const auto& p : phones) {
        int phone_id = get_last_id("phones.txt") + 1;
        phone_file << phone_id << "|" << donor_id << "|" << p << "|Mobile\n";
    }
    cout << "City: "; getline(cin, city);
    cout << "Region: "; getline(cin, region);
    cout << "Wereda (optional): "; getline(cin, wereda);
    cout << "Subcity (optional): "; getline(cin, subcity);
    addr_file << address_id << "|" << city << "|" << region << "|" << wereda << "|" << subcity << "\n";
    cout << "\nRegistration Complete! Your DonorID: " << donor_id << "\n\n";
}

void schedule_appointment() {
    int donor_id;
    cout << "Enter DonorID: "; cin >> donor_id; cin.ignore();
    if (!donor_exists(donor_id)) {
        cout << "Invalid DonorID!\n";
        return;
    }

    ofstream appt_file("appointments.txt", ios::app);
    if (appt_file.tellp() == 0) appt_file << "AppointmentID|DonorID|SupervisorID|AppointmentDate|AppointmentTime|Status\n";

    int appt_id = get_last_id("appointments.txt") + 1;
    string date, time;
    
    do {
        cout << "Date (YYYY-MM-DD): "; getline(cin, date);
    } while (!validate_date(date));
    
    do {
        cout << "Time (HH:MM): "; getline(cin, time);
    } while (!validate_time(time));

    appt_file << appt_id << "|" << donor_id << "|0|" << date << "|" << time << "|Scheduled\n";
    cout << "Appointment Scheduled! ID: " << appt_id << "\n";
}

void donor_menu() {
    while (true) {
        cout << "\nDonor Menu\n1. Register\n2. Schedule Appointment\n3. Exit\nChoice: ";
        int choice; cin >> choice; cin.ignore();
        switch (choice) {
            case 1: donor_registration(); break;
            case 2: schedule_appointment(); break;
            case 3: return;
            default: cout << "Invalid choice!\n";
        }
    }
}

// Supervisor functions
void view_donors() {
    ifstream file("donors.txt");
    if (!file) { cout << "No donors found!\n"; return; }

    string line;
    getline(file, line); // Skip header
    cout << "\nRegistered Donors:\n";
    while (getline(file, line)) {
        vector<string> fields = split(line, '|');
        cout << "ID: " << fields[0] << " | Name: " << fields[1] << " " << fields[2] 
             << " | Blood Type: " << fields[5] << "\n";
    }
}

void approve_appointments(int supervisor_id) {
    ifstream in("appointments.txt");
    if (!in) { cout << "No appointments!\n"; return; }

    vector<string> appointments;
    string line;
    getline(in, line); // Skip header
    while (getline(in, line)) appointments.push_back(line);
    in.close();

    vector<string> pending;
    for (auto& appt : appointments) {
        vector<string> fields = split(appt, '|');
        if (fields[5] == "Scheduled") pending.push_back(appt);
    }

    if (pending.empty()) { cout << "No pending appointments!\n"; return; }

    cout << "\nPending Appointments:\n";
    for (size_t i = 0; i < pending.size(); ++i) {
        vector<string> fields = split(pending[i], '|');
        cout << i+1 << ". ID: " << fields[0] << " | Donor: " << fields[1]
             << " | Date: " << fields[3] << " " << fields[4] << "\n";
    }

    cout << "Select appointment to approve (0 to cancel): ";
    int choice; cin >> choice; cin.ignore();
    if (choice < 1 || choice > pending.size()) return;

    string selected = pending[choice-1];
    vector<string> fields = split(selected, '|');
    fields[2] = to_string(supervisor_id);
    fields[5] = "Approved";

    // Update in main list
    for (auto& appt : appointments) {
        if (split(appt, '|')[0] == fields[0]) {
            appt = "";
            for (size_t i = 0; i < fields.size(); ++i) {
                if (i > 0) appt += "|";
                appt += fields[i];
            }
            break;
        }
    }

    // Rewrite file
    ofstream out("appointments.txt");
    out << "AppointmentID|DonorID|SupervisorID|AppointmentDate|AppointmentTime|Status\n";
    for (const auto& a : appointments) out << a << "\n";
    cout << "Appointment approved!\n";
}

void supervisor_menu(int supervisor_id) {
    while (true) {
        cout << "\nSupervisor Menu\n1. View Donors\n2. Approve Appointments\n3. Exit\nChoice: ";
        int choice; cin >> choice; cin.ignore();
        switch (choice) {
            case 1: view_donors(); break;
            case 2: approve_appointments(supervisor_id); break;
            case 3: return;
            default: cout << "Invalid choice!\n";
        }
    }
}

bool supervisor_login(int& supervisor_id) {
    string user, pass;
    cout << "Username: "; getline(cin, user);
    cout << "Password: "; getline(cin, pass);
    
    size_t pass_hash = hash<string>{}(pass);
    ifstream file("supervisors.txt");
    if (!file) return false;

    string line;
    getline(file, line); // Skip header
    while (getline(file, line)) {
        vector<string> fields = split(line, '|');
        if (fields[1] == user && stoull(fields[2]) == pass_hash) {
            supervisor_id = stoi(fields[0]);
            return true;
        }
    }
    return false;
}

// Main menu
int main() {
    // Create default supervisor if not exists
    ifstream sfile("supervisors.txt");
    if (!sfile) {
        ofstream out("supervisors.txt");
        out << "SupervisorID|Username|PasswordHash\n";
        out << "1|admin|" << hash<string>{}("admin123") << "\n";
    }

    while (true) {
        cout << "\nMain Menu\n1. Donor\n2. Supervisor\n3. Exit\nChoice: ";
        int choice; cin >> choice; cin.ignore();
        switch (choice) {
            case 1: donor_menu(); break;
            case 2: {
                int supervisor_id;
                if (supervisor_login(supervisor_id)) {
                    supervisor_menu(supervisor_id);
                } else {
                    cout << "Login failed!\n";
                }
                break;
            }
            case 3: return 0;
            default: cout << "Invalid choice!\n";
        }
    }
}