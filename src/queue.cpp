#ifndef MUSIC_QUEUE_H_
#define MUSIC_QUEUE_H_

#include <iostream>
#include <string>
#include <windows.h>
#include <algorithm>
#include <filesystem>
#include <thread>

#include "music_ui.hpp"
#include "queue.hpp"

Music::Music(std::string Title, std::string Artist, std::string Path) {
        title = Title;
        artist = Artist;
        path = Path;
        playStatus = notPlaying;
    }

MusicQueue::MusicQueue() {
        queueFront = nullptr;
        queueRear = nullptr;
        queueCurrent = nullptr;
}

void MusicQueue::addMusic(std::string title, std::string artist, std::string path) {
    Music* newMusic = new Music(title, artist, path);
    if (queueRear == nullptr) {
        queueFront = queueRear = queueCurrent = newMusic;
    }
    else {
        queueRear->next = newMusic; // queueRear hasn't been updated yet
        queueRear->next->prev = queueRear;
        queueRear = newMusic; // queueRear updated
    }
}

void MusicQueue::playOrPausedCurrentMusic() {
    if (queueFront == nullptr) {
        return;
    } else {
        if (queueCurrent->playStatus == notPlaying) {
            std::string path = convertPath(queueCurrent->path);

            // Convert std::string to std::wstring
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            std::wstring pathConverted = converter.from_bytes(path);
            std::wstring finalPath = L"open \"" + pathConverted + L"\" type mpegvideo alias mp3";

            mciSendStringW(finalPath.c_str(), NULL, 0, NULL);
            mciSendStringW(L"play mp3", NULL, 0, NULL);
            queueCurrent->playStatus = playing;

            // Check when the music has finished playing
            std::thread checkMusicThread([this]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Optional delay
                while (true) {
                    wchar_t  status[128] = {};
                    if (mciSendStringW(L"status mp3 mode", status, sizeof(status), NULL) != 0) {
                        std::cerr << "Failed to get status: " << GetLastError() << std::endl;
                    }
                    
                    // Output raw status
                    std::wcout << L"Status: " << status << std::endl;

                    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
                    std::string utf8Str = converter.to_bytes(status);
                    if (strcmp(utf8Str.c_str(), "stopped") == 0) {
                        // Music has finished playing
                        queueCurrent->playStatus = notPlaying;
                        mciSendStringW(L"close mp3", NULL, 0, NULL);
                        if (queueCurrent->next != nullptr) {
                            queueCurrent = queueCurrent->next;
                            playOrPausedCurrentMusic(); // Play the next music
                        } else {
                            std::cout << "End of the queue." << std::endl;
                        }
                        break;
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Check every second
                }
            });
            checkMusicThread.detach(); // Detach thread to run asynchronously

        } else if(queueCurrent->playStatus == paused) {
            mciSendStringW(L"resume mp3", NULL, 0, NULL);
            queueCurrent->playStatus = playing;
        } else {
            mciSendStringW(L"pause mp3", NULL, 0, NULL);
            queueCurrent->playStatus = paused;
        }
    }
}

// Return "Artist - Title" of the music that's currently in queue
std::string MusicQueue::getCurrentMusic() {
    return queueCurrent->artist + " - " + queueCurrent->title;
}


// Convert single backslash in windows path to double backslash
std::string convertPath(std::string path) {
    std::string convertedPath;

    for (char c : path) {
        if (c == '\\') {
            convertedPath += "\\\\";
        } else {
            convertedPath += c;
        }
    }
    return convertedPath;
}

#endif // From ifndef