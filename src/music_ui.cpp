#include "music_ui.hpp"
#include "queue.hpp"

// Template for auto-centering output stream (act like std::left or std::right, but for center/middle)
// https://stackoverflow.com/questions/14861018/center-text-in-fixed-width-field-with-stream-manipulators-in-c
template<typename charT, typename traits = std::char_traits<charT> >
class center_helper {
    std::basic_string<charT, traits> str_;
public:
    center_helper(std::basic_string<charT, traits> str) : str_(str) {}
    template<typename a, typename b>
    friend std::basic_ostream<a, b>& operator<<(std::basic_ostream<a, b>& s, const center_helper<a, b>& c);
};

template<typename charT, typename traits = std::char_traits<charT> >
center_helper<charT, traits> centered(std::basic_string<charT, traits> str) {
    return center_helper<charT, traits>(str);
}

// redeclare for std::string directly so we can support anything that implicitly converts to std::string
center_helper<std::string::value_type, std::string::traits_type> centered(const std::string& str) {
    return center_helper<std::string::value_type, std::string::traits_type>(str);
}

template<typename charT, typename traits>
std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& s, const center_helper<charT, traits>& c) {
    std::streamsize w = s.width();
    if (w > c.str_.length()) {
        std::streamsize left = (w + c.str_.length()) / 2;
        s.width(left);
        s << c.str_;
        s.width(w - left);
        s << "";
    } else {
        s << c.str_;
    }
    return s;
}

// Helper for the main printUI function
std::string printUI(std::string message, int maxLength, PageAlignment pageAlignment) {
    if (pageAlignment == Left) {
        message = "| " + message;
        size_t length = maxLength - message.length();
        for (size_t i = 1; i < length; i++) {
            message += " ";
            if (i == length - 1) {
                message += "|";
            }
        }
        return message;
    } else {
        std::string str;
        size_t length = floor(maxLength/2 - message.length()/2);
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
                str += "|";
            }
        }
        return str;
    }   
}

