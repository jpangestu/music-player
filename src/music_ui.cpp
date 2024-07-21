#include "music_ui.hpp"
#include "align_helper.hpp"

void MusicPlayerUI::setMaxWidth(int MaxWidth) {
    maxWidth = MaxWidth;
}

void MusicPlayerUI::printStrip() {
    for (int i = 0; i < maxWidth; i++) {
        if (i == 0 || i == maxWidth - 1) {
            std::cout << "+";
        } else {
            std::cout << "-";
        }
    }
    std::cout << std::endl;
}

void MusicPlayerUI::printStrip(int width) {
    for (int i = 0; i < width; i++) {
        if (i == 0 || i == width - 1) {
            std::cout << "+";
        } else {
            std::cout << "-";
        }
    }
    std::cout << std::endl;
}

void MusicPlayerUI::printSpace() {
    for (int i = 0; i < maxWidth - 1; i++) {
        if (i == 0 || i == maxWidth - 2) {
            std::cout << "|";
        }
        std::cout << " ";
    }
    std::cout << std::endl;
}

void MusicPlayerUI::printSpace(int width) {
    for (int i = 0; i < width - 1; i++) {
        if (i == 0 || i == width - 2) {
            std::cout << "|";
        }
        std::cout << " ";
    }
    std::cout << std::endl;
}

// Return string with left & right border according to maxWidth
std::string MusicPlayerUI::makeString(std::string message, PageAlignment pageAlignment) {
    if (pageAlignment == Left) {
        message = "| " + message;
        size_t length = maxWidth - message.length();
        for (size_t i = 1; i < length; i++) {
            message += " ";
            if (i == length - 1) {
                message += "|";
            }
        }
        return message;
    } else {
        std::string str;
        size_t length = floor(maxWidth/2 - message.length()/2);
        for (size_t i = 1; i < length; i++) {
            if (i == 1) {
                str += "|";
            } else {
                str += " ";
            }
        }

        str += message;

        length = maxWidth - str.length();
        for (size_t i = 1; i < length; i++) {
            str += " ";
            if (i == length - 1) {
                str += "|";
            }
        }
        return str;
    }   
}

// Return string with left & right border according to a custom width
std::string MusicPlayerUI::makeString(std::string message, PageAlignment pageAlignment, int width) {
    if (pageAlignment == Left) {
        message = "| " + message;
        size_t length = width - message.length();
        for (size_t i = 1; i < length; i++) {
            message += " ";
            if (i == length - 1) {
                message += "|";
            }
        }
        return message;
    } else {
        std::string str;
        size_t length = floor(width/2 - message.length()/2);
        for (size_t i = 1; i < length; i++) {
            if (i == 1) {
                str += "|";
            } else {
                str += " ";
            }
        }

        str += message;

        length = width - str.length();
        for (size_t i = 1; i < length; i++) {
            str += " ";
            if (i == length - 1) {
                str += "|";
            }
        }
        return str;
    }   
}

// Helper for the other createNowPlayingList() function;
std::string MusicPlayerUI::createNowPlayingList(std::string message, int width, PageAlignment pageAlignment) {
    message;
    size_t length = width - message.length();
    if (pageAlignment == Left) {
        size_t length = width - message.length();
        for (size_t i = 0; i < length; i++) {
            message += " ";
        }
        return message;
    } else {
        std::string str;
        size_t length = floor(width/2 - message.length()/2);
        for (size_t i = 0; i < length - 1; i++) {
            str += " ";
        }

        str += message;

        length = width - str.length();
        for (size_t i = 0; i < length; i++) {
            str += " ";
        }
        return str;
    }  
}

