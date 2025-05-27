#include <iostream>
#include <limits> // For numeric_limits
#include <cctype>
#include <ctime>

using namespace std;
struct Donor {
    string firstName;
    string lastName;
    string gender;
    string phone;
    string username;
    string password;
    string bloodType;
    string email;
    string city;
    string region;
    string kebele;
    string worda;
    Donor* next;
};


Donor* donorHead = nullptr;
struct Appointment {
    string donorUsername;
    string date;  
    string time;  
    string message;
    Appointment* next;
};

Appointment* appointmentHead = nullptr;

string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    char dateStr[11];
    sprintf(dateStr, "%04d-%02d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);

    return string(dateStr);
}

bool isDateValid(const string& inputDate) {
    string today = getCurrentDate();
    return inputDate >= today;
}

void addAppointment(const string& donorUsername, const string& date, const string& time, const string& message) {
    Appointment* newApp = new Appointment;
    newApp->donorUsername = donorUsername;
    newApp->date = date;
    newApp->time = time;
    newApp->message = message;
    newApp->next = nullptr;

    if (appointmentHead == nullptr) {
        appointmentHead = newApp;
    } else {
        Appointment* temp = appointmentHead;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newApp;
    }
}


// Check if string contains only letters (a-zA-Z)
bool isAlphaString(const string& s) {
    for (char ch : s) {
        if (!isalpha(ch)) return false;
    }
    return !s.empty();
}

// Check if gender is male or female (case insensitive)
bool isValidGender(const string& gender) {
    string g = gender;
    for (auto& c : g) c = tolower(c);
    return g == "male" || g == "female";
}

// Check if string is a 10-digit phone starting with 09 or 07
bool isValidPhone(const string& phone) {
    if (phone.size() != 10) return false;
    if (phone.substr(0, 2) != "09" && phone.substr(0, 2) != "07") return false;
    for (char ch : phone) {
        if (!isdigit(ch)) return false;
    }
    return true;
}

// Check password length > 6
bool isValidPassword(const string& pass) {
    return pass.length() > 6;
}

// Check blood type validity or empty
bool isValidBloodType(const string& blood) {
    const string validTypes[] = {"A", "A+", "A-", "B", "B+", "B-", "AB", "O", "O+", "O-"};
    if (blood.empty()) return true;
    for (const auto& t : validTypes) {
        if (blood == t) return true;
    }
    return false;
}

// Basic email validation: contains '@' and '.'
bool isValidEmail(const string& email) {
    if (email.empty()) return true; // optional
    size_t at_pos = email.find('@');
    size_t dot_pos = email.find('.', at_pos);
    return (at_pos != string::npos && dot_pos != string::npos);
}

// Function declarations
void donorDashboard();

void registerDonor();
void donorLoginAndDashboard();
void makeAppointment(const Donor* currentDonor);

void supervisorDashboard();
void viewDonors();
void sendMedicalHistory();
void sendHealthStatus();
void mainMenu();

void mainMenu() {
    int choice;
    do {
        cout << "\n===== Blood Bank System =====\n";
        cout << "1. Donor\n";
        cout << "2. Supervisor\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";

        cin >> choice;
        if (cin.fail()) {
            cin.clear(); // clear error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
            cout << "❌ Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1:
                donorDashboard();
                break;
            case 2:
                supervisorDashboard();
                break;
            case 3:
                cout << "👋 Exiting system. Goodbye!\n";
                return;
            default:
                cout << "❌ Invalid choice. Please try again.\n";
        }
    } while (true);
}

