#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <thread> // for time delay
#include <chrono> // for time delay
#include <iomanip> // csv mode alignments
#include <string>
#include <cctype>   // for isdigit()

using namespace std;

// *********************************************************
// Program: TC1L_GROUP13_main.cpp
// Course: CCP6114 Programming Fundamentals
// Lecture Class: TC1L
// Tutorial Class: TT2L
// Trimester: 2530
// Member_1: 252UC2425R | ARFA MIRZA BIN SHAMSUL SAFUAN | ARFA.MIRZA.SHAMSUL1@student.edu.my | 013-268 9303
// Member_2: 252UC241Q3 | EMIL SHADIQ BIN ISKANDAR | EMIL.SHADIQ.ISKANDAR1@mmu.student.edu.my | 011-1125-4415
// Member_3: 252UC24246 | DAIRELL HANNAN BIN AHMAD NIZAM | DAIRELL.HANNAN.AHMAD1@student.mmu.edu.my | 019-880 6564
// Member_4: 252UC241MF | LUQMAN HAKIM BIN MUHAMMAD FAMHI | LUQMAN.HAKIM.MUHAMMAD1@student.mmu.edu.my | 013-366-8674
// *********************************************************
// Task Distribution
// Member_1: Database & File Input/Output Lead, Documentation
// Member_2: Update & Delete Logic, Documentation
// Member_3: Validation & Error Handling, Documentation
// Member_4: Main Program, Row Count, Documentation, Milestone 1 Data saving to Mileston 2
// *********************************************************


const int MAX_COLUMNS = 10;
const int MAX_ROWS = 100;
const int MAX_INPUT_ROWS = 10; // NEW: max rows user can input per sheet

// Member 1 structure
struct Column
{
    string name;
    string type; // INT or TEXT
};

// Member 2 structure
struct Attendance
{
    int studentID;
    string name;
    int status; // 1 = Present, 0 = Absent
};

struct AttendanceRow
{
    string values[MAX_COLUMNS];
};

string termName;

struct Student {
    int id;
    string name;
    int status;
};

Student attendance[100];
int recordCount = 0;

// Member 4 - Introduction
void createSheet(string &sheetName);
int createColumnsExact(Column columns[]);
int getRowCountFromUser(); // NEW
void insertAttendanceRow(AttendanceRow sheet[], int &rowCount, Column columns[], int colCount);
void displayCSV(AttendanceRow sheet[], int rowCount, Column columns[], int colCount);

static string toLowerStr(string s)
{
    for (char &c : s)
        c = tolower(static_cast<unsigned char>(c));
    return s;
}

static bool isValidIntString(const string &s)
{
    if (s.empty()) return false;
    for (char c : s)
    {
        if (!isdigit(static_cast<unsigned char>(c)))
            return false;
    }
    return true;
}

