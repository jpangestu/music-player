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

// Main Menu (Library, Playlist, Now Playing) width
int maxLength = 92;

// Find all .mp3 files in a directory (and all of it's subdirectory) and store it in vector
vector<string> findMp3(string directory) {
    vector<filesystem::path> allEntry;
    vector<string> allMusic;

    // Store all directory to allEntry
    for (const auto& dirEntry : filesystem::recursive_directory_iterator(directory)) {
        allEntry.insert(allEntry.end(), dirEntry.path().generic_u8string());
    }

    // Select only the .mp3 file and store it to allMusic
    int index = 0;
    for (vector<filesystem::path>::iterator i = allEntry.begin(); i < allEntry.end(); i++) {
            filesystem::path tmp = allEntry[index];
            auto utf8 = tmp.u8string();
            auto str = std::string(reinterpret_cast<char const*>(utf8.data()), utf8.size());
            error_code error_code;
            // Store all .mp3 to allMusic
            if (tmp.extension() == ".mp3") {
                allMusic.insert(allMusic.end(), str);
            }

            index++; // Index = vector iterator
        }
    return allMusic;
}

// Remove unnecessary spaces
// https://stackoverflow.com/questions/8362094/replace-multiple-spaces-with-one-space-in-a-string
string rmMultSpace(string input) {
    // Remove unnecessary space
    std::string::iterator new_end = std::unique(input.begin(), input.end(),
    [=](char lhs, char rhs){ return (lhs == rhs) && (lhs == ' '); }
    );
    input.erase(new_end, input.end());
    return input;
}