void donorDashboard() {
    int choice;
    do {
        cout << "\n--- Donor Dashboard ---\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit Program\n";
        cout << "4. Back to Main Menu\n";
        cout << "Enter your choice: ";

        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "❌ Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1:
                cout << "[Register Functionality will be here]\n";
                registerDonor();
                break;
            case 2:
                cout << "[Login Functionality will be here]\n";
               donorLoginAndDashboard();
                break;
            case 3:
                cout << "Exiting the program. Goodbye!\n";
                exit(0);  // Exit whole program
                break;   // break is technically unreachable, but good style
            case 4:
                return;   // Back to main menu
            default:
                cout << "❌ Invalid choice. Try again.\n";
        }
    } while (true);
}
void registerDonor() {
    Donor* newDonor = new Donor();

    cout << "\n--- Donor Registration ---\n";

    // First name
    do {
        cout << "First Name: ";
        cin >> newDonor->firstName;
        if (!isAlphaString(newDonor->firstName))
            cout << "❌ Invalid first name. Use letters only.\n";
    } while (!isAlphaString(newDonor->firstName));

    // Last name
    do {
        cout << "Last Name: ";
        cin >> newDonor->lastName;
        if (!isAlphaString(newDonor->lastName))
            cout << "❌ Invalid last name. Use letters only.\n";
    } while (!isAlphaString(newDonor->lastName));

    // Gender
    do {
        cout << "Gender (male/female): ";
        cin >> newDonor->gender;
        if (!isValidGender(newDonor->gender))
            cout << "❌ Invalid gender. Enter 'male' or 'female'.\n";
    } while (!isValidGender(newDonor->gender));

    // Phone number
    do {
        cout << "Phone Number (start with 09 or 07, 10 digits): ";
        cin >> newDonor->phone;
        if (!isValidPhone(newDonor->phone))
            cout << "❌ Invalid phone number.\n";
    } while (!isValidPhone(newDonor->phone));

    // Username
    do {
        cout << "Username: ";
        cin >> newDonor->username;
        if (newDonor->username.empty())
            cout << "❌ Username cannot be empty.\n";
    } while (newDonor->username.empty());

    // Password + confirm password
    string confirmPass;
    do {
        cout << "Password (min 7 chars): ";
        cin >> newDonor->password;
        if (!isValidPassword(newDonor->password)) {
            cout << "❌ Password too short.\n";
            continue;
        }
        cout << "Confirm Password: ";
        cin >> confirmPass;
        if (confirmPass != newDonor->password)
            cout << "❌ Passwords do not match.\n";
    } while (!isValidPassword(newDonor->password) || confirmPass != newDonor->password);

    // Blood type (optional)
    do {
        cout << "Blood Type (optional, e.g., A, B+, O-): ";
        cin >> newDonor->bloodType;
        if (!isValidBloodType(newDonor->bloodType))
            cout << "❌ Invalid blood type.\n";
    } while (!isValidBloodType(newDonor->bloodType));

    // Email (optional)
    do {
        cout << "Email (optional): ";
        cin >> newDonor->email;
        if (!isValidEmail(newDonor->email))
            cout << "❌ Invalid email format.\n";
    } while (!isValidEmail(newDonor->email));

    // City
    do {
        cout << "City: ";
        cin >> newDonor->city;
        if (!isAlphaString(newDonor->city))
            cout << "❌ Invalid city. Use letters only.\n";
    } while (!isAlphaString(newDonor->city));

    // Region
    do {
        cout << "Region: ";
        cin >> newDonor->region;
        if (!isAlphaString(newDonor->region))
            cout << "❌ Invalid region. Use letters only.\n";
    } while (!isAlphaString(newDonor->region));

    // Kebele
    do {
        cout << "Kebele: ";
        cin >> newDonor->kebele;
        if (!isAlphaString(newDonor->kebele))
            cout << "❌ Invalid kebele. Use letters only.\n";
    } while (!isAlphaString(newDonor->kebele));

    // Worda
    do {
        cout << "Worda: ";
        cin >> newDonor->worda;
        if (!isAlphaString(newDonor->worda))
            cout << "❌ Invalid worda. Use letters only.\n";
    } while (!isAlphaString(newDonor->worda));

    // Add new donor to front of list
    newDonor->next = donorHead;
    donorHead = newDonor;

    cout << "✅ Donor registered successfully!\n";
}
void donorLoginAndDashboard() {
    string username, password;

    cout << "\n--- Donor Login ---\n";
    cout << "Username: ";
    cin >> username;

    cout << "Password: ";
    cin >> password;

    Donor* current = donorHead;
    while (current != nullptr) {
        if (current->username == username && current->password == password) {
            cout << "✅ Login successful! Welcome, " << current->firstName << "!\n";

            // Start donor dashboard loop
            int choice;
            do {
                cout << "\n--- Donor Dashboard ---\n";
                cout << "Welcome, " << current->firstName << " " << current->lastName << "!\n";
                cout << "1. Make Appointment\n2. View Medical Health\n3. View Health Status\n4. Logout (Back to Donor Menu)\n5. Exit\n";
                cout << "Choice: ";
                cin >> choice;

                switch (choice) {
                    case 1:
                        // TODO: Implement appointment function
                        cout << "Make Appointment selected (not implemented yet).\n";
                        makeAppointment(current);

                        break;
                    case 2:
                        // TODO: Implement medical health view function
                        cout << "View Medical Health selected (not implemented yet).\n";
                        break;
                    case 3:
                        // TODO: Implement health status view function
                        cout << "View Health Status selected (not implemented yet).\n";
                        break;
                    case 4:
                        cout << "Logging out...\n";
                        break;
                    case 5:
                        cout << "Exiting...\n";
                        exit(0);
                    default:
                        cout << "Invalid choice.\n";
                }
            } while (choice != 4);

            return; // Exit after logout
        }
        current = current->next;
    }

    cout << "❌ Invalid username or password.\n";
}
void makeAppointment(const Donor* currentDonor) {
    string date, time, message;

    cout << "\n--- Make Appointment ---\n";

    cout << "Enter appointment date (YYYY-MM-DD): ";
    cin >> date;

    if (!isDateValid(date)) {
        cout << "❌ Invalid date. Appointment date cannot be in the past.\n";
        return;
    }

    cout << "Enter appointment time (HH:MM, 24-hour): ";
    cin >> time;

    cout << "Enter a message (optional): ";
    cin.ignore();  // clear newline
    getline(cin, message);

    addAppointment(currentDonor->username, date, time, message);

    cout << "✅ Appointment successfully scheduled for " << date << " at " << time << ".\n";
}


