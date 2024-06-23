#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

// For reading .mp3 files tag (https://github.com/Arcxm/mp3_id3_tags)
#define MP3_ID3_TAGS_IMPLEMENTATION
#include "../lib/mp3_id3_tags/mp3_id3_tags.h"

using namespace std;

struct Music {
    string title;
    string artist;
    string album;
    int year;
};

enum MenuType {
    NowPlaying, Library, Playlist
};

enum PageAlignment {
    Left, Middle
};

// Maximum number of character for the interface (UI)
int maxLength = 85;

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
void printUI(string message, int maxLength, PageAlignment pageAlignment) {
    if (pageAlignment == Left) {
        message = "| " + message;
        size_t length = maxLength - message.length();
        for (size_t i = 1; i < length; i++) {
            message += " ";
            if (i == length - 1) {
                message += "|\n";
            }
        }
        cout << message;
    } else {
        string str;
        size_t length = maxLength/2 - message.length()/2;
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
                str += "|\n";
            }
        }
        cout << str;
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
            lowerOption += "|";
            lowerStrip += "+";
            continue;
        } else {
            upperStrip += "-";
            blankLine += " ";
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
            if ( i == 2) {
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
            if ( i == 2) {
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
            if ( i == 2) {
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

    if (menuType == NowPlaying) {
        
    } else {
        // Print menu (the content)
        for (int i = 0; i < list.size(); i++) {
            if ( i == 0) {
            printUI(list[i], maxLength, Middle);
            } else {
                printUI(list[i], maxLength, Left);
            }
        }
    }

    cout << blankLine << endl << lowerStrip << endl << lowerOption << endl << lowerStrip << endl;
    cout << "| Input: ";
}

// Create Now Playing Menu list (the contents)
vector<string> createNowPlayingList(string currentlyPlayedMusic, int maxLength, vector<string> queueList) {
    int npMaxLength = maxLength * 60/100;
    int queMaxLength = maxLength - npMaxLength;
    vector<string> nowPlayingMenu;

    for (int i = 0; i < npMaxLength; i++) {
        
    }

    for (int i = 0; i < queMaxLength; i++) {

    }
    nowPlayingMenu.insert(nowPlayingMenu.end(), "");

    return nowPlayingMenu;
}

void clearScreen() {
    try {
        system("cls");
    } catch (invalid_argument) {
        system("clear");
    }
}

int main() {

    // Define each menu list
    vector<string> playlistMenu({"Playlists "," Choose one option below by typing the number/symbol (0,1,<,... )",
                                 "1. Create New Playlist", "2. Show all playlist", "3. Edit Playlist",
                                 "4. Delete Playlist"});

    vector<string> musicLibraryMenu({"Music Library", " Choose one option below by typing the number/symbol (0,1,<,... )",
                                     "1. Add song to library", "2. Show all song in the library",
                                     "3. Edit song", "4. Delete song"});

    




    // Interface/UI
    string option;

    libraryMenu:
    clearScreen();
    printUI(musicLibraryMenu, maxLength, Library);
    getline(cin, option);

    // Add Song (.mp3)
    if (option == "1") {
        string dir;
        vector<string> allMp3Loc;
        vector<Music> allMp3File;
        cout << "Insert directory: "; getline(cin, dir);
        allMp3Loc = findMp3(dir);

        for (int i = 0; i < allMp3Loc.size(); i++) {
            FILE *f = fopen(allMp3Loc[i].c_str(), "rb");

            if (f) {
                mp3_id3_tags tags;
                if (mp3_id3_file_read_tags(f, &tags)) {
                    printf("MP3: %s\n\n", allMp3Loc[i].c_str());

                    printf("Title: %s\n", tags.title);
                    printf("Artist: %s\n", tags.artist);
                    printf("Album: %s\n", tags.album);
                    printf("Year: %s\n", tags.year);
                } else {
                    fprintf(stderr, "error: %s\n", mp3_id3_failure_reason());
                }

                fclose(f);
            } else {
                printf("failed to open/read '%s'\n", allMp3Loc[i].c_str());
            }
        }
    } else if (option == "2") {
        string dir;
        getline(cin, dir);
        mp3_id3_tags tags;

        char *artist = mp3_id3_read_tag(dir.c_str(), ARTIST);
        if (artist) {
        fprintf(stdout, "Artist: %s\n", artist);
        free(artist);
        artist = NULL;
        } else {
        fprintf(stderr, "error: %s\n", mp3_id3_failure_reason());
        }

        if (mp3_id3_read_tags(dir.c_str(), &tags)) {
            fprintf(stdout, "%s by %s\n", tags.title, tags.artist);
        } else {
            fprintf(stderr, "error: %s\n", mp3_id3_failure_reason());
        }

    } else if (option == "3") {

    } else if (option == "4") {

    } else if (option == "8") {

    } else if (option == "9") {
        goto playlistMenu;
    } else if (option == "0") {
        exit(EXIT_SUCCESS);
    } else if (option == "69") {
        string dir;
        getline(cin, dir);
        FILE *f = fopen(dir.c_str(), "rb");
        if (f) {
            mp3_id3_tags tags;
            if (mp3_id3_file_read_tags(f, &tags)) {
                printf("MP3: %s\n\n", dir.c_str());

                printf("Title: %s\n", tags.title);
                printf("Artist: %s\n", tags.artist);
                printf("Album: %s\n", tags.album);
                printf("Year: %s\n", tags.year);
            } else {
                fprintf(stderr, "error: %s\n", mp3_id3_failure_reason());
            }

            fclose(f);
        } else {
            printf("failed to open/read '%s'\n", dir.c_str());
        }
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
        
    } else if (option == "0") {
        exit(EXIT_SUCCESS);
    } else if (option == "69") {
        
    } else {
        goto playlistMenu;
    }
}