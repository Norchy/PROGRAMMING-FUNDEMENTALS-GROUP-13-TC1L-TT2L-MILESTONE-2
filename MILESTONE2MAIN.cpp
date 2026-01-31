#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

/*
    Structure to store attendance record
*/
struct Student {
    int id;
    string name;
    int status;
};

Student attendance[100];
int recordCount = 0;
string termName;

/*
    Function prototypes
*/
bool openAttendanceFile(fstream &file, string filename);
void loadAttendance(fstream &file);
void displayAttendance();
void saveAttendance(string filename);

/*
    Create school term (Database)
*/
void createSchoolTerm() {
    cout << "Create School Term (Database)\n";
    cout << "---------------------------------\n";
    cout << "Enter term name: ";
    cin >> termName;
    cout << "Database \"" << termName << "\" created and loaded.\n\n";
}

int main() {
    fstream dataFile;
    string filename;

    cout << "============================================\n";
    cout << " STUDENT ATTENDANCE TRACKER - MILESTONE 2\n";
    cout << "============================================\n\n";

    createSchoolTerm();

    cout << "Reading attendance data from file...\n";
    cout << "Enter attendance filename: ";
    cin >> filename;

    if (openAttendanceFile(dataFile, filename)) {
        cout << "File opened successfully.\n\n";

        loadAttendance(dataFile);
        dataFile.close();

        displayAttendance();
        saveAttendance(filename);

        cout << "Attendance data saved back to file.\n";
    }
    else {
        cout << "File open error.\n";
    }

    return 0;
}

/*
    Opens attendance file.
    Accepts fstream object by reference.
    Returns true if file opens successfully.
*/
bool openAttendanceFile(fstream &file, string filename) {
    file.open(filename, ios::in);
    if (file.fail())
        return false;
    else
        return true;
}

/*
    Loads attendance data from file into memory
*/
void loadAttendance(fstream &file) {
    string line;
    recordCount = 0;

    // Skip header
    getline(file, line);

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

/*
    Displays initial attendance sheet
*/
void displayAttendance() {
    cout << "---------------------------------\n";
    cout << "Current Attendance Sheet\n";
    cout << "---------------------------------\n";
    cout << "StudentID, Name, Status\n";

    for (int i = 0; i < recordCount; i++) {
        cout << attendance[i].id << ", "
             << attendance[i].name << ", "
             << attendance[i].status << endl;
    }
    cout << endl;
}

/*
    Saves attendance data back to CSV file
*/
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
