#include <iostream>
using namespace std;
struct doner{
   int id;
  char first_name[50];
  char last_name[50];
  char dob[11]; // YYYY-MM-DD
  int age;
  char gender[10];
  char blood_type[20];
  int address_id;
};
struct address{
  int id;
  char city[50];
  char region[50];
  char wereda[50];
  char subcity[50];
};
struct phone{
  int id;
  int doner_id;
  char number[15];
  char type[10]; // e.g., Mobile, Home
};
struct appointment{
  int id;
  int doner_id;
  int supervisor_id; // ID of the supervisor who approved/managed
  char date[11]; // YYYY-MM-DD
  char time[6]; // HH:MM
  char status[20]; // e.g., Scheduled, Approved, Completed, Cancelled
};
struct supervisor{
  int id;
  char username[50];
  size_t password_hash;
};
struct medical_history{
  int id;
  int doner_id;
  int supervisor_id; // ID of the supervisor who recorded the history
  char entry_date[11]; // YYYY-MM-DD
  char hiv_status[20]; // Positive/Negative
  char syphilis_status[20]; // Positive/Negative
  char hepatitis_status[20]; // Positive/Negative
  float sugar_level;
  char outcome_message[500]; // e.g., "Eligible for donation", "Deferred due to..."
};

struct health_status{
  int id;
  int doner_id;
  int supervisor_id; // ID of the supervisor who recorded the status
  char entry_date[11]; // YYYY-MM-DD
  char blood_pressure[20]; // e.g., "120/80"
  float weight;
  char start_time[6]; // HH:MM
  char end_time[6]; // HH:MM
  float sugar_level;
  char donation_date[11]; // YYYY-MM-DD
};
void donerregistration(){
  cout<<"*****welcome to doner registraction*****\n";

  doner d;
  cout<<"Enter first name: ";
  cin>>d.first_name;
  cout<<"Enter last name: ";
  cin>>d.last_name;
  cout<<"Enter date of birth (YYYY-MM-DD): ";
  cin>>d.dob;
  cout<<"Enter age: ";
  cin>>d.age}
void donermenu(){
  cout<<"doner menu\n";
  cout<<"1. register\n2. schedule appointment\n3. exit\n";
  int choice;
  cout<<"Enter your choice: ";
  cin>>choice;
  switch(choice){
    case 1:
      doner_registration();
      break;
    case 2:
      //schedule_appointment();
      break;
    case 3:
      cout<<"exit\n";
      break;
    default:
      cout<<"Invalid choice\n";
  }
}
void supervisormenu(){
  cout<<"supervisor menu\n";
  cout<<"1. view donors\n2. approve appointments\n3. exit\n";
  int choice;
  cout<<"Enter your choice: ";
  cin>>choice;
  switch(choice){
    case 1:
      //view_donors();
      break;
    case 2:
      //approve_appointments();
      break;
    case 3:
      cout<<"exit\n";
      break;
    default:
      cout<<"Invalid choice\n";
  }
}
int main(){
  cout<<"Welcome to the Blood Donation System\n";
  cout<<"1. Doner\n2. Supervisor\n3. Exit\n";
  int choice;
  cout<<"Enter your choice: ";
  cin>>choice;
  switch(choice){
    case 1:
      donermenu();
      break;
    case 2:
      supervisormenu();
      break;
    case 3:
      cout<<"exit\n";
      break;
    default:
      cout<<"Invalid choice\n";
  }
  
}
