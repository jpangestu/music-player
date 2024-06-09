#include <iostream>
#include <vector>

using namespace std;

vector<string> mainMenu({" Choose one option below by typing the number (0,1,2,.. )",
"1. Create", "2. Read", "3. Update", "4. Delete"});


void printUI(string message, int maxLength) {
    // String concatenation
    message = "| " + message;
    size_t length = maxLength - message.length();
    for (size_t i = 1; i < length; i++) {
        message += " ";
        if (i == length - 1) {
            message += "|\n";
        }
    }
    cout << message;
}

// string printUI(string message, string variable, int maxLength) {
//     // String concatenation
//     string s = message + variable;
//     size_t length = maxLength - (variable.length() + message.length());
//     for (size_t i = 1; i < length; i++) {
//         s += " ";
//         if (i == length - 1) {
//             s += "|\n";
//         }
//     }
//     return s;
// }

void printUI(vector<string> list, int maxLength) {
    cout << list.size() << endl;
    
    // Print upper border
    for (int i = 0; i < maxLength; i++) {
        if (i == 0 || i == maxLength - 1) {
            cout << "+";
        } else {
            cout << "-";
        }
    }
    cout << endl;

    // Print menu
    for (int i = 0; i < list.size(); i++) {
        printUI(list[i], 100);
    }

    // Print lower border
    for (int i = 0; i < maxLength; i++) {
        if (i == 0 || i == maxLength - 1) {
            cout << "|";
        } else {
            cout << " ";
        }
    }
    cout << endl;

    string lowerStrip = "";
    for (int i = 0; i < maxLength; i++) {
        if (i == 0 || i == maxLength - 1) {
            cout << "|";
            lowerStrip += "+";
        } else if (i == maxLength - 1 - 10) {
            cout << "| 0. Exit ";
            i += 9;
            lowerStrip += "+";
            for (int i = 0; i < 9; i++) {
                lowerStrip += "-";
            }
        } else {
            cout << " ";
            lowerStrip += "-";
        }
    }
    cout << endl << lowerStrip << endl;
    cout << "| Input: ";
}

int main() {
    printUI(mainMenu, 100);
}