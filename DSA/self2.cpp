#include <iostream>
#include <cstring>  // For strcmp
#include <vector>   // For storing donors
using namespace std;

struct donor {
  int id;
  char first_name[50];
  char last_name[50];
  char dob[11];  // YYYY-MM-DD
  int age;
  char gender[10];
  char blood_type[20];
  int address_id;
};

struct address {
  int id;
  char city[50];
  char region[50];
  char wereda[50];
  char subcity[50];
};

struct phone {
  int id;
  int donor_id;
  char number[15];
  char type[10];  // e.g., Mobile, Home
};

vector<donor> donors;  // Vector to store donors

void donerregistration() {
  donor d;
  
  // Input first and last name
  cout << "***** Welcome to Donor Registration *****\n";
  cout << "Enter first name: ";
  cin.ignore();  // To clear any leftover newline characters
  cin.getline(d.first_name, 50);
  
  cout << "Enter last name: ";
  cin.getline(d.last_name, 50);
  
  // Input date of birth
  cout << "Enter date of birth (YYYY-MM-DD): ";
  cin >> d.dob;

  // Age validation
  while (true) {
    cout << "Enter age (18-65): ";
    cin >> d.age;
    if (d.age < 18 || d.age > 65) {
      cout << "Invalid age. Please enter an age between 18 and 65.\n";
    } else {
      break;
    }
  }

  // Gender validation
  while (true) {
    cout << "Enter your gender (male or female): ";
    cin >> d.gender;
    if (strcmp(d.gender, "male") != 0 && strcmp(d.gender, "female") != 0) {
      cout << "Invalid gender. Please enter 'male' or 'female'.\n";
    } else {
      break;
    }
  }

  // Input blood type
  cout << "Enter your blood type (A+/O-/B+/AB-/I don't know): ";
  cin >> d.blood_type;

  // Address input
  address addr;
  cout << "\nEnter address details:\n";
  cout << "City: ";
  cin.ignore();  // To clear any leftover newline characters from previous input
  cin.getline(addr.city, 50);
  
  cout << "Region: ";
  cin.getline(addr.region, 50);
  
  cout << "Wereda: ";
  cin.getline(addr.wereda, 50);
  
  cout << "Subcity: ";
  cin.getline(addr.subcity, 50);
  
  addr.id = 1;  // Assigning a dummy ID for demonstration
  d.address_id = addr.id;

  // Add donor to the donor list
  donors.push_back(d);

  cout << "\nRegistration Successful!\n";
  cout << "Donor Info: " << d.first_name << " " << d.last_name << ", Age: " << d.age << ", Gender: " << d.gender << endl;
}

void donermenu() {
  int choice;
  do {
    cout << "\nDonor Menu\n";
    cout << "1. Register\n2. Schedule Appointment\n3. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
      case 1:
        donerregistration();
        break;
      case 2:
        // schedule_appointment();
        break;
      case 3:
        cout << "Exiting...\n";
        return;
      default:
        cout << "Invalid choice. Please try again.\n";
    }
  } while (true);
}

void supervisormenu() {
  int choice;
  do {
    cout << "\nSupervisor Menu\n";
    cout << "1. View Donors\n2. Approve Appointments\n3. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
      case 1:
        // view_donors();
        break;
      case 2:
        // approve_appointments();
        break;
      case 3:
        cout << "Exiting...\n";
        return;
      default:
        cout << "Invalid choice. Please try again.\n";
    }
  } while (true);
}

int main() {
  int choice;
  do {
    cout << "Welcome to the Blood Donation System\n";
    cout << "1. Donor\n2. Supervisor\n3. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
      case 1:
        donermenu();
        break;
      case 2:
        supervisormenu();
        break;
      case 3:
        cout << "Exiting...\n";
        return 0;
      default:
        cout << "Invalid choice. Please try again.\n";
    }
  } while (true);
  
  return 0;
}