// Create Now Playing Menu list (the contents)
std::vector<std::string> MusicPlayerUI::createNowPlayingList(std::string currentlyPlayedMusic, std::vector<std::string> queueList) {
    int npMenuMaxWidth = maxWidth - 3; // For counting left & right border
    int npmaxWidth = npMenuMaxWidth * 60/100;
    int quemaxWidth = npMenuMaxWidth - npmaxWidth;
    for (int i = 0; i < queueList.size(); i++) {
        queueList[i] = " " + queueList[i];
    }
    std::vector<std::string> nowPlayingMenu;
    std::string str;

    for (int i = 1; i <= 9; i++) {
        str = "";

        // Now Playing section only
        for (int j = 1; j <= npmaxWidth; j++) { // j starts at 1
            if (i == 1 || i == 9) {
                if (j == 1) {
                    str += " +";
                    j++;
                } else if (j == npmaxWidth) {
                    str += "-+";
                } else {
                    str += "-";
                }
            } else if (i == 4 || i == 6) {
                if (j == 1 || j == npmaxWidth) {
                    str += " |";
                    j++;
                } else {
                    str += " ";
                }
            } else if (i == 7) {
                if (j == 1 || j == npmaxWidth) {
                    str += " |";
                    j++;
                } else if (j == 5) {
                    str += "<< Prev.";
                    j += 7;
                } else  if (j == npmaxWidth - 2 - 7) { // 2 is for counting the space
                    str += "Next >>";
                    j += 6;
                } else if (j == npmaxWidth/2 - 5) {
                    str += "? Play/Pause";
                    j += 11;
                } else {
                    str += " ";
                }
            }  else if (i == 8) {
                if (j == 1 || j == npmaxWidth) {
                    str += " |";
                    j++;
                } else if (j == 5) {
                    str += "[ Sort";
                    j += 5;
                } else  if (j == npmaxWidth - 2 - 9) {
                    str += "Shuffle ]";
                    j += 8;
                } else {
                    str += " ";
                }
            }
        }

        if (i == 1) {
            str += createNowPlayingList("Queue", quemaxWidth, Middle);
        } else if (i == 2) {
            str = " " + makeString("Now Playing", Middle, npmaxWidth);
            str += createNowPlayingList(queueList[0], quemaxWidth, Left);
        } else if (i == 3) {
            str = " " + makeString("-------------", Middle, npmaxWidth);
            str += createNowPlayingList(queueList[1], quemaxWidth, Left);
        } else if (i == 4) {
            for (int a = 0; a < quemaxWidth; a++) {
                str += "-";
            }
        } else if (i == 5) {
            std::string tmp;
            if (currentlyPlayedMusic.length() > npmaxWidth) {
                for (int k = 0; k < npmaxWidth; k++) {
                    tmp += currentlyPlayedMusic[i];
                }
                currentlyPlayedMusic = tmp;
            }
            str = " " + makeString(" " + currentlyPlayedMusic, Middle, npmaxWidth);
            str += createNowPlayingList(queueList[2], quemaxWidth, Left);
        } else if (i == 6) {
            for (int a = 0; a < quemaxWidth; a++) {
                str += "-";
            }
        } else if (i == 7) {
            str += createNowPlayingList(queueList[3], quemaxWidth, Left);
        } else if (i == 8) {
            str += createNowPlayingList(queueList[4], quemaxWidth, Left);
        } else {
            for (int a = 0; a < quemaxWidth; a++) {
                str += " ";
            }
        }

        nowPlayingMenu.insert(nowPlayingMenu.end(), str);
    }
    return nowPlayingMenu;
}

std::string MusicPlayerUI::combineStrip(std::string strip1, std::string strip2) {
    std::string combinedStrip;
    if (strip1.length() < strip2.length()) {
        for (int i = 0; i < strip1.length(); i++) {
            combinedStrip += (strip1[i] == '+' || strip2[i] == '+') ? "+" : "-";
        }
    } else {
        for (int i = 0; i < strip1.length(); i++) {
            combinedStrip += (strip1[i] == '+' || strip2[i] == '+') ? "+" : "-";
        }
    }
    return combinedStrip;
}

// Create Navigation Menu
std::vector<std::string> MusicPlayerUI::makeSubMenuNav(std::vector<std::string> nav, int width) {
    std::vector<std::string> SubMenuNav;
    std::string subMenuNav, subMenuNavStrip, nav0strip, nav1strip, nav2strip;

    for (int i = 0; i < nav[0].length(); i++) {
        nav0strip += (i == 0 || i == nav[0].length() - 1) ? "+" : "-";
    }

    for (int i = 0; i < nav[1].length(); i++) {
        nav1strip += (i == 0 || i == nav[1].length() - 1) ? "+" : "-";
    }

    if (nav.size() == 3) {
        for (int i = 0; i < nav[2].length(); i++) {
            nav2strip += (i == 0 || i == nav[2].length() - 1) ? "+" : "-";
        }}
    

    if (nav.size() == 2) {
        int i = 0;
        while (i < width) {
            if (i == 0) {
                subMenuNav += nav[0];
                subMenuNavStrip += nav0strip;
                i += nav[0].length();
            } else if (i == width - nav[1].length()) {
                subMenuNav += nav[1];
                subMenuNavStrip += nav1strip;
                i += nav[1].length();
            } else {
                subMenuNav += " ";
                subMenuNavStrip += "-";
                i++;
            }
        }

        SubMenuNav.emplace_back(subMenuNavStrip);
        SubMenuNav.emplace_back(subMenuNav);
        SubMenuNav.emplace_back(subMenuNavStrip);
    } else if (nav.size() == 3) {
        int i = 0;
        while (i < width) {
            if (i == 0) {
                subMenuNav += nav[0];
                subMenuNavStrip += nav0strip;
                i += nav[0].length();
            } else if (i == width - nav[2].length()) {
                subMenuNav += nav[2];
                subMenuNavStrip += nav2strip;
                i += nav[2].length();
            } else if (i == (width / 2 - nav[1].length() / 2)) {
                subMenuNav += nav[1];
                subMenuNavStrip += nav1strip;
                i += nav[1].length();
            } else {
                subMenuNav += " ";
                subMenuNavStrip += "-";
                i++;
            }
        }

        SubMenuNav.emplace_back(subMenuNavStrip);
        SubMenuNav.emplace_back(subMenuNav);
        SubMenuNav.emplace_back(subMenuNavStrip);
    }
    return SubMenuNav;
}

