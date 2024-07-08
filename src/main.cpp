#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <windows.h>
#include <algorithm> // sort()
#include <conio.h> // getch()

// For forcing unicode standard
// #include <io.h>
// #include <fcntl.h>
// #include <locale>
// #include <clocale>
// #include <cstdio>

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
    vector<string> allMusic;

    for (const auto& dirEntry : filesystem::recursive_directory_iterator(directory)) {
        auto path = dirEntry.path();
        auto utf8 = path.u8string();
        auto str = std::string(reinterpret_cast<char const*>(utf8.data()), utf8.size());
        // all of the above conversion still not work, but I'll just leave it
        allEntry.insert(allEntry.end(), str);
        // allEntry.insert(allEntry.end(), dirEntry.path().string());
    }

    int index = 0;
    for (vector<string>::iterator i = allEntry.begin(); i < allEntry.end(); i++) {
            filesystem::path tmp = allEntry[index];
            error_code error_code;
            // Store all .mp3 to allMusic
            if (tmp.extension() == ".mp3") {
                allMusic.insert(allMusic.end(), allEntry[index]);
            }

            index++; // Index = vector iterator
        }
    return allMusic;
}





int main() {
    vector<string> dummyQueueList {"1. A little Piece of Heaven", "2. Afterlife", "3. Fiction", "4. Lost", "5. Save Me"};

    // Define each menu list
    vector<string> playlistMenu{"Playlists "," Choose one option below by typing the number/symbol (0,1,<,... )",
                                 "1. Create New Playlist", "2. Show all playlist", "3. Edit Playlist",
                                 "4. Delete Playlist"};

    vector<string> musicLibraryMenu{"Music Library", " Choose one option below by typing the number/symbol (0,1,<,... )",
                                     "1. Add song to library", "2. Show all song in the library", "3. Search Song", "4. Delete song from the library"};

    vector<Music*> allMusic;
    MusicQueue musicQueue;

    // Interface/UI
    string option;

    libraryMenu:
    clearScreen();
    printUI(musicLibraryMenu, maxLength, Library);
    getline(cin, option);

    // Add Song (.mp3)
    if (option == "1") {
        string directory;
        int countSuccess, countError;
        countSuccess = countError = 0;
        vector<string> musicPath, log;

        clearScreen();
        do{
            clearScreen();
            cout << "Insert directory: "; getline(cin, directory);
            cout << endl;
        } while (filesystem::exists(directory) == false); // Error handling (if directory invalid)
        
        musicPath = findMp3(directory);

        for (int i = 0; i < musicPath.size(); i++) {
            FILE *f = fopen(musicPath[i].c_str(), "rb");

            if (f) {
                mp3_id3_tags tags;
                string title, artist;
                if (mp3_id3_file_read_tags(f, &tags)) {
                    title = tags.title;
                    artist = tags.artist;
                    log.emplace_back(title + " - " + artist + "successfully added to the library");
                    allMusic.emplace_back(new Music{title, artist, musicPath[i]});
                    countSuccess++;
                } else {
                    string err = mp3_id3_failure_reason();
                    log.emplace_back("Error: %s\n" + err);
                    countError++;
                }

                fclose(f);
            } else {
                log.emplace_back("Failed to open/read" + musicPath[i]);
                countError++;
            }
        }

        // Sort all music and add to music queue
        sort(allMusic.begin(), allMusic.end(), compareMusicByTitle);
        for (int i = 0; i < allMusic.size(); i++) {      
            musicQueue.addMusic(allMusic[i]->title, allMusic[i]->artist, musicPath[i]);
        }
        musicQueue.setQueueToCircular();

        cout << musicPath.size() << " song(s) found!" << endl;
        cout << countSuccess << " song(s) successfully added to the library" << endl;
        cout << countError << " error(s) occured during import" << endl <<endl;

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
    else if (option == "69") {
        cout << "Before Sorting" << endl;
        for (int i =0; i < allMusic.size(); i++) {
            cout << allMusic[i]->title << " - " << allMusic[i]->artist << endl;
        }
        cout << endl << endl;

        sort(allMusic.begin(), allMusic.end(), compareMusicByTitle);
        cout << "After sorting" << endl;
        for (int i =0; i < allMusic.size(); i++) {
            cout << allMusic[i]->title << " - " << allMusic[i]->artist << endl;
        }

        getch();
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
    vector<string> nowPlayingMenu = createNowPlayingList(musicQueue.getCurrentMusic(), maxLength, dummyQueueList);
    printUI(nowPlayingMenu, maxLength, NowPlaying);
    getline(cin, option);

    long counter;

    if (option == "?") {
        musicQueue.playOrPausedCurrentMusic();
        
        goto nowPlayingMenu;
    }
    else if (option == "<") {
        musicQueue.playPrevMusic();
        goto nowPlayingMenu;
    }
    else if (option == ">") {
        musicQueue.playNextMusic();
        goto nowPlayingMenu;
    }
    // else if (option == "1") {

    // } else if (option == "2") {

    // } else if (option == "3") {
        
    // } else if (option == "4") {
        
    // } else if (option == "5") {

    // }
    else if (option == "6") {
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
        printConfirm();
        goto nowPlayingMenu;
    } else {
        goto nowPlayingMenu;
    }
}