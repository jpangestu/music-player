#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <cmath> // floor()
#include <conio.h> // getch()
#include <windows.h>

// For reading .mp3 files tag (https://github.com/Arcxm/mp3_id3_tags)
#define MP3_ID3_TAGS_IMPLEMENTATION
#include "../lib/mp3_id3_tags/mp3_id3_tags.h"

using namespace std;

struct Music {
    string title;
    string artist;
    string dir;
    Music* next;
    Music* prev;

    Music(string Title, string Artist, string Directory) {
        title = Title;
        artist = Artist;
        dir = Directory;
    }
};

enum MenuType {
    NowPlaying, Library, Playlist
};

enum PageAlignment {
    Left, Middle
};

// Maximum number of character for the interface (UI)
int maxLength = 86;

// Find all .mp3 files in a directory (and all of it's subdirectory) and store it in vector
vector<string> findMp3(string directory) {
    vector<string> allEntry;
    vector<string> allMp3;
    for (const auto& dirEntry : filesystem::recursive_directory_iterator(directory)) {
        allEntry.insert(allEntry.end(), dirEntry.path().string());
    }

    int index = 0;
    for (vector<string>::iterator i = allEntry.begin(); i < allEntry.end(); i++) {
            filesystem::path tmp = allEntry[index];
            error_code error_code;
            // Store all .mp3 to allMp3
            if (tmp.extension() == ".mp3") {
                allMp3.insert(allMp3.end(), allEntry[index]);
            }

            index++; // Index = vector iterator
        }
    return allMp3;
}

// Helper for the main printUI function
string printUI(string message, int maxLength, PageAlignment pageAlignment) {
    if (pageAlignment == Left) {
        message = "| " + message;
        size_t length = maxLength - message.length();
        for (size_t i = 1; i < length; i++) {
            message += " ";
            if (i == length - 1) {
                message += "|";
            }
        }
        return message;
    } else {
        string str;
        size_t length = floor(maxLength/2 - message.length()/2);
        for (size_t i = 1; i < length; i++) {
            if (i == 1) {
                str += "|";
            } else {
                str += " ";
            }
        }

        str += message;

        length = maxLength - str.length();
        for (size_t i = 1; i < length; i++) {
            str += " ";
            if (i == length - 1) {
                str += "|";
            }
        }
        return str;
    }   
}

// Print out Menu (Interface/UI) to console
void printUI(vector<string> list, int maxLength, MenuType menuType) {
    string upperStrip, blankLine, lowerStrip, lowerOption;
    upperStrip = blankLine = lowerStrip = lowerOption = "";

    for (int i = 0; i < maxLength; i++) {
        if (i == 0 || i == maxLength - 1) {
            upperStrip += "+";
            blankLine += "|";
        } else {
            upperStrip += "-";
            blankLine += " ";
        }

    }
    
    for (int i = 0; i < maxLength; i++) {
        if (i == 0 || i == maxLength - 1) {
            lowerOption += "|";
            lowerStrip += "+";
            continue;
        }
        
        if (i == maxLength - 11) {
            lowerOption += "| 0. Exit ";
            i += 9; // 9 is the number of character of "| 0. Exit " minus 1
            lowerStrip += "+";
            for (int j = 0; j < 9; j++) {
                lowerStrip += "-";
            }
            continue;
        }

        // Print lower border option based on position (type of menu)
        if (menuType == NowPlaying) {
            if (i == 2) {
                lowerOption += "8. Playlist | 9. Library |";
                i += 26;

                for (int j = 0; j < 26; j++) {
                    if (j == 12 || j == 25) {
                        lowerStrip+= "+";
                    } else {
                        lowerStrip += "-";
                    }
                }
            }
        } else if (menuType == Library) {
            if (i == 2) {
                lowerOption += "8. Now Playing | 9. Playlist |";
                i += 30;
                
                for (int j = 0; j < 30; j++) {
                    if (j == 15 || j == 29) {
                        lowerStrip+= "+";
                    } else {
                        lowerStrip += "-";
                    }
                }
            }
        } else {
            if (i == 2) {
                lowerOption += "8. Library | 9. Now Playing |";
                i += 29;
                
                for (int j = 0; j < 29; j++) {
                    if (j == 11 || j == 28) {
                        lowerStrip+= "+";
                    } else {
                        lowerStrip += "-";
                    }
                }
            }
        }

        lowerOption += " ";
        lowerStrip += "-";
    }

    cout << upperStrip << endl;

    // Print menu (the content)
    if (menuType == NowPlaying) {
        for (int i = 0; i < list.size(); i++) {
            cout << "|" << list[i] << "|" << endl;
        }
    } else {
        for (int i = 0; i < list.size(); i++) {
            if ( i == 0) {
            cout << printUI(list[i], maxLength, Middle) << endl;
            } else {
                cout << printUI(list[i], maxLength, Left) << endl;
            }
        }
        cout << blankLine << endl;
    }
    cout << lowerStrip << endl << lowerOption << endl << lowerStrip << endl;
    cout << "| Input: ";
}

