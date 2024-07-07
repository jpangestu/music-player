enum MenuType {
    NowPlaying, Library, Playlist
};

enum PageAlignment {
    Left, Middle
};

// Maximum number of character for the interface (UI)
extern int maxLength;

std::string printUI(std::string message, int maxLength, PageAlignment pageAlignment);
void printUI(std::vector<std::string> list, int maxLength, MenuType menuType);
std::string printNowPlaying(std::string message, int maxLength, PageAlignment pageAlignment);
std::vector<std::string> createNowPlayingList(std::string currentlyPlayedMusic, int maxLength, std::vector<std::string> queueList);
void clearScreen();
void printConfirm();