// Print out Menu (Interface/UI) to console
void printUI(std::vector<std::string> list, int maxLength, MenuType menuType) {
    std::string upperStrip, blankLine, lowerStrip, lowerOption;
    upperStrip = blankLine = lowerStrip = lowerOption = "";

    for (int i = 0; i < maxLength; i++) {
        if (i == 0 || i == maxLength - 1) {
            upperStrip += "+";
            blankLine += "|";
        } else {
            upperStrip += "-";
            blankLine += " ";
        }

    }
    
    for (int i = 0; i < maxLength; i++) {
        if (i == 0 || i == maxLength - 1) {
            lowerOption += "|";
            lowerStrip += "+";
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
            if (i == 2) {
                lowerOption += "< Playlist | Library > |";
                i += 24;

                for (int j = 0; j < 24; j++) {
                    if (j == 11 || j == 23) {
                        lowerStrip+= "+";
                    } else {
                        lowerStrip += "-";
                    }
                }
            }
        } else if (menuType == Library) {
            if (i == 2) {
                lowerOption += "< Now Playing | Playlist > |";
                i += 28;
                
                for (int j = 0; j < 28; j++) {
                    if (j == 14 || j == 27) {
                        lowerStrip+= "+";
                    } else {
                        lowerStrip += "-";
                    }
                }
            }
        } else {
            if (i == 2) {
                lowerOption += "< Library | Now Playing > |";
                i += 27;
                
                for (int j = 0; j < 27; j++) {
                    if (j == 10 || j == 26) {
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

    std::cout << upperStrip << std::endl;

    // Print menu (the content)
    if (menuType == NowPlaying) {
        for (int i = 0; i < list.size(); i++) {
            std::cout << "|" << list[i] << "|" << std::endl;
        }
    } else {
        for (int i = 0; i < list.size(); i++) {
            if ( i == 0) {
            std::cout << printUI(list[i], maxLength, Middle) << std::endl;
            } else {
                std::cout << printUI(list[i], maxLength, Left) << std::endl;
            }
        }
        std::cout << blankLine << std::endl;
    }
    std::cout << lowerStrip << std::endl << lowerOption << std::endl << lowerStrip << std::endl;
    std::cout << "| Input: ";
}

// Helper for createNowPlayingList
std::string printNowPlaying(std::string message, int maxLength, PageAlignment pageAlignment) {
    message;
    size_t length = maxLength - message.length();
    if (pageAlignment == Left) {
        size_t length = maxLength - message.length();
        for (size_t i = 0; i < length; i++) {
            message += " ";
        }
        return message;
    } else {
        std::string str;
        size_t length = floor(maxLength/2 - message.length()/2);
        for (size_t i = 0; i < length - 1; i++) {
            str += " ";
        }

        str += message;

        length = maxLength - str.length();
        for (size_t i = 0; i < length; i++) {
            str += " ";
        }
        return str;
    }  
}

// Create Now Playing Menu list (the contents)
std::vector<std::string> createNowPlayingList(std::string currentlyPlayedMusic, int maxLength, std::vector<std::string> queueList) {
    maxLength -= 3; // For counting left & right border
    int npMaxLength = maxLength * 60/100;
    int queMaxLength = maxLength - npMaxLength;
    for (int i = 0; i < queueList.size(); i++) {
        queueList[i] = " " + queueList[i];
    }
    std::vector<std::string> nowPlayingMenu;
    std::string str;

    for (int i = 1; i <= 9; i++) {
        str = "";

        // Now Playing section only
        for (int j = 1; j <= npMaxLength; j++) { // j starts at 1
            if (i == 1 || i == 9) {
                if (j == 1) {
                    str += " +";
                    j++;
                } else if (j == npMaxLength) {
                    str += "-+";
                } else {
                    str += "-";
                }
            } else if (i == 4 || i == 6) {
                if (j == 1 || j == npMaxLength) {
                    str += " |";
                    j++;
                } else {
                    str += " ";
                }
            } else if (i == 7) {
                if (j == 1 || j == npMaxLength) {
                    str += " |";
                    j++;
                } else if (j == 5) {
                    str += "<< Prev.";
                    j += 7;
                } else  if (j == npMaxLength - 2 - 7) { // 2 is for counting the space
                    str += "Next >>";
                    j += 6;
                } else if (j == npMaxLength/2 - 5) {
                    str += "? Play/Pause";
                    j += 11;
                } else {
                    str += " ";
                }
            }  else if (i == 8) {
                if (j == 1 || j == npMaxLength) {
                    str += " |";
                    j++;
                } else if (j == 5) {
                    str += "[ Sort";
                    j += 5;
                } else  if (j == npMaxLength - 2 - 9) {
                    str += "Shuffle ]";
                    j += 8;
                } else {
                    str += " ";
                }
            }
        }

        if (i == 1) {
            str += printNowPlaying("Queue", queMaxLength, Middle);
        } else if (i == 2) {
            str = " " + printUI("Now Playing", npMaxLength, Middle);
            str += printNowPlaying(queueList[0], queMaxLength, Left);
        } else if (i == 3) {
            str = " " + printUI("-------------", npMaxLength, Middle);
            str += printNowPlaying(queueList[1], queMaxLength, Left);
        } else if (i == 4) {
            for (int a = 0; a < queMaxLength; a++) {
                str += "-";
            }
        } else if (i == 5) {
            std::string tmp;
            if (currentlyPlayedMusic.length() > npMaxLength) {
                for (int k = 0; k < npMaxLength; k++) {
                    tmp += currentlyPlayedMusic[i];
                }
                currentlyPlayedMusic = tmp;
            }
            str = " " + printUI(currentlyPlayedMusic, npMaxLength, Middle);
            str += printNowPlaying(queueList[2], queMaxLength, Left);
        } else if (i == 6) {
            for (int a = 0; a < queMaxLength; a++) {
                str += "-";
            }
        } else if (i == 7) {
            str += printNowPlaying(queueList[3], queMaxLength, Left);
        } else if (i == 8) {
            str += printNowPlaying(queueList[4], queMaxLength, Left);
        } else {
            for (int a = 0; a < queMaxLength; a++) {
                str += " ";
            }
        }

        nowPlayingMenu.insert(nowPlayingMenu.end(), str);
    }
    return nowPlayingMenu;
}

void MusicPlayerUI::showAllSong(std::vector<Music*> allMusic) {
    // Find longest title length
    int longestTitle = 0, longestArtist = 0; // longest content length;
    std::string header1 = "No. ", header2 = "Title", header3 = "Artist";
    std::string topSeparator, contentSeparator;

    //Find longest title
    for (int i = 0; i < allMusic.size(); i++) {
        if (longestTitle < allMusic[i]->title.length()) {
            longestTitle = allMusic[i]->title.length();
        }
    }

    // Find longest artist length
    for (int i = 0; i < allMusic.size(); i++) {
        if (longestArtist < allMusic[i]->artist.length()) {
            longestArtist = allMusic[i]->artist.length();
        }
    }

    topSeparator = contentSeparator = "+-";
    topSeparator += "-------";
    contentSeparator += "-----+-";
    for (int i = 0; i < longestTitle; i++) {
        topSeparator += "-";
        contentSeparator += "-";
    }
    topSeparator += "---";
    contentSeparator += "-+-";
    for (int i = 0; i < longestArtist; i++) {
        topSeparator += "-";
        contentSeparator += "-";
    }
    topSeparator += "-+";
    contentSeparator += "-+";

    // Print header
    std::cout << topSeparator << std::endl
    << std::left
    << "| " << std::setw(4 + 3 + longestTitle + 3 + longestArtist) << "!> Enter the corresponding number to play the song" << " |" << std::endl
    << contentSeparator << std::endl
    << "| " << header1 << " | " << std::setw(longestTitle) << centered(header2) << " | " << std::setw(longestArtist) << centered(header3) << " |" << std::endl
    << contentSeparator << std::endl;

    // Print all songs
    for (int i = 0; i < allMusic.size(); i++) {
        std::string iter = std::to_string(i + 1) + ".";
        std::cout << std::left
        << "| "<< std::setw(4) << iter << " | " << std::setw(longestTitle) << allMusic[i]->title << " | " << std::setw(longestArtist) << allMusic[i]->artist << " |" <<std::endl;  
    }

    std::cout << contentSeparator << std::endl
    << "| Input: ";
}

void clearScreen() {
    try {
        system("cls");
    } catch (std::invalid_argument) {
        system("clear");
    }
}

void printConfirm() {
    std::cout << "Press any key to go back to the Main Menu" << std::endl;
    getch();
    clearScreen();
}