void supervisorDashboard() {
    cout << "\n--- Welcome to Supervisor Dashboard ---\n";
    // You can add supervisor options here later
     string username, password;

    cout << "\n--- Supervisor Login ---\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    if (username == "sup1" && password == "sup123456") {
        cout << "✅ Supervisor login successful!\n";

        int choice;
        do {
            cout << "\n--- Supervisor Dashboard ---\n";
            cout << "1. View Donors\n";
            cout << "2. Send Medical History\n";
            cout << "3. Send Health Status\n";
            cout << "4. Logout (Back to Supervisor Menu)\n";
            cout << "5. Exit\n";
            cout << "Choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    viewDonors();
                    break;
                case 2:
                    sendMedicalHistory();
                    break;
                case 3:
                    sendHealthStatus();
                    break;
                case 4:
                    cout << "Logging out...\n";
                    break;
                case 5:
                    cout << "Exiting...\n";
                    exit(0);
                default:
                    cout << "Invalid choice.\n";
            }
        } while (choice != 4);
    } else {
        cout << "❌ Invalid username or password.\n";
    }
}
void viewDonors() {
    cout << "\n--- List of Donors ---\n";

    Donor* current = donorHead;
    if (!current) {
        cout << "No donors registered yet.\n";
        return;
    }

    while (current != nullptr) {
        cout << "Name: " << current->firstName << " " << current->lastName
             << ", Username: " << current->username
             << ", Phone: " << current->phoneNumber << "\n";
        current = current->next;
    }
}

void sendMedicalHistory() {
    cout << "\n--- Send Medical History ---\n";
    // To be implemented
}

void sendHealthStatus() {
    cout << "\n--- Send Health Status ---\n";
    // To be implemented
}


int main() {
    mainMenu();
    return 0;
}
