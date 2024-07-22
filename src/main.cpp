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
    vector<Music*> allMusic; // All music in the library is stored here
    vector<Playlist*> allPlaylist;
    vector<Music*> currentQueue; // Currently played queue (can be library or playlist)
    MusicQueue musicQueue;
    MusicPlayerUI musicPlayerUI;

    // Set Main Menu maximum (Library, Playlist, Now Playing) width
    int maxWidth = 92;
    musicPlayerUI.setMaxWidth(maxWidth);

    // Define each menu list
    vector<string> playlistMenu{"Playlists","-----------", "1. Create New Playlist",
                                "2. Show All Playlist", "3. Edit Playlist (Add/Remove Songs To/From Playlist)", "4. Delete Playlist"};

    vector<string> musicLibraryMenu{"Music Library", "---------------", "1. Add song to library",
                                    "2. Show all song in the library", "3. Search Song", "4. Remove song from the library"};

    string option; // User's Input

    libraryMenu:
    clearScreen();
    cout << endl << endl;
    libraryMenuErr:
    musicPlayerUI.printMainMenu(musicLibraryMenu, LibraryMenu);
    getline(cin, option);

    // Add song (.mp3)
    if (option == "1") { 
        string directory;
        int countSuccess, countError, countDuplicate;
        countSuccess = countError = countDuplicate = 0;
        vector<string> musicPath, log;

        clearScreen();
        cout << endl << endl;
        insertDir:
        musicPlayerUI.printStrip();
        cout << musicPlayerUI.makeString("Please input the directory of your music files (ex: C:\\Users\\USERNAME\\Music)", Middle) << endl
        << "| Input   : "; getline(cin, directory);
        if (filesystem::exists(directory) == false) {
            clearScreen();
            cout << endl << "  [Error] Invalid directory" << endl;
            goto insertDir;
        }

        addSongsMenu:
        clearScreen();
        cout << endl << endl;
        musicPlayerUI.printStrip();
        musicPlayerUI.printSpace();
        cout << musicPlayerUI.makeString("Scanning: " + directory, Left) << endl;

        musicPath = findMp3(directory);

        for (int i = 0; i < musicPath.size(); i++) {
            FILE *f = fopen(musicPath[i].c_str(), "rb");

            if (f) {
                mp3_id3_tags tags;
                string title, artist;
                if (mp3_id3_file_read_tags(f, &tags)) {
                    title = tags.title;
                    artist = tags.artist;
                    if (musicQueue.musicInLibrary(allMusic, title, artist) == false) {
                        allMusic.emplace_back(new Music{title, artist, musicPath[i]});
                        log.emplace_back("[SUCCESS]: Added to library -> " + musicPath[i]);
                        countSuccess++;
                    } else {
                        log.emplace_back("[FAIL]   : Already in the library -> " + musicPath[i]);
                        countDuplicate++;
                    }
                } else {
                    string err = mp3_id3_failure_reason();
                    log.emplace_back("[ERROR]  : " + err + " -> " + musicPath[i]);
                    countError++;
                }

                fclose(f);
            } else {
                log.emplace_back("[ERROR]  : Failed to open/read -> " + musicPath[i]);
                countError++;
            }
        }

        // Sort all music and add to music queue
        sort(allMusic.begin(), allMusic.end(), compareMusicByTitle);
        for (int i = 0; i < allMusic.size(); i++) {
            musicQueue.addMusic(allMusic[i]);
        }
        musicQueue.setQueueToCircular();
        currentQueue = allMusic;
        
        musicPlayerUI.printSpace();
        cout << "| " << setw(4) << std::right << to_string(musicPath.size()) << setw(maxWidth - 8) << std::left << " song(s) found" << " |" << endl;
        cout << "| " << setw(4) << std::right << to_string(countDuplicate) << setw(maxWidth - 8) << std::left << " song(s) already in the lbrary" << " |" << endl;
        cout << "| " << setw(4) << std::right << to_string(countSuccess) << setw(maxWidth - 8) << std::left << " song(s) added to the lbrary" << " |" << endl;
        cout << "| " << setw(4) << std::right << to_string(countError) << setw(maxWidth - 8) << std::left << " error(s) occured during import" << " |" << endl;
        musicPlayerUI.printSpace();
        vector<string> nav = {"| < Back |", "| ? View Log |", "| 0. Exit |"};
        nav = musicPlayerUI.makeSubMenuNav(nav, maxWidth);
        for (int i = 0; i < nav.size(); i++) {
            cout << nav[i] << endl;
        }
        cout << musicPlayerUI.makeString("[!] Press Enter or < to go back to the Music Library Menu", Left) << endl;
        cout << "| Input: "; getline(cin, option);
        if (option == "" || option == "<") {
            goto libraryMenu;
        } else if (option == "?") {
            clearScreen();
            for (const auto& string : log) {
                cout << string << endl;
            }
            cout << endl;
            printConfirm("Music Library Menu");
            goto libraryMenu;
        } else if (option == "0") {
            exit(EXIT_SUCCESS);
        } else {
            goto addSongsMenu;
        }
        goto libraryMenu;
    }
    // Show all songs in the library
    else if (option == "2") {
        showMenu:
        clearScreen();
        cout << endl << endl;

        if (allMusic.size() == 0) {
            musicPlayerUI.printStrip();
            cout << musicPlayerUI.makeString("No songs found. Consider adding some first.", Middle) << endl;
            musicPlayerUI.printSpace();
            vector<string> nav = {"| < Back |", "| 0. Exit |"};
            nav = musicPlayerUI.makeSubMenuNav(nav, maxWidth);
            cout << nav[0] << endl;
            cout << nav[1] << endl;
            cout << nav[2] << endl;
            cout << musicPlayerUI.makeString("[!] Press enter or < to go back to the Library Menu", Left) << endl;
            cout << "| Input: "; getline(cin, option);

            if (option == "<" || option == "") {
                goto libraryMenu;
            } else if (option == "0") {
               exit(EXIT_SUCCESS);
            } else {
                goto showMenu;
            }
        }
        
        vector<string> stripAndWidth = musicPlayerUI.ShowAllSongs(allMusic);
        vector<string> nav = {"| < Back |", "| 0. Exit |"};
        nav = musicPlayerUI.makeSubMenuNav(nav, stoi(stripAndWidth[1]));
        nav[0] = musicPlayerUI.combineStrip(stripAndWidth[0], nav[0]);
        cout << nav[0] << endl;
        cout << nav[1] << endl;
        cout << nav[2] << endl;
        cout << musicPlayerUI.makeString("[!] Enter the song number you wish to play", Left, stoi(stripAndWidth[1])) << endl;
        cout << "| Input: "; getline(cin, option);

        try {
            stoi(option);
        } catch (invalid_argument) {
            if (option == "?") {
                goto showMenu;
            } else if (option == "<") {
                goto libraryMenu;
            } else {
                goto showMenu;
            }
        }

        if (stoi(option) >= 1 and stoi(option) <= allMusic.size()) {
            currentQueue = allMusic;
            musicQueue.setCurrentMusic(currentQueue, allMusic[stoi(option) - 1]->path);
            musicQueue.playOrPauseCurrentMusic();
            goto nowPlayingMenu;
        } else if (option == "0") {
            exit(EXIT_SUCCESS);
        } else {
            goto showMenu;
        }
    }
    // Search song
    else if (option == "3") {
        string title;
        int localWidth = maxWidth;

        searchOption:
        clearScreen();
        cout << endl << endl;
        searchOptionErr:
        musicPlayerUI.printStrip();
        cout << "| Search title: "; getline(cin, title);

        title = rmMultSpace(title);

        searchResult:
        clearScreen();
        cout << endl << endl;
        vector<Music*> matchedMusic = musicQueue.searchMusic(allMusic, title);
        if (matchedMusic.size() != 0) {
            clearScreen();
            cout << endl << endl;
            vector<string> stripAndWidth = musicPlayerUI.ShowAllSongs(matchedMusic);
            localWidth = stoi(stripAndWidth[1]);
            vector<string> nav = {"| < Back |", "| ? Search Again |", "| 0. Exit |"};
            nav = musicPlayerUI.makeSubMenuNav(nav, localWidth);
            nav[0] = musicPlayerUI.combineStrip(stripAndWidth[0], nav[0]);
            cout << nav[0] << endl;
            cout << nav[1] << endl;
            cout << nav[2] << endl;
        } else {
            clearScreen();
            cout << endl;
            cout << "  [Result] No matching songs found for the provided title" << endl;
            goto searchOptionErr;
        }
        cout << musicPlayerUI.makeString("[!] Enter the song number you wish to play", Left, localWidth) << endl;
        cout << "| Input: "; getline(cin, option);

        try {
            stoi(option);
        } catch (invalid_argument) {
            if (option == "?") {
                goto searchOption;
            } else if (option == "<") {
                goto libraryMenu;
            } else {
                goto searchResult;
            }
        }

        if (stoi(option) >= 1 and stoi(option) <= matchedMusic.size()) {
            currentQueue = allMusic;
            musicQueue.setCurrentMusic(currentQueue, matchedMusic[stoi(option) - 1]->path);
            musicQueue.playOrPauseCurrentMusic();
            goto nowPlayingMenu;
        } else if (option == "0") {
            exit(EXIT_SUCCESS);
        } else {
            goto searchResult;
        }
    }
    // Remove song from the library 
    else if (option == "4") {
        string title;
        int localWidth = maxWidth;

        removeOption:
        clearScreen();
        cout << endl << endl;
        removeOptionErr:
        musicPlayerUI.printStrip();
        cout << "| Search title to delete: "; getline(cin, title);

        title = rmMultSpace(title);

        removeResult:
        clearScreen();
        cout << endl << endl;
        vector<Music*> matchedMusic = musicQueue.searchMusic(allMusic, title);
        if (matchedMusic.size() != 0) {
            clearScreen();
            cout << endl << endl;
            vector<string> stripAndWidth = musicPlayerUI.ShowAllSongs(matchedMusic);
            localWidth = stoi(stripAndWidth[1]);
            vector<string> nav = {"| < Back |", "| ? Search Again |", "| 0. Exit |"};
            nav = musicPlayerUI.makeSubMenuNav(nav, localWidth);
            nav[0] = musicPlayerUI.combineStrip(stripAndWidth[0], nav[0]);
            cout << nav[0] << endl;
            cout << nav[1] << endl;
            cout << nav[2] << endl;
        } else {
            clearScreen();
            cout << endl;
            cout << "  [Result] No matching songs found for the provided title" << endl;
            goto removeOptionErr;
        }
        cout << musicPlayerUI.makeString("[!] Enter the song number you wish to remove", Left, localWidth) << endl;
        cout << "| Input: "; getline(cin, option);
        
        try {
            stoi(option);
        } catch (invalid_argument) {
            if (option == "?") {
                goto removeOption;
            } else if (option == "<") {
                goto libraryMenu;
            } else {
                goto removeResult;
            }
        }

        if (stoi(option) >= 1 and stoi(option)  <= matchedMusic.size()) {
            allMusic = musicQueue.removeMusic(allMusic, matchedMusic[stoi(option) - 1]);
            cout << endl << matchedMusic[stoi(option)]->title << " - " << matchedMusic[stoi(option)]->artist << " is successfuly removed from the library" << endl << endl;
            printConfirm("Music Library Menu");
            goto libraryMenu;
        } else if (option == "0") {
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
    musicPlayerUI.printMainMenu(playlistMenu, PlaylistMenu);
    getline(cin, option);

    // Add playlist
    if (option == "1") {
        string playlistName;
        clearScreen();
        cout << endl << endl;
        musicPlayerUI.printStrip();
        cout << "| Insert Playlist Name: "; getline(cin, playlistName);
        allPlaylist.emplace_back(new Playlist(playlistName));
        playlistCreated:
        clearScreen();
        cout << endl;
        cout << "[Success] Playlist created successfully" << endl;
        musicPlayerUI.printStrip();
        cout << musicPlayerUI.makeString("Insert Playlist Name: " + playlistName, Left) << endl;
        musicPlayerUI.printSpace();
        vector<string> nav = {"| < Back |", "| 0. Exit |"};
        nav = musicPlayerUI.makeSubMenuNav(nav, maxWidth);
        cout << nav[0] << endl;
        cout << nav[1] << endl;
        cout << nav[2] << endl;
        cout << musicPlayerUI.makeString("[!] Press enter or < to go back to the Playlist Menu", Left) << endl;
        cout << "| Input: "; getline(cin, option);

        if (option == "" || option == "<") {
            goto playlistMenu;
        } else if (option == "0") {
            exit(EXIT_SUCCESS);
        } else {
            goto playlistCreated;
        }
    }
    // Show all playlist
    else if (option == "2") {
        string allPlaylistIndex, songsIndex;
        int localWidth = maxWidth;

        showPlaylist:
        clearScreen();
        cout << endl << endl;
        
        if (allPlaylist.size() == 0) {
            clearScreen(); cout << endl << endl;
            musicPlayerUI.printStrip();
            cout << musicPlayerUI.makeString("Playlist is empty. Consider adding one first.", Middle) << endl;
        } else {
            clearScreen(); cout << endl << endl;
            musicPlayerUI.printStrip();
            for (int i = 0; i < allPlaylist.size(); i++) {
                cout << musicPlayerUI.makeString(to_string(i + 1) + ". " + allPlaylist[i]->name, Left) << endl;
            }
        }

        vector<string> nav = {"| < Back |", "| 0. Exit |"};
        nav = musicPlayerUI.makeSubMenuNav(nav, maxWidth);
        musicPlayerUI.printSpace();
        cout << nav[0] << endl;
        cout << nav[1] << endl;
        cout << nav[2] << endl;
        if (allPlaylist.size() != 0) {
            cout << musicPlayerUI.makeString("[!] Enter the playlist number to view its songs", Left) << endl;
        }
        cout << "| Input: "; getline(cin, allPlaylistIndex);

        try {
            stoi(allPlaylistIndex);
        } catch (invalid_argument) {
            if (allPlaylistIndex == "<") {
            goto playlistMenu;
            } else {
                goto showPlaylist;
            }
        }

        // Playlist selected
        if (stoi(allPlaylistIndex) >= 1 and stoi(allPlaylistIndex) <= allPlaylist.size()) {
            showPlaylistSelected:
            clearScreen(); cout << endl << endl;

            if (allPlaylist[stoi(allPlaylistIndex) - 1]->songs.size() == 0) {
                musicPlayerUI.printStrip();
                cout << musicPlayerUI.makeString("Selected playlist is empty.", Middle) << endl;
                musicPlayerUI.printSpace();
                nav = {"| < Back |", "| 0. Exit |"};
                nav = musicPlayerUI.makeSubMenuNav(nav, localWidth);
            } else {
                vector<string> stripAndWidth = musicPlayerUI.ShowAllSongs(allPlaylist[stoi(allPlaylistIndex) - 1]->songs);
                localWidth = stoi(stripAndWidth[1]);
                nav = {"| < Back |", "| 0. Exit |"};
                nav = musicPlayerUI.makeSubMenuNav(nav, localWidth);
                nav[0] = musicPlayerUI.combineStrip(stripAndWidth[0], nav[0]);
            }
            cout << nav[0] << endl;
            cout << nav[1] << endl;
            cout << nav[2] << endl;
            if (allPlaylist[stoi(allPlaylistIndex) - 1]->songs.size() != 0) {
                cout << musicPlayerUI.makeString("[!] Enter the song number if you wish to play the song", Left, localWidth) << endl;
            }
            cout << "| Input: "; getline(cin, songsIndex);

            try {
                stoi(songsIndex);
            } catch (invalid_argument) {
                if (songsIndex == "<") {
                goto showPlaylist;
                } else {
                    goto showPlaylistSelected;
                }
            }

            // Song selected
            if (stoi(songsIndex) >= 1 and stoi(songsIndex) <= allPlaylist[stoi(allPlaylistIndex) - 1]->songs.size()) {
                for (int i = 0; i < allPlaylist[stoi(allPlaylistIndex) - 1]->songs.size(); i++) {
                    musicQueue.addMusic(allPlaylist[stoi(allPlaylistIndex) - 1]->songs[i]);
                }
                musicQueue.setQueueToCircular();
                currentQueue.clear();
                currentQueue = allPlaylist[stoi(allPlaylistIndex) - 1]->songs;
                musicQueue.setCurrentMusic(currentQueue, allPlaylist[stoi(allPlaylistIndex) - 1]->songs[stoi(songsIndex) - 1]->path);
                musicQueue.playOrPauseCurrentMusic();
            } else if (songsIndex == "0") {
                exit(EXIT_SUCCESS);
            } else {
                goto showPlaylistSelected;
            }
        } else if (allPlaylistIndex == "0") {
            exit(EXIT_SUCCESS);
        } else {
            goto showPlaylist;
        }
    }
    // Edit playlist
    else if (option == "3") {
        string allPlaylistIndex;
        editPlaylistMenu:
        clearScreen();
        cout << endl;
        if (allPlaylist.size() == 0) {
            musicPlayerUI.printStrip();
            cout << musicPlayerUI.makeString("No playlists found. Consider creating one first.", Middle) << endl;
            musicPlayerUI.printSpace();
        } else {
            cout << endl;
            musicPlayerUI.printStrip();
            for (int i = 0; i < allPlaylist.size(); i++) {
                cout << musicPlayerUI.makeString(to_string(i + 1) + ". " + allPlaylist[i]->name, Left) << endl;
            }
            musicPlayerUI.printSpace();
        }

        vector<string> nav = {"| < Back |", "| 0. Exit |"};
        nav = musicPlayerUI.makeSubMenuNav(nav, maxWidth);
        cout << nav[0] << endl;
        cout << nav[1] << endl;
        cout << nav[2] << endl;
        if (allPlaylist.size() != 0) {
            cout << musicPlayerUI.makeString("[!] Enter the playlist number you wish to edit", Left) << endl;
        }
        cout << "| Input: "; getline(cin, allPlaylistIndex);

        try {
            stoi(allPlaylistIndex);
        } catch (invalid_argument) {
            if (allPlaylistIndex == "<") {
            goto playlistMenu;
            } else {
                goto editPlaylistMenu;
            }
        }

        if (stoi(allPlaylistIndex) >= 1 and stoi(allPlaylistIndex) <= allPlaylist.size()) {
            clearScreen();
            cout << endl;
            playlistSelected:
            musicPlayerUI.printStrip();
            cout << musicPlayerUI.makeString("Currently Editing: " + allPlaylist[stoi(allPlaylistIndex) - 1]->name, Middle) << endl;
            cout << musicPlayerUI.makeString("1. Add Song", Left) << endl;
            cout << musicPlayerUI.makeString("2. Remove Song", Left) << endl;
            musicPlayerUI.printSpace();
            nav = {"| < Back |", "| 0. Exit |"};
            nav = musicPlayerUI.makeSubMenuNav(nav, maxWidth);
            cout << nav[0] << endl;
            cout << nav[1] << endl;
            cout << nav[2] << endl;
            cout << "| Input: "; getline(cin, option);

            if (option == "<") {
                goto editPlaylistMenu;
            }
            // Edit playlist -> Add song
            else if (option == "1") {
                string title2 = "", songIndex;
                int localWidth2 = maxWidth;

                searchOption2:
                clearScreen();
                cout << endl << endl;
                searchOption2Err:
                musicPlayerUI.printStrip();
                cout << "| Search title: "; getline(cin, title2);

                title2 = rmMultSpace(title2);

                searchResult2:
                clearScreen();
                cout << endl << endl;
                vector<Music*> matchedMusic = musicQueue.searchMusic(allMusic, title2);
                if (matchedMusic.size() != 0) {
                    clearScreen();
                    cout << endl << endl;
                    vector<string> stripAndWidth = musicPlayerUI.ShowAllSongs(matchedMusic);
                    localWidth2 = stoi(stripAndWidth[1]);
                    nav = {"| < Back |", "| ? Search Again |", "| 0. Exit |"};
                    nav = musicPlayerUI.makeSubMenuNav(nav, localWidth2);
                    nav[0] = musicPlayerUI.combineStrip(stripAndWidth[0], nav[0]);
                    cout << nav[0] << endl;
                    cout << nav[1] << endl;
                    cout << nav[2] << endl;
                } else {
                    clearScreen();
                    cout << endl;
                    cout << "  [Result] No matching songs found for the provided title" << endl;
                    goto searchOption2Err;
                }
                cout << musicPlayerUI.makeString("[!] Enter the number of the song you wish to add to the playlist", Left, localWidth2) << endl;
                cout << "| Input: "; getline(cin, songIndex);

                try {
                    stoi(songIndex);
                } catch (invalid_argument) {
                    if (songIndex == "?") {
                        goto searchOption2;
                    } else if (songIndex == "<") {
                        goto playlistSelected;
                    } else {
                        goto searchResult2;
                    }
                }

                if (stoi(songIndex) >= 1 and stoi(songIndex) <= matchedMusic.size()) {
                    if (allPlaylist[stoi(allPlaylistIndex) - 1]->songs.size() != 0) {
                        // Check duplicate
                        for (int i = 0; i < allPlaylist[stoi(allPlaylistIndex) - 1]->songs.size(); i++) {
                            if (allPlaylist[stoi(allPlaylistIndex) - 1]->songs[i] == matchedMusic[stoi(songIndex) - 1]) {
                                clearScreen();
                                cout << endl;
                                cout << "[Fail] The selected song is already in the playlist" << endl;
                                goto playlistSelected;
                            }
                        }
                    }

                    // Add song to the playlist and sort playlist alphabetically
                    allPlaylist[stoi(allPlaylistIndex) - 1]->songs.emplace_back(matchedMusic[stoi(songIndex) - 1]);
                    sort(allPlaylist[stoi(allPlaylistIndex) - 1]->songs.begin(), allPlaylist[stoi(allPlaylistIndex) - 1]->songs.end(), compareMusicByTitle);
                    clearScreen();
                    cout << endl;
                    cout << "[Success] Song is successfully added to the playlist" << endl;
                    goto playlistSelected;
                } else if (songIndex == "0") {
                    exit(EXIT_SUCCESS);
                } else {
                    goto searchResult2;
                }
            }
            // Edit playlist -> Remove song
            else if (option == "2") {
                string songsIndex;
                removeFromPlaylist:
                clearScreen();
                cout << endl << endl;
                if (allPlaylist[stoi(allPlaylistIndex) - 1]->songs.size() == 0) {
                    musicPlayerUI.printStrip();
                    cout << musicPlayerUI.makeString("Playlist is empty. Nothing to remove.", Middle) << endl;
                    musicPlayerUI.printSpace();
                } else {
                    musicPlayerUI.printStrip();
                    for (int i = 0; i < allPlaylist[stoi(allPlaylistIndex) - 1]->songs.size(); i++) {
                        cout << musicPlayerUI.makeString(to_string(i + 1) + ". " +
                        allPlaylist[stoi(allPlaylistIndex) - 1]->songs[i]->title + " - " +
                        allPlaylist[stoi(allPlaylistIndex) - 1]->songs[i]->artist, Left) << endl;
                    }
                }
                
                nav = {"| < Back |", "| 0. Exit |"};
                nav = musicPlayerUI.makeSubMenuNav(nav, maxWidth);
                cout << nav[0] << endl;
                cout << nav[1] << endl;
                cout << nav[2] << endl;    
                if (allPlaylist[stoi(allPlaylistIndex) - 1]->songs.size() != 0) {
                    cout << musicPlayerUI.makeString("[!] Enter the number of the song you wish to remove from the playlist", Left) << endl;
                }
                cout << "| Input: "; getline(cin, songsIndex);

                try {
                    stoi(songsIndex);
                } catch (invalid_argument) {
                    if (songsIndex == "<") {
                        clearScreen();
                        cout << endl << endl;
                        goto playlistSelected;
                    } else {
                        goto removeFromPlaylist;
                    }
                }

                if (stoi(songsIndex) >= 1 and stoi(songsIndex) <= allPlaylist[stoi(allPlaylistIndex) - 1]->songs.size()) {
                    allPlaylist[stoi(allPlaylistIndex) - 1]->songs = musicQueue.removeMusic(allPlaylist[stoi(allPlaylistIndex) - 1]->songs, allPlaylist[stoi(allPlaylistIndex) - 1]->songs[stoi(songsIndex) - 1]);
                    clearScreen();
                    cout << endl << endl;
                    cout << "  [Success] The selected song has been removed from the playlist" << endl << endl;
                    printConfirm("Edit Playlist Menu");
                    goto removeFromPlaylist;
                } else if (songsIndex == "0") {
                    exit(EXIT_SUCCESS);
                } else {
                    clearScreen();
                    cout << endl << endl;
                    goto playlistSelected;
                }
            } else if (option == "0") {
                exit(EXIT_SUCCESS);
            } else {
                clearScreen();
                cout << endl << endl;
                goto playlistSelected;
            }
        } else if (option == "0") {
            exit(EXIT_SUCCESS);
        } else {
            goto editPlaylistMenu;
        } 
    }
    // Delete playlist
    else if (option == "4") {
        clearScreen();
        cout << endl << endl;
        deletePlaylist:
        
        if (allPlaylist.size() == 0) {
            clearScreen(); cout << endl << endl;
            musicPlayerUI.printStrip();
            cout << musicPlayerUI.makeString("Playlist is empty. Nothing to delete.", Middle) << endl;
        } else {
            clearScreen(); cout << endl << endl;
            musicPlayerUI.printStrip();
            for (int i = 0; i < allPlaylist.size(); i++) {
                cout << musicPlayerUI.makeString(to_string(i + 1) + ". " + allPlaylist[i]->name, Left) << endl;
            }
        }

        vector<string> nav = {"| < Back |", "| 0. Exit |"};
        nav = musicPlayerUI.makeSubMenuNav(nav, maxWidth);
        musicPlayerUI.printSpace();
        cout << nav[0] << endl;
        cout << nav[1] << endl;
        cout << nav[2] << endl;
        if (allPlaylist.size() != 0) {
            cout << musicPlayerUI.makeString("[!] Enter the playlist number you wish to delete", Left) << endl;
        }
        cout << "| Input: "; getline(cin, option);

        try {
            stoi(option);
        } catch (invalid_argument) {
            if (option == "<") {
            goto playlistMenu;
            } else {
                clearScreen(); cout << endl << endl;
                goto deletePlaylist;
            }
        }

        // Playlist selected
        if (stoi(option) >= 1 and stoi(option) <= allPlaylist.size()) {
            int index = 0;
            auto it = allPlaylist.begin();
            while (it != allPlaylist.end()) {
                if (index == stoi(option) - 1) {
                    allPlaylist.erase(it);
                } else {
                    ++it;
                }
                ++index;
            }
            
            clearScreen(); cout << endl;
            cout << musicPlayerUI.makeString("  [Success] Selected playlist has been deleted", Left) << endl;
            goto deletePlaylist;
        } else if (option == "0") {
            exit(EXIT_SUCCESS);
        } else {
            clearScreen(); cout << endl << endl;
            goto deletePlaylist;
        }
    }
    else if (option == "<") {
        goto libraryMenu;
    } else if (option == ">") {
        goto nowPlayingMenu;
    } else if (option == "0") {
        exit(EXIT_SUCCESS);
    } else {
        goto playlistMenu;
    }


    nowPlayingMenu:
    musicQueue.sort(currentQueue);
    clearScreen();
    cout << endl << endl;
    nowPlayingMenuErr:
    musicPlayerUI.printMainMenu(musicPlayerUI.createNowPlayingList(musicQueue.getCurrentMusic(), musicQueue.getQueueList(5)), NowPlayingMenu);
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