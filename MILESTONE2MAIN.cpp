#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <thread>   // for time delay
#include <chrono>   // for time delay
#include <iomanip>  // csv mode alignments
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

// =====================
// Global constants
// =====================
const int MAX_COLUMNS = 10;
const int MAX_ROWS = 100;
const int MAX_INPUT_ROWS = 10;

// ===============================
// Member 1 (ARFA): Structures
// ===============================
struct Column
{
    string name;
    string type; // INT or TEXT
};

// ===============================
// Member 2 (EMIL): Structures
// ===============================
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

// ===============================
// Member 4 (LUQMAN): Globals
// ===============================
string termName;

struct Student {
    int id;
    string name;
    int status;
};

Student attendance[100];
int recordCount = 0;

// ===============================
// Function prototypes (Milestone 1)
// ===============================
void createSheet(string &sheetName);
int createColumnsExact(Column columns[]);
int getRowCountFromUser();
void insertAttendanceRow(AttendanceRow sheet[], int &rowCount, Column columns[], int colCount);
void displayCSV(AttendanceRow sheet[], int rowCount, Column columns[], int colCount);

// ===============================
// Member 3 (HANNAN): Helper utils
// ===============================

// Convert string to lowercase (for comparisons)
static string toLowerStr(string s)
{
    for (char &c : s)
        c = tolower(static_cast<unsigned char>(c));
    return s;
}

// Check if a string is a valid integer (digits only)
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

// Trim whitespace from both ends
static string trim(const string &s)
{
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// Parse format like: "Name (INT)" or "Name (TEXT)"
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

// ==========================================
// Member 1 (ARFA): Create attendance sheet
// ==========================================

// Create sheet name with basic validation
void createSheet(string &sheetName)
{
    cout << "=============================================\n";
    cout << "Enter attendance sheet name: ";
    getline(cin, sheetName);

    if (sheetName.empty())
    {
        cout << "Error: Sheet name cannot be empty.\n";
        createSheet(sheetName);
    }
    else
    {
        cout << "Attendance sheet \"" << sheetName << "\" created successfully.\n";
    }
}

// Create columns with (INT)/(TEXT) validation (max 10)
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
            cout << "\nEnter column " << (i + 1)
                 << " name (Ensure to put (INT) or (TEXT) at the end of the name for column types): ";
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

// ==========================================
// Member 4 (LUQMAN): Row count input
// ==========================================

// Let user choose how many rows to insert (max 10)
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

// ==========================================
// Member 2 (EMIL): Insert attendance row
// ==========================================

// Insert row with type checking and status validation
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

        // Special rule: status must be 0 or 1 (still works even if user names it "STATUS")
        if (colNameLower == "status")
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
            continue;
        }

        // Generic INT/TEXT validation for all other columns
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
        else // TEXT
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

    rowCount++;
    cout << "Row inserted successfully.\n";
}

// ==========================================
// Member 3 (HANNAN): Display CSV
// ==========================================