static string trim(const string &s)
{
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

static bool parseColumnDefinition(const string &input, string &colName, string &colType)
{
    string s = trim(input);

    size_t openPos = s.find_last_of('(');
    size_t closePos = s.find_last_of(')');

    if (openPos == string::npos || closePos == string::npos || closePos < openPos)
        return false;

    string namePart = trim(s.substr(0, openPos));
    string typePart = trim(s.substr(openPos + 1, closePos - openPos - 1));

    if (namePart.empty() || typePart.empty())
        return false;

    string typeLower = toLowerStr(typePart);
    if (typeLower == "int")
        colType = "INT";
    else if (typeLower == "text")
        colType = "TEXT";
    else
        return false;

    colName = namePart;
    return true;
}


// ===============================
// Member 1: Create attendance sheet
// ===============================
void createSheet(string &sheetName)
{
    cout << "=============================================\n";
    cout << "Enter attendance sheet name: ";
    getline(cin, sheetName);

    if (sheetName.empty())
    {
        cout << "Error: Sheet name cannot be empty.\n";
        createSheet(sheetName); // ask again
    }
    else
    {
        cout << "Attendance sheet \"" << sheetName << "\" created successfully.\n";
    }
}

/*
    Member 1 (Exact sample behaviour):
    - Only accepts 3 columns
    - Prompts for the exact 3 column "names" shown in sample output
    - Automatically sets type according to the fixed format
*/
int createColumnsExact(Column columns[])
{
    string numCols;
    int colCount = 0;

    while (true)
    {
        cout << "Define number of columns (max 10): ";
        cin >> numCols;

        if (!isValidIntString(numCols))
        {
            cout << "Error: Please enter a valid number.\n";
            continue;
        }

        colCount = stoi(numCols);

        if (colCount < 1 || colCount > MAX_COLUMNS)
        {
            cout << "Error: Number of columns must be between 1 and 10.\n";
            continue;
        }

        break;
    }

    cin.ignore();

    for (int i = 0; i < colCount; i++)
    {
        while (true)
        {
            cout << "\nEnter column " << (i + 1) << " name (Ensure to put (INT) or (TEXT) at the end of the name for column types): ";
            string input;
            getline(cin, input);

            string nameParsed, typeParsed;
            if (!parseColumnDefinition(input, nameParsed, typeParsed))
            {
                cout << "Error: Please use format like \"ColumnName (INT)\" or \"ColumnName (TEXT)\".\n";
                continue;
            }

            columns[i].name = nameParsed;
            columns[i].type = typeParsed;
            break;
        }
    }

    cout << "\nSheet structure created successfully.\n";
    return colCount;
}

// NEW: let user choose how many rows to insert (max 10)
int getRowCountFromUser()
{
    string input;
    int rows = 0;

    while (true)
    {
        cout << "=============================================\n";
        cout << "Define number of rows to insert (max 10): ";
        cin >> input;

        if (!isValidIntString(input))
        {
            cout << "Error: Please enter a valid number.\n";
            continue;
        }

        rows = stoi(input);

        if (rows < 1 || rows > MAX_INPUT_ROWS)
        {
            cout << "Error: Number of rows must be between 1 and 10.\n";
            continue;
        }

        break;
    }

    return rows;
}

// ===============================
// Member 2: Insert attendance row
// ===============================
void insertAttendanceRow(AttendanceRow sheet[], int &rowCount, Column columns[], int colCount)
{
    if (rowCount >= MAX_ROWS)
    {
        cout << "Error: Attendance sheet is full.\n";
        return;
    }

    cout << "\n----------------------------------\n";
    cout << "Insert New Attendance Row\n";
    cout << "----------------------------------\n";

    for (int i = 0; i < colCount; i++)
    {
        string colNameLower = toLowerStr(columns[i].name);

        if (colNameLower == "studentid")
        {
            while (true)
            {
                cout << "Enter StudentID: " << flush;
                string v;
                getline(cin, v);
                v = trim(v);

                if (!isValidIntString(v))
                {
                    cout << "Error: Invalid INT value. Please enter a number.\n";
                    continue;
                }

                sheet[rowCount].values[i] = v;
                break;
            }
        }
        else if (colNameLower == "status")
        {
            while (true)
            {
                cout << "Enter Status (Present: 1, Absent: 0): " << flush;
                string v;
                getline(cin, v);
                v = trim(v);

                if (v != "0" && v != "1")
                {
                    cout << "Error: Status must be 0 or 1.\n";
                    continue;
                }

                sheet[rowCount].values[i] = v;
                break;
            }
        }
        else
        {
            if (columns[i].type == "INT")
            {
                while (true)
                {
                    cout << "Enter " << columns[i].name << ": " << flush;
                    string v;
                    getline(cin, v);
                    v = trim(v);

                    if (!isValidIntString(v))
                    {
                        cout << "Error: Invalid INT value. Please enter a number.\n";
                        continue;
                    }

                    sheet[rowCount].values[i] = v;
                    break;
                }
            }
            else
            {
                while (true)
                {
                    cout << "Enter " << columns[i].name << ": " << flush;
                    string v;
                    getline(cin, v);
                    v = trim(v);

                    if (v.empty())
                    {
                        cout << "Error: TEXT value cannot be empty.\n";
                        continue;
                    }

                    sheet[rowCount].values[i] = v;
                    break;
                }
            }
        }
    }

    rowCount++;
    cout << "Row inserted successfully.\n";
}

void displayCSV(AttendanceRow sheet[], int rowCount, Column columns[], int colCount)
{
    if (rowCount == 0)
        {
            cout << "-------------------------" << endl;
            cout << "Attendance sheet is empty" << endl;
            cout << "-------------------------" << endl;
            return;
        }


    cout << "=============================================\n\n";
    cout << "--------------------------------" << endl;
    cout << "VIew Attendance Sheet (CSV Mode)" << endl;
    cout << "--------------------------------" << endl;

    int width[MAX_COLUMNS];

    //Automatic determine width for each coloumn based on user input
    for (int j = 0; j < colCount; j++)
        width[j] = columns[j].name.length();

    for (int i = 0; i < rowCount; i++)
    {
        for (int j = 0; j < colCount; j++)
        {
            int len = sheet[i].values[j].length();
            if (len > width[j])
                width[j] = len;
        }
    }

    // Padding so user input tak dekat dengan comma and centred
    for (int j = 0; j < colCount; j++)
        width[j] += 1;

    // header print
    for (int j = 0; j < colCount; j++)
    {
        cout << left << setw(width[j]) << columns[j].name;
        if (j != colCount - 1) cout << ", ";
    }
    cout << endl;

    // row printing
    for (int i = 0; i < rowCount; i++)
    {
        for (int j = 0; j < colCount; j++)
        {
            cout << left << setw(width[j]) << sheet[i].values[j];
            if (j != colCount - 1) cout << ", ";
        }
        cout << endl;
    }

}


// MILESTONE 2 CODES //

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

void saveSheetToFile(string filename,
                     AttendanceRow sheet[],
                     int rowCount,
                     Column columns[],
                     int colCount)
{
    ofstream file(filename);

    // header
    for (int i = 0; i < colCount; i++)
    {
        file << columns[i].name;
        if (i != colCount - 1) file << ",";
    }
    file << "\n";

    // rows
    for (int r = 0; r < rowCount; r++)
    {
        for (int c = 0; c < colCount; c++)
        {
            file << sheet[r].values[c];
            if (c != colCount - 1) file << ",";
        }
        file << "\n";
    }

    file.close();
}

void loadSheetFromFile(string filename,
                       AttendanceRow sheet[],
                       int &rowCount,
                       Column columns[],
                       int &colCount)
{
    ifstream file(filename);
    string line;

    rowCount = 0;
    colCount = 0;

    // read header
    if (getline(file, line))
    {
        stringstream ss(line);
        string col;
        while (getline(ss, col, ',') && colCount < MAX_COLUMNS)
        {
            columns[colCount].name = trim(col);
            columns[colCount].type = "TEXT"; // flexible default
            colCount++;
        }
    }

    // read rows
    while (getline(file, line) && rowCount < MAX_ROWS)
    {
        stringstream ss(line);
        for (int c = 0; c < colCount; c++)
        {
            getline(ss, sheet[rowCount].values[c], ',');
            sheet[rowCount].values[c] = trim(sheet[rowCount].values[c]);
        }
        rowCount++;
    }

    file.close();
}

// Function prototypes (PART 2)

int findStudentIndex(int id);
int inputInt(const string& msg);
int inputStatus();
void updateAttendanceRow();
void deleteAttendanceRow();


// PART 2 FUNCTIONS 

void updateRowCSV(AttendanceRow sheet[], int rowCount, Column columns[], int colCount)
{
    int r, c;
    cout << "Enter row number to update (1-" << rowCount << "): ";
    cin >> r;

    if (r < 1 || r > rowCount) {
        cout << "Invalid row.\n";
        return;
    }

    cout << "Choose column:\n";
    for (int i = 0; i < colCount; i++)
        cout << i + 1 << ". " << columns[i].name << endl;

    cin >> c;

    if (c < 1 || c > colCount) {
        cout << "Invalid column.\n";
        return;
    }

    cin.ignore();
    cout << "Enter new value: ";
    getline(cin, sheet[r - 1].values[c - 1]);

    cout << "Row updated successfully.\n";
}

void deleteRowCSV(AttendanceRow sheet[], int &rowCount)
{
    int r;
    cout << "Enter row number to delete (1-" << rowCount << "): ";
    cin >> r;

    if (r < 1 || r > rowCount) {
        cout << "Invalid row.\n";
        return;
    }

    for (int i = r - 1; i < rowCount - 1; i++)
        sheet[i] = sheet[i + 1];

    rowCount--;

    // Member 4 - it displays the updated row
    cout << "Row deleted successfully.\n";

    cout << "--------------------------------\n";
    cout << "Count Rows\n";
    cout << "Number of rows: " << rowCount << "\n" <<endl;
}


void IntroductionProcess(string &sheetName,Column columns[], AttendanceRow attendanceSheet[], int &rowCount) // Introduction process so the program runs after user make their choice
{
    fstream dataFile;
    string filename;

    createSchoolTerm();

    cout << "Reading attendance data from file...\n";
    cout << "Enter attendance filename: ";
    cin >> filename;

    int colCount; loadSheetFromFile(sheetName, attendanceSheet, rowCount, columns, colCount);
    displayCSV(attendanceSheet, rowCount, columns, colCount);


    if (openAttendanceFile(dataFile, filename)) {
        cout << "File opened successfully.\n\n";
        dataFile.close();

    }

    else
    {
        cout << "File not found. Starting with empty attendance sheet.\n\n";
        recordCount = 0;
    }

    int choice;
    do {
        cout << "---------------------------------\n";
        cout << "1. Display Attendance\n";
        cout << "2. Update Attendance Row\n";
        cout << "3. Delete Attendance Row\n";
        cout << "4. Exit\n";
        cout << "Choose: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayCSV(attendanceSheet, rowCount, columns, colCount);
                break;

            case 2:
                updateRowCSV(attendanceSheet, rowCount, columns, colCount);
                saveAttendance(filename);
                break;

            case 3:
                deleteRowCSV(attendanceSheet, rowCount);
                saveAttendance(filename);
                break;

            case 4:
                saveSheetToFile(sheetName, attendanceSheet, rowCount, columns, colCount);
                cout << "Attendance data saved. Exiting...\n";
                return;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 0);
}

void introduction(string &sheetName, Column columns[], AttendanceRow attendanceSheet[], int &rowCount) // function that holds the entire program
{
    fstream dataFile;
    string filename;
    string option;

    cout << "\nWeclome to the 'Student Attendance Tracker'. Please select an option below. (1-5)" << endl;
    cout << "Option 1: Create Sheet" << endl;
    cout << "Option 2: Create School Term Database" << endl;
    cout << "Option 3: What is Student Attendance Tracker" << endl;
    cout << "Option 4: Quit" << endl;

    cin >> option;

    if (option == "1")
    {
        rowCount = 0;

        cin.ignore();
        string answer;

        // Member 1
        createSheet(sheetName);
        int colCount = createColumnsExact(columns);

        // NEW: user define how many rows to insert (max 10)
        int totalRows = getRowCountFromUser();
        cin.ignore(); // consume newline after cin >>

        // Member 2 (insert based on user input rows)
        for (int i = 0; i < totalRows; i++)
        {
            insertAttendanceRow(attendanceSheet, rowCount, columns, colCount);
        }

        // Member 3
        displayCSV(attendanceSheet, rowCount, columns, colCount);

        saveSheetToFile(sheetName, attendanceSheet, rowCount, columns, colCount);

        this_thread::sleep_for(chrono::seconds(2));

        while (true)
        {
            cout << "\nDo you want to return to the main menu?(Yes/No)" << endl;
            cin >> answer;


            for (char &c : answer) //FUNCTION TO LOWERCASE THE STRING
                c = tolower(static_cast<unsigned char>(c));


            if (answer == "yes")
            {
                introduction(sheetName, columns, attendanceSheet, rowCount);
                return;
            }

            else if (answer == "no")
            {
                cout << "Have a good day!" << endl;
                return;
            }
            else
            {
                cout << "Invalid Input. Try again." << endl;
            }
        }
    }

    else if (option == "2")
    {
        cout << "============================================\n" << endl;
        IntroductionProcess(sheetName, columns, attendanceSheet, rowCount);
    }

    else if (option == "3")
    {
        StudentTrackerDescription();
        this_thread::sleep_for(chrono::seconds(2));
        cout << "============================================\n" << endl;

        introduction(sheetName, columns, attendanceSheet, rowCount);
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

        introduction(sheetName, columns, attendanceSheet, rowCount);

    }

}


// PART 1 FUNCTIONS (MEMBER CODE)

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



//  PART 3 FUNCTIONS

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

    string sheetName;
    Column columns[MAX_COLUMNS];
    AttendanceRow attendanceSheet[MAX_ROWS];
    int rowCount = 0;


    introduction(sheetName, columns, attendanceSheet, rowCount);


    return 0;
}

