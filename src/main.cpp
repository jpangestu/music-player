#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <windows.h>
#include <algorithm> // sort()
#include <conio.h> // getch()

#include "music_ui.hpp"
#include "queue.hpp"

// For reading .mp3 files tag (https://github.com/Arcxm/mp3_id3_tags)
#define MP3_ID3_TAGS_IMPLEMENTATION
#include "../lib/mp3_id3_tags/mp3_id3_tags.h"

using namespace std;

int maxLength = 100;


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





int main() {
    vector<string> dummyQueueList {"1. A little Piece of Heaven", "2. Afterlife", "3. Fiction", "4. Lost", "5. Save Me"};

    // Define each menu list
    vector<string> playlistMenu{"Playlists "," Choose one option below by typing the number/symbol (0,1,<,... )",
                                 "1. Create New Playlist", "2. Show all playlist", "3. Edit Playlist",
                                 "4. Delete Playlist"};

    vector<string> musicLibraryMenu{"Music Library", " Choose one option below by typing the number/symbol (0,1,<,... )",
                                     "1. Add song to library", "2. Show all song in the library", "3. Search Song", "4. Delete song from the library"};

    vector<string> allMusicLoc;
    vector<Music*> allMusic;
    string musicInQue = " ";
    Queue myQueue;

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
                    cout << title << " by " << artist << " has added to the library" << endl;
                    allMusic.emplace_back(new Music{title, artist, allMusicLoc[i]});
                    myQueue.addSong(title, artist, allMusicLoc[i]);
                } else {
                    fprintf(stderr, "error: %s\n", mp3_id3_failure_reason());
                }

                fclose(f);
            } else {
                printf("failed to open/read '%s'\n", allMusicLoc[i].c_str());
            }
        }

        musicInQue = myQueue.musicInQue();
        cout << endl;
        printConfirm();
        goto libraryMenu;
    } else if (option == "2") {
        clearScreen();

        for (int i = 0; i < allMusic.size(); i++) {
            cout << i + 1  << ". "<< allMusic[i]->title << " - " << allMusic[i]->artist << endl;
        }

        printConfirm();
        goto libraryMenu;
    }
    // else if (option == "3") {
        
    // } else if (option == "4") {

    // }
    else if (option == "8") {
        goto nowPlayingMenu;
    } else if (option == "9") {
        goto playlistMenu;
    } else if (option == "0") {
        exit(EXIT_SUCCESS);
    } else {
        goto libraryMenu;
    }


    playlistMenu:
    clearScreen();
    printUI(playlistMenu, maxLength, Playlist);
    getline(cin, option);

    // if (option == "1") {

    // } else if (option == "2") {

    // } else if (option == "3") {
        
    // } else if (option == "4") {
        
    // } else
    if (option == "8") {
        goto libraryMenu;
    } else if (option == "9") {
        goto nowPlayingMenu;
    } else if (option == "0") {
        exit(EXIT_SUCCESS);
    } else {
        goto playlistMenu;
    }


    nowPlayingMenu:
    clearScreen();
    vector<string> nowPlayingMenu = createNowPlayingList(musicInQue, maxLength, dummyQueueList);
    printUI(nowPlayingMenu, maxLength, NowPlaying);
    getline(cin, option);

    long counter;

    if (option == "?") {
        if(musicInQue != "") {
            if (counter % 2 == 1) {
                myQueue.playCurrentSong(true);
            } else if (counter % 2 == 0) {
                myQueue.playCurrentSong(false);
            }
            counter++;
        }
        
        goto nowPlayingMenu;
    }
    // else if (option == "<") {

    // }
    else if (option == ">") {
        myQueue.playNextSong();
        musicInQue = myQueue.musicInQue();
        goto nowPlayingMenu;
    }
    // else if (option == "1") {

    // } else if (option == "2") {

    // } else if (option == "3") {
        
    // } else if (option == "4") {
        
    // } else if (option == "5") {

    // }
    else if (option == "6") {
        myQueue.shuffleQueue();
        goto nowPlayingMenu;
    }
    // else if (option == "7") {

    // }
    else if (option == "8") {
        goto playlistMenu;
    } else if (option == "9") {
        goto libraryMenu;
    } else if (option == "0") {
        exit(EXIT_SUCCESS);
    } else if (option == "display") {
        myQueue.displayPlaylist();
        printConfirm();
        goto nowPlayingMenu;
    } else {
        goto nowPlayingMenu;
    }
}