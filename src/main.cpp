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

// Find all .mp3 files in a directory (and all of it's subdirectory)
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
        printUI(list[i], maxLength);
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

    // Define each menu list
    vector<string> playlistMenu({" Choose one option below by typing the number/symbol (0,1,<,... )",
                                 "1. Create New Playlist", "2. Show all playlist", "3. Edit Playlist",
                                 "4. Delete Playlist"});

    vector<string> musicLibraryMenu({" Choose one option below by typing the number/symbol (0,1,<,... )",
                                     "1. Add song to library", "2. Show all song in the library",
                                     "3. Edit song", "4. Delete song"});

    // Add music from .mp3 files
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




    // Interface/UI
    printUI(playlistMenu, 80);
}