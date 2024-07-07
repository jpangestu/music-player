#ifndef MUSIC_QUEUE_H_
#define MUSIC_QUEUE_H_

#include <iostream>
#include <string>
#include <windows.h>
#include <algorithm>
#include <filesystem>

#include "music_ui.hpp"
#include "queue.hpp"

MusicQueue::addMusic(std::string title, std::string artist, std::string dir) {
    Music* newNode = new Music(title, artist, dir);
    if (rear == nullptr) {
        front = rear = newNode;
    }
    else {
        rear->next = newNode;
        rear = newNode;
    }
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