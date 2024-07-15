// personal_watchlist.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
using namespace std;

// Menu Array
string menu[7] = { "1. Add a movie", "2. Update a movie", "3. Delete a movie", "4. Search a movie", "5. Sort Movies", "6. History", "0. Exit" };
string HistoryMenu[4] = { "1. Search a movie", "2. Watchlist","3. Delete a movie", "0. Exit" };

// ===== Define Movie Structure =====
struct Movie {
    string title;
    string genre;
    string rating;
    string year;

    Movie* prev = NULL;
    Movie* next = NULL;
} *WLHead, * WLTail, * curr, * newNode, *before, *after
, *HHead, *HTail;

// ===== Create Linked List for Watchlist and History =====
void createWL(string title, string genre, string rating, string year) {
	WLHead = new Movie();
    WLHead->title = title;
    WLHead->genre = genre;
    WLHead->rating = rating;
    WLHead->year = year;
    WLTail = WLHead;
}
void createH(string title, string genre, string rating, string year) {
    HHead = new Movie();
    HHead->title = title;
    HHead->genre = genre;
    HHead->rating = rating;
    HHead->year = year;
    HTail = HHead;
}

// ===== Count number of nodes in Watchlist and History =====
int countWL(){
    curr = WLHead;
    int num = 0;
    while (curr != NULL) {
        num++;
        curr = curr->next;
    }
    return num;
}
int countH() {
    curr = HHead;
    int num = 0;
    while (curr != NULL) {
        num++;
        curr = curr->next;
    }
    return num;
}

// ===== Add a movie to the front of theHistory =====
void addFirstH(string title, string genre, string rating, string year) {
    newNode = new Movie();
    newNode->title = title;
    newNode->genre = genre;
    newNode->rating = rating;
    newNode->year = year;
    newNode->next = HHead;
    HHead->prev = newNode;
    HHead = newNode;
}

// ===== Add a movie to the end of the Watchlist=====
void addLastWL(string title, string genre, string rating, string year) {
	newNode = new Movie();
	newNode->title = title;
	newNode->genre = genre;
	newNode->rating = rating;
	newNode->year = year;
    newNode->prev = WLTail;
	WLTail->next = newNode;
	WLTail = newNode;
}

// ===== Sorting Watchlist =====
void sortWL(int i) {
    Movie* current = NULL, * index = NULL;
    string temp;
    //Check whether list is empty  
    if (WLHead == NULL) {
        return;
    }
    else {
        if(i == 1){// ===== sorting title
            //Current will point to head  
            for (current = WLHead; current->next != NULL; current = current->next) {
                //Index will point to node next to current  
                for (index = current->next; index != NULL; index = index->next) {
                    //If current's Title is greater than index's Title, swap the data of current and index  
                    if (current->title > index->title) {
                        temp = current->title;
                        current->title = index->title;
                        index->title = temp;
                    }
                }
            }
        }
        else if (i == 2) { // ===== sorting Genre
            //Current will point to head  
            for (current = WLHead; current->next != NULL; current = current->next) {
                //Index will point to node next to current  
                for (index = current->next; index != NULL; index = index->next) {
                    //If current's genre is greater than index's genre, swap the data of current and index  
                    if (current->genre > index->genre) {
                        temp = current->genre;
                        current->genre = index->genre;
                        index->genre = temp;
                    }
                }
            }
        }
        else if (i == 3) { // ===== sorting Year
            //Current will point to head  
            for (current = WLHead; current->next != NULL; current = current->next) {
                //Index will point to node next to current  
                for (index = current->next; index != NULL; index = index->next) {
                    //If current's year is greater than index's year, swap the data of current and index  
                    if (current->year > index->year) {
                        temp = current->year;
                        current->year = index->year;
                        index->year = temp;
                    }
                }
            }
        }
        else if (i == 4) { // ===== sorting rating
            //Current will point to head  
            for (current = WLHead; current->next != NULL; current = current->next) {
                //Index will point to node next to current  
                for (index = current->next; index != NULL; index = index->next) {
                    //If current's rating is greater than index's rating, swap the rating of current and index  
                    if (current->rating > index->rating) {
                        temp = current->rating;
                        current->rating = index->rating;
                        index->rating = temp;
                    }
                }
            }
        }
    }
}

