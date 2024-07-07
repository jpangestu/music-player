#ifndef MUSIC_UI_H_
#define MUSIC_UI_H_

#include <iostream>
#include <string>
#include <vector>
#include <conio.h> // getch()
#include <cmath> // floor()

#include "music_ui.hpp"

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
                lowerOption += "8. Playlist | 9. Library |";
                i += 26;

                for (int j = 0; j < 26; j++) {
                    if (j == 12 || j == 25) {
                        lowerStrip+= "+";
                    } else {
                        lowerStrip += "-";
                    }
                }
            }
        } else if (menuType == Library) {
            if (i == 2) {
                lowerOption += "8. Now Playing | 9. Playlist |";
                i += 30;
                
                for (int j = 0; j < 30; j++) {
                    if (j == 15 || j == 29) {
                        lowerStrip+= "+";
                    } else {
                        lowerStrip += "-";
                    }
                }
            }
        } else {
            if (i == 2) {
                lowerOption += "8. Library | 9. Now Playing |";
                i += 29;
                
                for (int j = 0; j < 29; j++) {
                    if (j == 11 || j == 28) {
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
    maxLength -= 2; // For counting left & right border
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
                if (j == 1 || j == npMaxLength) {
                    str += "+";
                } else {
                    str += "-";
                }
            } else if (i == 4 || i == 6) {
                if (j == 1 || j == npMaxLength) {
                    str += "|";
                } else {
                    str += " ";
                }
            } else if (i == 7) {
                if (j == 1 || j == npMaxLength) {
                    str += "|";
                } else if (j == 4) {
                    str += "< Prev.";
                    j += 6;
                } else  if (j == npMaxLength - 2 - 6) { // 2 is for counting the space
                    str += "Next >";
                    j += 5;
                } else if (j == npMaxLength/2 - 5) {
                    str += "? Play/Pause";
                    j += 11;
                } else {
                    str += " ";
                }
            }  else if (i == 8) {
                if (j == 1 || j == npMaxLength) {
                    str += "|";
                } else if (j == 4) {
                    str += "6. Shuffle";
                    j += 9;
                } else  if (j == npMaxLength - 2 - 7) {
                    str += "7. Sort";
                    j += 6;
                } else {
                    str += " ";
                }
            }
        }

        if (i == 1) {
            str += printNowPlaying("Queue", queMaxLength, Middle);
        } else if (i == 2) {
            str = printUI("Now Playing", npMaxLength, Middle);
            str += printNowPlaying(queueList[0], queMaxLength, Left);
        } else if (i == 3) {
            str = printUI("-------------", npMaxLength, Middle);
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
            str = printUI(currentlyPlayedMusic, npMaxLength, Middle);
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


#endif // From ifndef