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
        void setMaxWidth(int MaxWidth);
        void printStrip();
        void printStrip(int width);
        void printSpace();
        void printSpace(int width);
        std::string makeString(std::string message, PageAlignment pageAlignment);
        std::string makeString(std::string message, PageAlignment pageAlignment, int width);
        std::string combineStrip(std::string strip1, std::string strip2);
        std::vector<std::string> makeSubMenuNav(std::vector<std::string> nav, int width);
        std::string createNowPlayingList(std::string message, int width, PageAlignment pageAlignment);
        std::vector<std::string> createNowPlayingList(std::string currentlyPlayedMusic, std::vector<std::string> queueList);
        void printMainMenu(std::vector<std::string> list, MenuType menuType);
        std::vector<std::string> ShowAllSongs(std::vector<Music*> allMusic);
        void AddSongs(int maxWidth, std::string directory, int songFound, int songAdded, int errorCount);
};

void clearScreen();
void printConfirm();

#endif // From ifndef