// Global Variable 
int WLPage = 1, WLTotalPage = round(countWL()/8)+1, WLElement = 0, HPage = 1, HTotalPage = 1, HElement = 0;

// ===== Watchlist and History Menu Printing Function =====
void WLMenu() {
    WLTotalPage = round(countWL() / 8)+1;
    ::cout << "+" << setw(66) << setfill('-') << "+" << endl;
    ::cout << "|" << setfill(' ') << setw(41) << "Personal Watchlist" << setw(25) << "|" << endl;
    ::cout << "+" << setw(66) << setfill('-') << "+" << endl;
    ::cout << "| " << setw(40) << setfill(' ') << left << "Watchlist"; ::cout << " | "; ::cout << setw(20) << left << "Menu" << " |" << endl;
    ::cout << "+" << setw(66) << setfill('-') << right << "+" << endl;
    if (WLHead == NULL) {
        for (int i = 0; i < 8; i++) {
            if (i == 3) {
                ::cout << "| " << setw(27) << right << "Watchlist Empty" << setw(16) << " | " << setw(20) << left << menu[i] << " |" << endl;
                ::cout << "| " << setw(43) << right << " | " << setw(22) << right << "|" << endl;
            }
            else {
                ::cout << "| " << setw(43) << setfill(' ') << " | " << setw(20) << left << menu[i] << " |" << endl;
                ::cout << "| " << setw(43) << right << " | "<< setw(22) << right << "|" << endl;
            }
		}
	} else {
        curr = WLHead;
        if (WLPage > 1) {
            for (int i = 0; i < (WLPage-1) * 8; i++) {
                curr = curr->next;
            }
        }
        for (int i = 0; i < 8; i++){
            if(curr != NULL){
                ::cout << "| " << setw(40) << left << setfill(' ') << (curr->title+" (" + curr->year + ")" )<< " | " << setw(20) << left << menu[i] << " |" << endl;
                ::cout << "| " << setw(40) << left << setfill(' ')<<("   " + curr->genre + " - " + curr->rating) << " | " << setw(22) << right << "|" << endl;
                curr = curr->next;
            }
            else {
                ::cout << "| " << setw(43) << setfill(' ') << " | " << setw(20) << left << menu[i] << " |" << endl;
                ::cout << "| " << setw(43) << right << " | " << setw(22) << right << "|" << endl;
            }
        }
	}
    ::cout << "+" << setw(66) << setfill('-') << "+" << endl;
    ::cout << ("|  Page "+ to_string(WLPage)+"/"+ to_string(WLTotalPage)) << setw(57) << setfill(' ') << "Previous(<)   Watch(^)   Next(>) | " << endl;
    ::cout << "+" << setw(66) << setfill('-') << "+" << endl;
}
void HMenu() {
    HTotalPage = round(countH() / 8) + 1;
    ::cout << "+" << setw(66) << setfill('-') << "+" << endl;
    ::cout << "|" << setfill(' ') << setw(41) << "Personal Watchlist" << setw(25) << "|" << endl;
    ::cout << "+" << setw(66) << setfill('-') << "+" << endl;
    ::cout << "| " << setw(40) << setfill(' ') << left << "History"; ::cout << " | "; ::cout << setw(20) << left << "Menu" << " |" << endl;
    ::cout << "+" << setw(66) << setfill('-') << right << "+" << endl;
    if (HHead == NULL) {
        for (int i = 0; i < 8; i++) {
            if (i == 3) {
                ::cout << "| " << setw(27) << right << "History Empty" << setw(16) << " | " << setw(20) << left << HistoryMenu[i] << " |" << endl;
                ::cout << "| " << setw(43) << right << " | " << setw(22) << right << "|" << endl;
            }
            else if(i<4){
                ::cout << "| " << setw(43) << setfill(' ') << " | " << setw(20) << left << HistoryMenu[i] << " |" << endl;
                ::cout << "| " << setw(43) << right << " | " << setw(22) << right << "|" << endl;
            }
            else {
                ::cout << "| " << setw(43) << right << " | " << setw(22) << right << "|" << endl;
                ::cout << "| " << setw(43) << right << " | " << setw(22) << right << "|" << endl;
            }
        }
    }
    else {
        curr = HHead;
        if (HPage > 1) {
            for (int i = 0; i < (HPage - 1) * 8; i++) {
                curr = curr->next;
            }
        }
        for (int i = 0; i < 8; i++) {
            if (curr != NULL) {
                ::cout << "| " << setw(40) << left << setfill(' ') << (curr->title + " (" + curr->year + ")") << " | " << setw(20) << left << HistoryMenu[i] << " |" << endl;
                ::cout << "| " << setw(40) << left << setfill(' ') << ("   " + curr->genre + " - " + curr->rating) << " | " << setw(22) << right << "|" << endl;
                curr = curr->next;
            }
            else if(i<4) {
                ::cout << "| " << setw(43) << setfill(' ') << " | " << setw(20) << left << HistoryMenu[i] << " |" << endl;
                ::cout << "| " << setw(43) << right << " | " << setw(22) << right << "|" << endl;
            } 
            else {
                ::cout << "| " << setw(43) << right << " | " << setw(22) << right << "|" << endl;
                ::cout << "| " << setw(43) << right << " | " << setw(22) << right << "|" << endl;
            }
        }
    }
    ::cout << "+" << setw(66) << setfill('-') << "+" << endl;
    ::cout << ("|  Page " + to_string(HPage) + "/" + to_string(HTotalPage)) << setw(57) << setfill(' ') << "Previous(<)   Watch(^)   Next(>) | " << endl;
    ::cout << "+" << setw(66) << setfill('-') << "+" << endl;
}