int main() {
    // Define each menu list
    vector<string> playlistMenu{"Playlists "," Choose one option below by typing the number/symbol (0,1,<,... )",
                                 "1. Create New Playlist", "2. Show all playlist", "3. Edit Playlist",
                                 "4. Delete Playlist"};

    vector<string> musicLibraryMenu{"Music Library", " Choose one option below by typing the number/symbol (0,1,<,... )",
                                     "1. Add song to library", "2. Show all song in the library", "3. Search Song", "4. Remove song from the library"};


    vector<Music*> allMusic; // All music in the library is stored here
    vector<Music*> playlist[10]; // beta
    vector<Music*> currentQueue; // Currently played queue (can be library or playlist)
    MusicQueue musicQueue;
    MusicPlayerUI musicPlayerUI;

    string option; // User's Input

    libraryMenu:
    clearScreen();
    cout << endl << endl;
    libraryMenuErr:
    printUI(musicLibraryMenu, maxLength, Library);
    getline(cin, option);

    // Add music (.mp3)
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
                    if (musicQueue.musicInLibrary(allMusic, musicPath[i]) == false) {
                        allMusic.emplace_back(new Music{title, artist, musicPath[i]});
                        countSuccess++;
                    }
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

        cout << endl << endl << endl;
        // Sort all music and add to music queue
        sort(allMusic.begin(), allMusic.end(), compareMusicByTitle);
        for (int i = 0; i < allMusic.size(); i++) {
            musicQueue.addMusic(allMusic[i]);
        }
        musicQueue.setQueueToCircular();

        currentQueue = allMusic;

        cout << musicPath.size() << " song(s) found!" << endl;
        cout << countSuccess << " song(s) successfully added to the library" << endl;
        cout << countError << " error(s) occured during import" << endl <<endl;

        getch();
        string logOption;
        cout << "Enter ! to view log" << endl;
        cin >> logOption;

        if (logOption == "!") {
            for (int i = 0; i < log.size(); i++) {
                cout << log[i] << endl << endl;
            }
        }

        printConfirm();
        goto libraryMenu;
    }
    // Show all music in the library
    else if (option == "2") {
        string showOption;

        showMenu:
        clearScreen();
        cout << endl << endl;
        musicPlayerUI.showAllSong(allMusic);
        getline(cin, showOption);

        try {
            stoi(showOption);
        } catch (invalid_argument) {
            if (showOption == "?") {
                goto showMenu;
            } else if (showOption == "<") {
                goto libraryMenu;
            } else {
                goto showMenu;
            }
        }

        if (stoi(showOption) >= 1 and stoi(showOption) <= 15) {
            musicQueue.setCurrentMusic(currentQueue, allMusic[stoi(showOption) - 1]->path);
            musicQueue.playOrPauseCurrentMusic();
            goto nowPlayingMenu;
        } else if (showOption == "0") {
            exit(EXIT_SUCCESS);
        } else {
            goto showMenu;
        }
    }
    // Search music
    else if (option == "3") {
        string searchOption, title;

        searchOption:
        clearScreen();
        cout << endl << endl;
        cout << "Search title: "; getline(cin, title);

        title = rmMultSpace(title);

        searchResult:
        clearScreen();
        cout << "  Enter the corresponding number to play the song" << endl;
        vector<Music*> matchedMusic = musicQueue.searchMusic(allMusic, title);
        for (int i = 0; i < matchedMusic.size(); i++) {
            cout << i + 1 << ". " << matchedMusic[i]->title << " - " << matchedMusic[i]->artist << endl;
        }
        cout << "? Re-search | < Back to Library Menu | 0. Exit" << endl;
        cout << "Input: "; getline(cin, searchOption);

        try {
            stoi(searchOption);
        } catch (invalid_argument) {
            if (searchOption == "?") {
                goto searchOption;
            } else if (searchOption == "<") {
                goto libraryMenu;
            } else {
                goto searchResult;
            }
        }

        if (stoi(searchOption) >= 1 and stoi(searchOption) <= 15) {
            musicQueue.setCurrentMusic(currentQueue, matchedMusic[stoi(searchOption) - 1]->path);
            musicQueue.playOrPauseCurrentMusic();
            goto nowPlayingMenu;
        } else if (searchOption == "0") {
            exit(EXIT_SUCCESS);
        } else {
            goto searchResult;
        }
    }
    // Remove music from the library 
    else if (option == "4") {
        string removeOption, title;

        removeOption:
        clearScreen();
        cout << endl << endl;
        cout << "Search title to delete: "; getline(cin, title);

        title = rmMultSpace(title);

        removeResult:
        clearScreen();
        cout << "  Enter the corresponding number to delete the song" << endl;
        vector<Music*> matchedMusic = musicQueue.searchMusic(allMusic, title);
        for (int i = 0; i < matchedMusic.size(); i++) {
            cout << i + 1 << ". " << matchedMusic[i]->title << " - " << matchedMusic[i]->artist << endl;
        }
        cout << "? Re-search | < Back to Library Menu | 0. Exit" << endl;
        cout << "Input: "; getline(cin, removeOption);

        try {
            stoi(removeOption);
        } catch (invalid_argument) {
            if (removeOption == "?") {
                goto removeOption;
            } else if (removeOption == "<") {
                goto libraryMenu;
            } else {
                goto removeResult;
            }
        }

        if (stoi(removeOption) >= 1 and stoi(removeOption) <= 15) {
            musicQueue.removeMusic(allMusic, matchedMusic[stoi(removeOption) - 1]);
            getch();
            goto nowPlayingMenu;
        } else if (removeOption == "0") {
            exit(EXIT_SUCCESS);
        } else {
            goto removeResult;
        }
    }
    else if (option == "<") {
        goto nowPlayingMenu;
    }
    else if (option == ">") {
        goto playlistMenu;
    }
    else if (option == "0") {
        exit(EXIT_SUCCESS);
    } else if (option == "test") {
        getch();
    }
    else {
        goto libraryMenu;
    }


    playlistMenu:
    clearScreen();
    cout << endl << endl;
    playlistMenuErr:
    printUI(playlistMenu, maxLength, Playlist);
    getline(cin, option);

    // if (option == "1") {

    // } else if (option == "2") {

    // } else if (option == "3") {
        
    // } else if (option == "4") {
        
    // } else
    if (option == "<") {
        goto libraryMenu;
    } else if (option == ">") {
        goto nowPlayingMenu;
    } else if (option == "0") {
        exit(EXIT_SUCCESS);
    } else {
        goto playlistMenu;
    }


    nowPlayingMenu:
    clearScreen();
    cout << endl << endl;
    nowPlayingMenuErr:
    vector<string> nowPlayingMenu = createNowPlayingList(musicQueue.getCurrentMusic(), maxLength, musicQueue.getQueueList(5));
    printUI(nowPlayingMenu, maxLength, NowPlaying);
    getline(cin, option);

    if (option == "?") {
        musicQueue.playOrPauseCurrentMusic();
        goto nowPlayingMenu;
    } else if (option == "<<") {
        musicQueue.playPrevMusic();
        goto nowPlayingMenu;
    } else if (option == ">>") {
        musicQueue.playNextMusic();
        goto nowPlayingMenu;
    } else if (option == "]") {
        musicQueue.shuffle(currentQueue);
        goto nowPlayingMenu;
    } else if (option == "[") {
        musicQueue.sort(currentQueue);
        goto nowPlayingMenu;
    } else if (option == "1") {
        musicQueue.setCurrentMusic(-2);
        musicQueue.playOrPauseCurrentMusic();
        goto nowPlayingMenu;
    } else if (option == "2") {
        musicQueue.setCurrentMusic(-1);
        musicQueue.playOrPauseCurrentMusic();
        goto nowPlayingMenu;
    } else if (option == "3") {
        musicQueue.setCurrentMusic(0);
        musicQueue.playOrPauseCurrentMusic();
        goto nowPlayingMenu;
    } else if (option == "4") {
        musicQueue.setCurrentMusic(1);
        musicQueue.playOrPauseCurrentMusic();
        goto nowPlayingMenu;
    } else if (option == "5") {
        musicQueue.setCurrentMusic(2);
        musicQueue.playOrPauseCurrentMusic();
        goto nowPlayingMenu;
    } else if (option == "<") {
        goto playlistMenu;
    } else if (option == ">") {
        goto libraryMenu;
    } else if (option == "0") {
        exit(EXIT_SUCCESS);
    } else {
        goto nowPlayingMenu;
    }
}