#include <fstream>
#include <iostream>
#include <iomanip>
using namespace std;



    int validate_student_id (string prompt)
    {
        int id;
        while (true) 
        {
            cout << prompt;
            if (cin >> id)
            {
                cin.ignore();
                return id;
            }
            else
            {
                cout << "ERROR: Invalid integer. Try again." << endl;
                cin.clear();
                cin.ignore();
            }
            }
    }

    int validate_status(string prompt)
    {
        int status;
        while (true)
        {
            status = validate_student_id(prompt);
            if (status == 0 || status == 1) return status;
            cout << "ERROR: Status must be 0 or 1" << endl;
        }
    }
void report_missing_id (int id)
{
    cout << "ERROR: Student ID does not exist." << endl;
}




int main()
{
    cout << "=============================================\n";
    cout << "STUDENT ATTENDANCE TRACKER - MILESTONE 1\n";
    cout << "=============================================\n\n";

    cout << "---Advanced Error Handling Demo---" << endl;

    int idInput = validate_student_id("Enter StudentID to update: ");
    int idStatus = validate_status("Enter status: ");

}