// ====================  M A I N   F U N C T I O N  ====================
int main()
{
    string title, year, genre, rating;
    char sortMenu;
    int MenuChoose = 0;
    menuChoose:
    if (MenuChoose == 0){// =============== W A T C H L I S T ===============
        WLMenu();
        char Choose;
        
        cout << "INPUT      : "; cin >> Choose;
        switch (Choose) {
        case '1':// ===== add movie
            cout << "Title      : "; 
            cin.ignore();
            getline(cin, title);
            cout << "Year       : "; cin >> year;
            cout << "Genre      : "; cin >> genre;
            cout << "Rating     : "; cin >> rating;
            if (WLHead==NULL) {
                createWL(title, genre, rating, year);
            }
            else {
                addLastWL(title, genre, rating, year);
            }
            goto menuChoose;
            break;
        case '2':// ===== update movie
            if (WLHead == NULL){
                cout << "Watchlist is Empty!" << endl;
                goto menuChoose;
            }
            cout << "Title      : "; 
            cin.ignore();
            getline(cin, title);
            curr = WLHead;
            while (curr != NULL) {
                if (curr->title == title) {
                    cout << "Title      : "; 
                    cin.ignore();
                    getline(cin, title);
                    cout << "Year       : "; cin >> year;
                    cout << "Genre      : "; cin >> genre;
                    cout << "Rating     : "; cin >> rating;
                    curr->title = title; curr->year = year; curr->genre = genre; curr->rating = rating;
                    cout << "Movie Updated Successfully!" << endl;
                    goto menuChoose;
                }
                curr = curr->next;
            }
            cout << "Movie not Found!" << endl;
            goto menuChoose;
            break;
        case '3':// ===== delete a movie
            if (WLHead == NULL) {
                cout << "Watchlist is Empty!" << endl;
                goto menuChoose;
            }
            cout << "Title      : "; 
            cin.ignore();
            getline(std::cin, title);
            if (WLHead->title == title) {
                curr = WLHead;
                WLHead = WLHead->next;
                delete curr;
                cout << "Movie Deleted!" << endl;
                goto menuChoose;
            }
            if (WLTail->title == title) {
                before = WLTail->prev;
                before->next = NULL;
                delete WLTail;
                WLTail = before;
                cout << "Movie Deleted!" << endl;
                goto menuChoose;
            }
            curr = WLHead;
            while (curr != NULL) {
                if (curr->title == title) {
                    before = curr->prev;
                    after = curr->next;
                    before->next = after;
                    after->prev = before;
                    delete curr;
                    cout << "Movie Deleted!" << endl;
                    goto menuChoose;
                }
                curr = curr->next;
            }
            cout << "Movie not Found!" << endl;
            goto menuChoose;
            break;
        case '4':// ===== search a movie
            if (WLHead == NULL) {
                cout << "Watchlist is Empty!" << endl;
                goto menuChoose;
            }
            cout << "Title      : "; 
            cin.ignore();
            getline(cin, title);
            curr = WLHead; WLElement = 0;
            while (curr != NULL) {
                if (curr->title == title) {
                    WLPage = static_cast<int>(round(WLElement / 8)+1);
                    cout << "Movie Found!" << endl;
                    goto menuChoose;
                }
                curr = curr->next;
                WLElement++;
            }
            cout << "Movie not Found!" << endl;
            goto menuChoose;
            break;
        case '5':// ===== Sort Movies
            cout << "Sort Movie By :" << endl;
            cout << "[T]title|[G]genre|[Y]year|[R]rating" << endl;
            cout << "Choose     : "; cin >> sortMenu;
            if (sortMenu == 'T' || sortMenu == 't') {
                sortWL(1);
                cout << "Watchlist has been Sorted!" << endl;
                goto menuChoose;
            }
            else if (sortMenu == 'G' || sortMenu == 'g') {
                sortWL(2);
                cout << "Watchlist has been Sorted!" << endl;
                goto menuChoose;
            }
            else if (sortMenu == 'Y' || sortMenu == 'y') {
                sortWL(3);
                cout << "Watchlist has been Sorted!" << endl;
                goto menuChoose;
            }
            else if (sortMenu == 'R' || sortMenu == 'r') {
                sortWL(4);
                cout << "Watchlist has been Sorted!" << endl;
                goto menuChoose;
            }
            else {
                cout << "Command not Found!" << endl;
                goto menuChoose;
            }
            break;
        case '6': // ==== goto history
            MenuChoose = 1;
            goto menuChoose;
            break;
        case '0':
            return 0;
            break;
        case '<': // ===== Previous Page
            if (WLPage <= 1)
                goto menuChoose;
            WLPage--;
            goto menuChoose;
            break;
        case '^':// ===== watch movie
            if (WLHead == NULL) {
                cout << "Watchlist is Empty!" << endl;
                goto menuChoose;
            }
            cout << "Title      : ";
            cin.ignore();
            getline(cin, title);
            curr = WLHead;
            while (curr != NULL) {
                if (curr->title == title) {
                    if (curr == WLHead) {
                        if (HHead == NULL) {
                            // create HHead
                            createH(curr->title, curr->genre, curr->rating, curr->year);
                            WLHead = WLHead->next;
                            WLHead->prev == NULL;
                            cout << "Movie Watched and Moved to History!" << endl;
                            goto menuChoose;
                        }
                        addFirstH(curr->title, curr->genre, curr->rating, curr->year);
                        WLHead = WLHead->next;
                        WLHead->prev == NULL;
                        cout << "Movie Watched and Moved to History!" << endl;
                        goto menuChoose;
                    } else if (curr == WLTail) {
                        if (HHead == NULL) {
                            // create HHead
                            createH(curr->title, curr->genre, curr->rating, curr->year);
                            WLTail = WLTail->prev;
                            WLTail->next = NULL;
                            cout << "Movie Watched and Moved to History!" << endl;
                            goto menuChoose;
                        }
                        addFirstH(curr->title, curr->genre, curr->rating, curr->year);
                        WLTail = WLTail->prev;
                        WLTail->next = NULL;
                        cout << "Movie Watched and Moved to History!" << endl;
                        goto menuChoose;
                    }
                    if (HHead == NULL) {
                        createH(curr->title, curr->genre, curr->rating, curr->year);
                        before = curr->prev;
                        after = curr->next;
                        before->next = after;
                        after->prev = before;
                        cout << "Movie Watched and Moved to History!" << endl;
                        goto menuChoose;
                    }
                    addFirstH(curr->title, curr->genre, curr->rating, curr->year);
                    before = curr->prev;
                    after = curr->next;
                    before->next = after;
                    after->prev = before;
                    cout << "Movie Watched and Moved to History!" << endl;
                    goto menuChoose;
                }
                curr = curr->next;
            }
            cout << "Movie not Found!" << endl;
            goto menuChoose;
            break;
        case '>': // ===== next page
            if (WLPage >= WLTotalPage)
                goto menuChoose;
            WLPage++;
            goto menuChoose;
            break;
        case '9': // ========== HIDDEN FUNCTION ========================+
            createWL("The Dark Knight", "Action", "9.0", "2008");//     |
            addLastWL("Jumanji", "Adventure", "8.5", "2019");//         |
            addLastWL("Titanic", "Romance", "9.5", "1976");//           |
            addLastWL("Weathering with You", "Drama", "8.4", "2019");// |
            addLastWL("MEG", "Thriller", "7.6", "2018");//              |
            addLastWL("The Witcher", "Fantasy", "9.6", "2021");//       |
            addLastWL("Barbie", "Kids", "8.2", "2006");//               |
            addLastWL("John Wick", "Thriller","8.8","2023");//          |
            addLastWL("Fury", "Historical", "9.1","2015");//            |
            addLastWL("Mouse", "Psychological", "7.2", "2022");//       |
            addLastWL("Incantation", "Horror", "7.6", "2012");//        |
            addLastWL("Squid Game", "Mystery", "6.9", "2020");//        |
            addLastWL("Ready Player One", "Sci-Fi", "7.1", "2017");//   |
            //==========================================================+
            goto menuChoose;
            break;
        default:
            goto menuChoose;
            break;
        }
    }else if (MenuChoose == 1) { // =================== H I S T O R Y ===================
        HMenu();
        char Choose;
        cout << "INPUT      : "; cin >> Choose;
        switch (Choose) {
        case '1': // ===== search a movie =====
            if (HHead == NULL) {
                cout << "History is Empty!" << endl;
                goto menuChoose;
            }
            cout << "Title      : ";
            cin.ignore();
            getline(cin, title);
            curr = HHead; HElement = 0;
            while (curr != NULL) {
                if (curr->title == title) {
                    HPage = static_cast<int>(round(HElement / 8) + 1);
                    cout << "Movie Found!" << endl;
                    goto menuChoose;
                }
                curr = curr->next;
                HElement++;
            }
            cout << "Movie not Found!" << endl;
            goto menuChoose;
            break;
        case '2': // ===== goto Watchlist =====
            MenuChoose = 0;
            goto menuChoose;
            break;
        case '3': // ===== Delete a movie from history =====
            if (HHead == NULL) {
                cout << "Watchlist is Empty!" << endl;
                goto menuChoose;
            }
            cout << "Title      : ";
            cin.ignore();
            getline(std::cin, title);
            if (HHead->title == title) {
                curr = HHead;
                HHead = HHead->next;
                delete curr;
                cout << "Movie Deleted from History!" << endl;
                goto menuChoose;
            }
            if (HTail->title == title) {
                before = HTail->prev;
                before->next = NULL;
                delete HTail;
                HTail = before;
                cout << "Movie Deleted from History!" << endl;
                goto menuChoose;
            }
            curr = HHead;
            while (curr != NULL) {
                if (curr->title == title) {
                    before = curr->prev;
                    after = curr->next;
                    before->next = after;
                    after->prev = before;
                    delete curr;
                    cout << "Movie Deleted from History!" << endl;
                    goto menuChoose;
                }
                curr = curr->next;
            }
            cout << "Movie not Found!" << endl;
            goto menuChoose;
            break;
        case '0':
            return 0;
            break;
        case '<':
            if (HPage <= 1)
                goto menuChoose;
            HPage--;
            goto menuChoose;
            break;
        case '^':
            if (HHead == NULL) {
                cout << "History is Empty!" << endl;
                goto menuChoose;
            }
            cout << "Title      : ";
            cin.ignore();
            getline(cin, title);
            curr = HHead;
            while (curr != NULL) {
                if (curr->title == title) {
                    if (curr->title == HHead->title) {
                        cout << "Movie Watched and History Updated!" << endl;
                        goto menuChoose;
                    }
                    if (curr->title == HTail->title) {
                        curr = HTail;
                        curr->next = HHead;
                        HHead->prev = curr;
                        HHead = curr;
                        HTail = HTail->prev;
                        HTail->next = NULL;
                        cout << "Movie Watched and History Updated!" << endl;
                        goto menuChoose;
                    }
                    curr->prev->next = curr->next;
                    curr->next->prev = curr->prev;
                    // adding curr to history
                    curr->next = HHead;
                    curr->prev = NULL;
                    HHead->prev = curr;
                    HHead = curr;
                    cout << "Movie Watched and History Updated!" << endl;
                    goto menuChoose;
                }
                curr = curr->next;
            }
            cout << "Movie not Found in History!" << endl;
            goto menuChoose;
            break;
        case '>':
            if (HPage >= HTotalPage)
                goto menuChoose;
            HPage++;
            goto menuChoose;
            break;
        default:
            goto menuChoose;
            break;
        }
    }
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