string printNowPlaying(string message, int maxLength, PageAlignment pageAlignment) {
    message;
    size_t length = maxLength - message.length();
    if (pageAlignment == Left) {
        size_t length = maxLength - message.length();
        for (size_t i = 0; i < length; i++) {
            message += " ";
        }
        return message;
    } else {
        string str;
        size_t length = floor(maxLength/2 - message.length()/2);
        for (size_t i = 0; i < length - 1; i++) {
            str += " ";
        }

        str += message;

        length = maxLength - str.length();
        for (size_t i = 0; i < length; i++) {
            str += " ";
        }
        return str;
    }  
}
// Create Now Playing Menu list (the contents)
vector<string> createNowPlayingList(string currentlyPlayedMusic, int maxLength, vector<string> queueList) {
    maxLength -= 2; // For counting left & right border
    int npMaxLength = maxLength * 60/100;
    int queMaxLength = maxLength - npMaxLength;
    for (int i = 0; i < queueList.size(); i++) {
        queueList[i] = " " + queueList[i];
    }
    vector<string> nowPlayingMenu;
    string str;

    for (int i = 1; i <= 9; i++) {
        str = "";

        // Now Playing section only
        for (int j = 1; j <= npMaxLength; j++) { // j starts at 1
            if (i == 1 || i == 9) {
                if (j == 1 || j == npMaxLength) {
                    str += "+";
                } else {
                    str += "-";
                }
            } else if (i == 4 || i == 6) {
                if (j == 1 || j == npMaxLength) {
                    str += "|";
                } else {
                    str += " ";
                }
            } else if (i == 7) {
                if (j == 1 || j == npMaxLength) {
                    str += "|";
                } else if (j == 4) {
                    str += "< Prev.";
                    j += 6;
                } else  if (j == npMaxLength - 2 - 6) { // 2 is for counting the space
                    str += "Next >";
                    j += 5;
                } else if (j == npMaxLength/2 - 5) {
                    str += "? Play/Pause";
                    j += 11;
                } else {
                    str += " ";
                }
            }  else if (i == 8) {
                if (j == 1 || j == npMaxLength) {
                    str += "|";
                } else if (j == 4) {
                    str += "6. Shuffle";
                    j += 9;
                } else  if (j == npMaxLength - 2 - 7) {
                    str += "7. Sort";
                    j += 6;
                } else {
                    str += " ";
                }
            }
        }

        if (i == 1) {
            str += printNowPlaying("Queue", queMaxLength, Middle);
        } else if (i == 2) {
            str = printUI("Now Playing", npMaxLength, Middle);
            str += printNowPlaying(queueList[0], queMaxLength, Left);
        } else if (i == 3) {
            str = printUI("-------------", npMaxLength, Middle);
            str += printNowPlaying(queueList[1], queMaxLength, Left);
        } else if (i == 4) {
            for (int a = 0; a < queMaxLength; a++) {
                str += "-";
            }
        } else if (i == 5) {
            string tmp;
            if (currentlyPlayedMusic.length() > npMaxLength) {
                for (int k = 0; k < npMaxLength; k++) {
                    tmp += currentlyPlayedMusic[i];
                }
                currentlyPlayedMusic = tmp;
            }
            str = printUI(currentlyPlayedMusic, npMaxLength, Middle);
            str += printNowPlaying(queueList[2], queMaxLength, Left);
        } else if (i == 6) {
            for (int a = 0; a < queMaxLength; a++) {
                str += "-";
            }
        } else if (i == 7) {
            str += printNowPlaying(queueList[3], queMaxLength, Left);
        } else if (i == 8) {
            str += printNowPlaying(queueList[4], queMaxLength, Left);
        } else {
            for (int a = 0; a < queMaxLength; a++) {
                str += " ";
            }
        }

        nowPlayingMenu.insert(nowPlayingMenu.end(), str);
    }
    return nowPlayingMenu;
}

