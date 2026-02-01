#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <thread> // for time delay
#include <chrono> // for time delay

using namespace std;


// Structure to store attendance record

struct Student {
    int id;
    string name;
    int status;
};

Student attendance[100];
int recordCount = 0;
string termName;


// Function prototypes (PART 1 - MEMBER)

bool openAttendanceFile(fstream &file, string filename);
void loadAttendance(fstream &file);
void displayAttendance();
void saveAttendance(string filename);


void StudentTrackerDescription () // Description display for main menu
{
    cout << "Student Attendance Tracker is a C++ program that was built for tracking attendance." << endl;
    cout << "It makes tracking attendance more efficient by storing student datas." << endl;
}



void createSchoolTerm() // Create school term (Database)
{
    cout << "Create School Term (Database)\n";
    cout << "---------------------------------\n";
    cout << "Enter term name: ";
    cin >> termName;
    cout << "Database \"" << termName << "\" created and loaded.\n\n";
}


// Function prototypes (PART 2)

int findStudentIndex(int id);
int inputInt(const string& msg);
int inputStatus();
void updateAttendanceRow();
void deleteAttendanceRow();


void IntroductionProcess() // Introduction process so the program runs after user make their choice
{
    fstream dataFile;
    string filename;

    createSchoolTerm();

    cout << "Reading attendance data from file...\n";
    cout << "Enter attendance filename: ";
    cin >> filename;

    if (openAttendanceFile(dataFile, filename)) {
        cout << "File opened successfully.\n\n";
        loadAttendance(dataFile);
        dataFile.close();
        displayAttendance();
    } else {
        cout << "File not found. Starting with empty attendance sheet.\n\n";
        recordCount = 0;
    }

    int choice;
    do {
        cout << "---------------------------------\n";
        cout << "1. Display Attendance\n";
        cout << "2. Update Attendance Row\n";
        cout << "3. Delete Attendance Row\n";
        cout << "0. Exit\n";
        cout << "Choose: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayAttendance();
                break;

            case 2:
                updateAttendanceRow();
                saveAttendance(filename);
                break;

            case 3:
                deleteAttendanceRow();
                saveAttendance(filename);
                break;

            case 0:
                saveAttendance(filename);
                cout << "Attendance data saved. Exiting...\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 0);
}

void introduction() // function that holds the entire program
{
    fstream dataFile;
    string filename;
    string option;

    cout << "\nWeclome to the 'Student Attendance Tracker'. Please select an option below. (1-4)" << endl;
    cout << "Option 1: Create School Term Database" << endl;
    cout << "Option 2: What is Student Attendance Tracker" << endl;
    cout << "Option 3: View Row Counts" << endl;
    cout << "Option 4: Quit" << endl;

    cin >> option;

    if (option == "1")
    {
        cout << "============================================\n" << endl;
        IntroductionProcess();
    }

    else if (option == "2")
    {
        StudentTrackerDescription();
        this_thread::sleep_for(chrono::seconds(2));
        cout << "============================================\n" << endl;

        introduction();
    }

    else if (option == "3")
    {
        cout << "WIP" << endl;
        this_thread::sleep_for(chrono::seconds(2));
        cout << "============================================\n" << endl;

        introduction();
    }


    else if (option == "4")
    {
        cout << "This early?! Goodbye." << endl;
        return;
    }

    else
    {
        cout << "Invalid Input. Please try again." << endl;
        cout << "============================================\n" << endl;
        introduction();
    }

}



/*
    PART 1 FUNCTIONS (MEMBER CODE)
*/
bool openAttendanceFile(fstream &file, string filename) {
    file.open(filename, ios::in);
    return !file.fail();
}

void loadAttendance(fstream &file) {
    string line;
    recordCount = 0;

    getline(file, line); // skip header

    while (getline(file, line) && recordCount < 100) {
        stringstream ss(line);
        string temp;

        getline(ss, temp, ',');
        attendance[recordCount].id = stoi(temp);

        getline(ss, attendance[recordCount].name, ',');

        getline(ss, temp, ',');
        attendance[recordCount].status = stoi(temp);

        recordCount++;
    }
}

void displayAttendance() {
    cout << "---------------------------------\n";
    cout << "Current Attendance Sheet\n";
    cout << "---------------------------------\n";
    cout << "StudentID, Name, Status\n";

    if (recordCount == 0) {
        cout << "(empty)\n\n";
        return;
    }

    for (int i = 0; i < recordCount; i++) {
        cout << attendance[i].id << ", "
             << attendance[i].name << ", "
             << attendance[i].status << "\n";
    }
    cout << endl;
}

void saveAttendance(string filename) {
    fstream file;
    file.open(filename, ios::out);

    file << "StudentID,Name,Status\n";
    for (int i = 0; i < recordCount; i++) {
        file << attendance[i].id << ","
             << attendance[i].name << ","
             << attendance[i].status << "\n";
    }
    file.close();
}


/*
    PART 2 FUNCTIONS (You)
*/
void updateAttendanceRow() {
    cout << "------------------------------\n";
    cout << "Update Attendance Row\n";
    cout << "------------------------------\n";

    int id = inputInt("Enter StudentID to update: ");
    int index = findStudentIndex(id);

    if (index == -1) {
        cout << "ERROR: StudentID not found in CSV file.\n\n";
        return;
    }

    attendance[index].status = inputStatus();
    cout << "Row updated successfully.\n\n";
    displayAttendance();
}

void deleteAttendanceRow() {
    cout << "------------------------------\n";
    cout << "Delete Attendance Row\n";
    cout << "------------------------------\n";

    int id = inputInt("Enter StudentID to delete: ");
    int index = findStudentIndex(id);

    if (index == -1) {
        cout << "ERROR: StudentID not found in CSV file.\n\n";
        return;
    }

    for (int i = index; i < recordCount - 1; i++) {
        attendance[i] = attendance[i + 1];
    }
    recordCount--;

    cout << "Row deleted successfully.\n\n";
    displayAttendance();
}
/*
    PART 3 FUNCTIONS (Member)
*/
int findStudentIndex(int id) {
    for (int i = 0; i < recordCount; i++) {
        if (attendance[i].id == id)
            return i;
    }
    return -1;
}

int inputInt(const string& msg) {
    int value;
    while (true) {
        cout << msg;
        cin >> value;

        if (!cin.fail())
            return value;

        cout << "ERROR: Please enter a valid INTEGER.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int inputStatus() {
    int s;
    while (true) {
        s = inputInt("Enter new status (1 = Present, 0 = Absent): ");
        if (s == 0 || s == 1)
            return s;

        cout << "ERROR: Status must be 0 or 1.\n";
    }
}

int main() {

    cout << "============================================\n";
    cout << " STUDENT ATTENDANCE TRACKER - MILESTONE 2\n";
    cout << "============================================\n\n";

    introduction();


    return 0;
}