// Display sheet in CSV mode with auto spacing
void displayCSV(AttendanceRow sheet[], int rowCount, Column columns[], int colCount)
{
    if (rowCount == 0)
    {
        cout << "-------------------------\n";
        cout << "Attendance sheet is empty\n";
        cout << "-------------------------\n";
        return;
    }

    cout << "=============================================\n\n";
    cout << "--------------------------------\n";
    cout << "VIew Attendance Sheet (CSV Mode)\n";
    cout << "--------------------------------\n";

    int width[MAX_COLUMNS];

    for (int j = 0; j < colCount; j++)
        width[j] = (int)columns[j].name.length();

    for (int i = 0; i < rowCount; i++)
    {
        for (int j = 0; j < colCount; j++)
        {
            int len = (int)sheet[i].values[j].length();
            if (len > width[j])
                width[j] = len;
        }
    }

    for (int j = 0; j < colCount; j++)
        width[j] += 1;

    // header
    for (int j = 0; j < colCount; j++)
    {
        cout << left << setw(width[j]) << columns[j].name;
        if (j != colCount - 1) cout << ", ";
    }
    cout << endl;

    // rows
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

// =======================================================
// MILESTONE 2: File Handling (PART 1 - MEMBER)
// =======================================================
bool openAttendanceFile(fstream &file, string filename);
void loadAttendance(fstream &file);
void displayAttendance();
void saveAttendance(string filename);

// ==========================================
// Member 4 (LUQMAN): Description + Term
// ==========================================

void StudentTrackerDescription()
{
    cout << "Student Attendance Tracker is a C++ program that was built for tracking attendance." << endl;
    cout << "It makes tracking attendance more efficient by storing student datas." << endl;
}

void createSchoolTerm()
{
    cout << "Create School Term (Database)\n";
    cout << "---------------------------------\n";
    cout << "Enter term name: ";
    cin >> termName;
    cout << "Database \"" << termName << "\" created and loaded.\n\n";
}

// ==========================================
// Member 1 (ARFA): Save/Load sheet to CSV
// ==========================================

// Save sheet to CSV and include type in header: "ColName (INT)"
void saveSheetToFile(string filename,
                     AttendanceRow sheet[],
                     int rowCount,
                     Column columns[],
                     int colCount)
{
    ofstream file(filename);

    // header (store name + type)
    for (int i = 0; i < colCount; i++)
    {
        file << columns[i].name << " (" << columns[i].type << ")";
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

// Load sheet and restore types from header when possible
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
            col = trim(col);

            string nameParsed, typeParsed;

            // If header has "(INT)/(TEXT)", we restore it
            if (parseColumnDefinition(col, nameParsed, typeParsed))
            {
                columns[colCount].name = nameParsed;
                columns[colCount].type = typeParsed;
            }
            else
            {
                // For old CSV without types, store name and guess type for common columns
                columns[colCount].name = col;
                string lower = toLowerStr(col);

                if (lower == "studentid" || lower == "status")
                    columns[colCount].type = "INT";
                else
                    columns[colCount].type = "TEXT";
            }

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

// ==========================================
// Member 2 (EMIL): Update & Delete (generic type enforcement)
// ==========================================

// Update any cell based on column type (INT/TEXT), regardless of column name
void updateRowCSV(AttendanceRow sheet[], int rowCount, Column columns[], int colCount)
{
    if (rowCount <= 0)
    {
        cout << "-------------------------\n";
        cout << "Attendance sheet is empty\n";
        cout << "-------------------------\n";
        return;
    }

    string rStr, cStr;
    int r = 0, c = 0;

    // row number validation
    while (true)
    {
        cout << "Enter row number to update (1-" << rowCount << "): ";
        cin >> rStr;

        if (!isValidIntString(rStr))
        {
            cout << "Error: Please enter a valid number.\n";
            continue;
        }

        r = stoi(rStr);
        if (r < 1 || r > rowCount)
        {
            cout << "Error: Invalid row.\n";
            continue;
        }
        break;
    }

    // choose column
    cout << "Choose column:\n";
    for (int i = 0; i < colCount; i++)
        cout << i + 1 << ". " << columns[i].name << " (" << columns[i].type << ")\n";

    while (true)
    {
        cout << "Enter column number (1-" << colCount << "): ";
        cin >> cStr;

        if (!isValidIntString(cStr))
        {
            cout << "Error: Please enter a valid number.\n";
            continue;
        }

        c = stoi(cStr);
        if (c < 1 || c > colCount)
        {
            cout << "Error: Invalid column.\n";
            continue;
        }
        break;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // new value validation based on column type
    string newVal;
    string colType = columns[c - 1].type;

    while (true)
    {
        cout << "Enter new value: ";
        getline(cin, newVal);
        newVal = trim(newVal);

        if (colType == "INT")
        {
            if (!isValidIntString(newVal))
            {
                cout << "Error: This column only accepts INTEGER values.\n";
                continue;
            }
            break;
        }
        else if (colType == "TEXT")
        {
            if (newVal.empty())
            {
                cout << "Error: TEXT value cannot be empty.\n";
                continue;
            }
            break;
        }
        else
        {
            if (newVal.empty())
            {
                cout << "Error: Value cannot be empty.\n";
                continue;
            }
            break;
        }
    }

    sheet[r - 1].values[c - 1] = newVal;
    cout << "Row updated successfully.\n";
}

// Delete row with basic range check
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

    cout << "Row deleted successfully.\n";
    cout << "--------------------------------\n";
    cout << "Count Rows\n";
    cout << "Number of rows: " << rowCount << "\n" << endl;
}

// ==========================================
// Member 4 (LUQMAN): Program flow (menus)
// ==========================================

void IntroductionProcess(string &sheetName, Column columns[], AttendanceRow attendanceSheet[], int &rowCount)
{
    fstream dataFile;
    string filename;

    createSchoolTerm();

    cout << "Reading attendance data from file...\n";
    cout << "Enter attendance filename: ";
    cin >> filename;

    // Load using the filename user typed (so data will persist after restart)
    int colCount;
    loadSheetFromFile(filename, attendanceSheet, rowCount, columns, colCount);
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
                saveSheetToFile(filename, attendanceSheet, rowCount, columns, colCount);
                break;

            case 3:
                deleteRowCSV(attendanceSheet, rowCount);
                saveSheetToFile(filename, attendanceSheet, rowCount, columns, colCount);
                break;

            case 4:
                saveSheetToFile(filename, attendanceSheet, rowCount, columns, colCount);
                cout << "Attendance data saved. Exiting...\n";
                return;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 0);
}

void introduction(string &sheetName, Column columns[], AttendanceRow attendanceSheet[], int &rowCount)
{
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

        // Member 1: create sheet + columns
        createSheet(sheetName);
        int colCount = createColumnsExact(columns);

        // Member 4: choose number of rows to insert
        int totalRows = getRowCountFromUser();
        cin.ignore();

        // Member 2: insert rows
        for (int i = 0; i < totalRows; i++)
        {
            insertAttendanceRow(attendanceSheet, rowCount, columns, colCount);
        }

        // Member 3: display CSV
        displayCSV(attendanceSheet, rowCount, columns, colCount);

        // Save using sheetName (kept same as your original behaviour)
        // Note: this creates a file named exactly like sheetName (including spaces if any)
        saveSheetToFile(sheetName, attendanceSheet, rowCount, columns, colCount);

        this_thread::sleep_for(chrono::seconds(2));

        while (true)
        {
            cout << "\nDo you want to return to the main menu?(Yes/No)" << endl;
            cin >> answer;

            for (char &c : answer)
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

// ==========================================
// PART 1 (MEMBER): attendance[] file functions
// ==========================================

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

// ==========================================
// PART 3 (MEMBER): helper functions kept
// ==========================================

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

// ==========================================
// Member 4 (LUQMAN): main()
// ==========================================
int main()
{
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