// Print out Menu (Interface/UI) to console
void MusicPlayerUI::printMainMenu(std::vector<std::string> list, MenuType menuType) {
    std::string upperStrip, blankLine, lowerStrip, lowerOption;
    upperStrip = blankLine = lowerStrip = lowerOption = "";

    for (int i = 0; i < maxWidth; i++) {
        if (i == 0 || i == maxWidth - 1) {
            upperStrip += "+";
            blankLine += "|";
        } else {
            upperStrip += "-";
            blankLine += " ";
        }

    }
    
    for (int i = 0; i < maxWidth; i++) {
        if (i == 0 || i == maxWidth - 1) {
            lowerOption += "|";
            lowerStrip += "+";
            continue;
        }
        
        if (i == maxWidth - 11) {
            lowerOption += "| 0. Exit ";
            i += 9; // 9 is the number of character of "| 0. Exit " minus 1
            lowerStrip += "+";
            for (int j = 0; j < 9; j++) {
                lowerStrip += "-";
            }
            continue;
        }

        // Print lower border option based on position (type of menu)
        if (menuType == NowPlayingMenu) {
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
        } else if (menuType == LibraryMenu) {
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
    if (menuType == NowPlayingMenu) {
        for (int i = 0; i < list.size(); i++) {
            std::cout << "|" << list[i] << "|" << std::endl;
        }
    } else {
        for (int i = 0; i < list.size(); i++) {
            if ( i == 0 || i == 1) {
            std::cout << makeString(list[i], Middle) << std::endl;
            } else {
                std::cout << makeString(list[i], Left) << std::endl;
            }
        }
        std::cout << blankLine << std::endl;
    }
    std::cout << lowerStrip << std::endl << lowerOption << std::endl << lowerStrip << std::endl;
    std::cout << "| Input: ";
}

// Return the last strip (separator) and the width of the contents
std::vector<std::string> MusicPlayerUI::ShowAllSongs(std::vector<Music*> allMusic) {
    std::vector<std::string> stripAndWidth;
    // Find longest title length
    int longestTitle = 0, longestArtist = 0, width = 0; // longest content length;
    std::string header1 = "No.", header2 = "Title", header3 = "Artist";
    std::string contentSeparator;

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

    contentSeparator = "+-";
    contentSeparator += "-----+-";
    for (int i = 0; i < longestTitle; i++) {
        contentSeparator += "-";
    }
    contentSeparator += "-+-";
    for (int i = 0; i < longestArtist; i++) {
        contentSeparator += "-";
    }
    contentSeparator += "-+";

    width = 2 + header1.length() + 1 + 3 + longestTitle + 3 + longestArtist + 2;

    // Print header
    std::cout << contentSeparator << std::endl
    << std::setfill(' ')  << "| " << std::setw(4) << righted(header1) << " | " << std::setw(longestTitle) << centered(header2) << " | " << std::setw(longestArtist) << centered(header3) << " |" << std::endl
    << contentSeparator << std::endl;

    // Print all songs
    for (int i = 0; i < allMusic.size(); i++) {
        std::string iter = std::to_string(i + 1) + ".";
        std::string title = allMusic[i]->title, artist = allMusic[i]->artist;
        std::cout << "| "<< std::setw(4) << righted(iter) << " | " << std::setw(longestTitle) << lefted(title) << " | " << std::setw(longestArtist) << lefted(artist) << " |" <<std::endl;  
    }

    stripAndWidth.emplace_back(contentSeparator);
    stripAndWidth.emplace_back(std::to_string(width));
    return stripAndWidth;
}

void MusicPlayerUI::AddSongs(int maxWidth, std::string directory, int songFound, int songAdded, int errorCount) {

}

void clearScreen() {
    try {
        system("cls");
    } catch (std::invalid_argument) {
        system("clear");
    }
}

void printConfirm(std:: string menuName) {
    std::cout << "Press any key to go back to the " << menuName << std::endl;
    getch();
    clearScreen();
}


