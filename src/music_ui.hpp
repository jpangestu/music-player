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
    NowPlayingMenu, LibraryMenu, PlaylistMenu
};

enum PageAlignment {
    Left, Middle
};

class MusicPlayerUI {
    private:
        int maxWidth = 92;
    public:
        void setMaxWidth(int MaxWidth);
        void printStrip();
        void printSpace();
        std::string printString(std::string message, PageAlignment pageAlignment);
        std::string printString(std::string message, PageAlignment pageAlignment, int width);
        void printNav(std::vector<std::string> nav);
        std::string createNowPlayingList(std::string message, int width, PageAlignment pageAlignment);
        std::vector<std::string> createNowPlayingList(std::string currentlyPlayedMusic, std::vector<std::string> queueList);
        void printMainMenu(std::vector<std::string> list, MenuType menuType);
        void ShowAllSongs(std::vector<Music*> allMusic);
        void AddSongs(int maxWidth, std::string directory, int songFound, int songAdded, int errorCount);
};

void clearScreen();
void printConfirm(std::string menuName);

#endif // From ifndef
