#ifndef MUSIC_UI_H_
#define MUSIC_UI_H_

#include <iostream>
#include <string>
#include <vector>
#include <conio.h> // getch()
#include <cmath> // floor()
#include <iomanip>

#include "queue.hpp"

enum MenuType {
    NowPlaying, Library, Playlist
};

enum PageAlignment {
    Left, Middle
};

class MusicPlayerUI {
    private:
        int maxLength = 92;
    public:
        void showAllSong(std::vector<Music*> allMusic);
        void addMusic(int maxWidth, std::string directory, int songFound, int songAdded, int errorCount);
        void printStrip(int maxWidth);
        void printSpace(int maxWidth);

};

// Maximum number of character for the interface (UI)
extern int maxWidth;

std::string printUI(std::string message, int maxLength, PageAlignment pageAlignment);
void printUI(std::vector<std::string> list, int maxLength, MenuType menuType);
std::vector<std::string> createNowPlayingList(std::string currentlyPlayedMusic, int maxLength, std::vector<std::string> queueList);
std::string printNowPlaying(std::string message, int maxLength, PageAlignment pageAlignment);
void clearScreen();
void printConfirm(std::string menuName);

#endif // From ifndef
