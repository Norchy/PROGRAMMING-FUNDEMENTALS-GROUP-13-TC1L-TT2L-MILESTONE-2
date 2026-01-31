#include <iostream>
#include <string>
using namespace std;

struct Student {
    int id;
    string name;
    int status; // 0 or 1
};

void displaySheet(Student students[], int size) {
    cout << "StudentID, Name, Status\n";
    for (int i = 0; i < size; i++) {
        cout << students[i].id << ", " << students[i].name << ", " << students[i].status << "\n";
    }
}

void updateAttendanceRow(Student students[], int size) {
    cout << "----------------------------------------\n";
    cout << "Update Attendance Row\n";
    cout << "----------------------------------------\n";

    int id, newStatus;
    cout << "Enter StudentID to update: ";
    cin >> id;

    bool found = false;
    for (int i = 0; i < size; i++) {
        if (students[i].id == id) {
            cout << "Enter new status: ";
            cin >> newStatus;

            students[i].status = newStatus;
            found = true;

            cout << "Row updated successfully.\n\n";
            cout << "Updated Sheet:\n";
            displaySheet(students, size);
            cout << "\n";
            break;
        }
    }

    if (!found) {
        cout << "Error: StudentID does not exist.\n";
    }
}

void deleteAttendanceRow(Student students[], int &size) {
    cout << "----------------------------------------\n";
    cout << "Delete Attendance Row\n";
    cout << "----------------------------------------\n";

    int id;
    cout << "Enter StudentID to delete: ";
    cin >> id;

    bool found = false;
    for (int i = 0; i < size; i++) {
        if (students[i].id == id) {
            for (int j = i; j < size - 1; j++) {
                students[j] = students[j + 1];
            }
            size--;
            found = true;

            cout << "Row deleted successfully.\n\n";
            cout << "Updated Sheet:\n";
            displaySheet(students, size);
            cout << "\n";
            break;
        }
    }

    if (!found) {
        cout << "Error: StudentID does not exist.\n";
    }
}

void countRows(int size) {
    cout << "----------------------------------------\n";
    cout << "Count Rows\n";
    cout << "----------------------------------------\n";
    cout << "Number of rows: " << size << "\n";
}

int main() {
    // Example data (replace with your loaded CSV data)
    Student students[100] = {
        {1001, "John Tan", 1},
        {1002, "Mei Ling", 0},
        {1003, "Aaron Lee", 1}
    };
    int size = 3;

    // Your part (Milestone 2)
    updateAttendanceRow(students, size);
    cout << "----------------------------------------\n";
    cout << "Delete Attendance Row\n"; // matches the sample section flow
    cout << "----------------------------------------\n";
    // (If you already print the section header elsewhere, remove the 3 lines above)
    deleteAttendanceRow(students, size);

    countRows(size);

    return 0;
}
```