void clearScreen() {
    try {
        system("cls");
    } catch (invalid_argument) {
        system("clear");
    }
}

void printConfirm() {
    cout << "Press any key to go back to the Main Menu" << endl;
    getch();
    clearScreen();
}

int main() {
    vector<string> dummyQueueList {"1. A little Piece of Heaven", "2. Afterlife", "3. Fiction", "4. Lost", "5. Save Me"};

    // Define each menu list
    vector<string> playlistMenu{"Playlists "," Choose one option below by typing the number/symbol (0,1,<,... )",
                                 "1. Create New Playlist", "2. Show all playlist", "3. Edit Playlist",
                                 "4. Delete Playlist"};

    vector<string> musicLibraryMenu{"Music Library", " Choose one option below by typing the number/symbol (0,1,<,... )",
                                     "1. Add song to library", "2. Show all song in the library", "3. Search Song" "4. Delete song from the library"};

    vector<string> allMusicLoc;
    vector<Music*> allMusic;
    string musicInQue = " ";

    // Interface/UI
    string option;

    libraryMenu:
    clearScreen();
    printUI(musicLibraryMenu, maxLength, Library);
    getline(cin, option);

    // Add Song (.mp3)
    if (option == "1") {
        string dir;

        clearScreen();
        cout << "Insert directory: "; getline(cin, dir);
        cout << endl;
        allMusicLoc = findMp3(dir);

        for (int i = 0; i < allMusicLoc.size(); i++) {
            FILE *f = fopen(allMusicLoc[i].c_str(), "rb");

            if (f) {
                mp3_id3_tags tags;
                string title, artist;
                if (mp3_id3_file_read_tags(f, &tags)) {
                    title = tags.title;
                    artist = tags.artist;
                    cout << title << " by " << artist << " has been added to library" << endl;
                    allMusic.emplace_back(new Music{title, artist, allMusicLoc[i]});
                } else {
                    fprintf(stderr, "error: %s\n", mp3_id3_failure_reason());
                }

                fclose(f);
            } else {
                printf("failed to open/read '%s'\n", allMusicLoc[i].c_str());
            }
        }

        musicInQue = allMusic[0]->artist + " - " + allMusic[0]->title;
        cout << endl;
        printConfirm();
        goto libraryMenu;
    } else if (option == "2") {
        clearScreen();

        for (int i = 0; i < allMusic.size(); i++) {
            cout << i + 1  << ". "<< allMusic[i]->artist << " - " << allMusic[i]->title << endl;
        }

        printConfirm();
        goto libraryMenu;
    } else if (option == "3") {

    } else if (option == "4") {

    } else if (option == "8") {
        goto nowPlayingMenu;
    } else if (option == "9") {
        goto playlistMenu;
    } else if (option == "0") {
        exit(EXIT_SUCCESS);
    } else if (option == "debug") {

    } else {
        goto libraryMenu;
    }


    playlistMenu:
    clearScreen();
    printUI(playlistMenu, maxLength, Playlist);
    getline(cin, option);

    if (option == "1") {

    } else if (option == "2") {

    } else if (option == "3") {
        
    } else if (option == "4") {
        
    } else if (option == "8") {
        goto libraryMenu;
    } else if (option == "9") {
        goto nowPlayingMenu;
    } else if (option == "0") {
        exit(EXIT_SUCCESS);
    } else if (option == "69") {
        
    } else {
        goto playlistMenu;
    }


    nowPlayingMenu:
    clearScreen();
    vector<string> nowPlayingMenu = createNowPlayingList(musicInQue, maxLength, dummyQueueList);
    printUI(nowPlayingMenu, maxLength, NowPlaying);
    getline(cin, option);

    if (option == "1") {

    } else if (option == "2") {

    } else if (option == "3") {
        
    } else if (option == "4") {
        
    } else if (option == "8") {
        goto playlistMenu;
    } else if (option == "9") {
        goto libraryMenu;
    } else if (option == "0") {
        exit(EXIT_SUCCESS);
    } else if (option == "69") {
        
    } else {
        goto nowPlayingMenu;
    }
}