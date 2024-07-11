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

// Add new Music (struct) node
void MusicQueue::addMusic(Music* newMusic) {
    if (queueRear == nullptr) {
        queueFront = queueRear = queueCurrent = newMusic;
    }
    else {
        queueRear->next = newMusic; // queueRear hasn't been updated yet
        queueRear->next->prev = queueRear;
        queueRear = queueRear->next; // queuRear updated
    }
}

// Return all music that matched the pattern
std::vector<Music*> MusicQueue::searchMusic(std::vector<Music*> queue, std::string pattern) {
    std::vector<Music*> matchedMusic;
    // Change pattern to lowercase for case insensitive matching
    std::transform(pattern.begin(), pattern.end(), pattern.begin(), [](unsigned char c){ return std::tolower(c); });
    for (int i = 0; i < queue.size(); i++) {
        std::string str = queue[i]->title;
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return std::tolower(c); });
        size_t index = str.find(pattern);
        if (index != std::string::npos) { // return value of find() if pattern not found is std::string::npos (aka -1)
            matchedMusic.insert(matchedMusic.end(), queue[i]);
        }
    }
    return matchedMusic;
}

void MusicQueue::removeMusic(std::vector<Music*> allMusic, Music* musicToDelete) {
    if (allMusic.size() == 1) {
        allMusic.clear();
        queueCurrent = queueFront = queueRear = nullptr;
    } else if (allMusic.size() == 2) {
        int index = 0;
        for (std::vector<Music*>::iterator it = allMusic.begin(); it != allMusic.end(); ++it) {
            if (musicToDelete == allMusic[index]) {
                allMusic.erase(it);
                queueRear = queueCurrent;
                queueCurrent = queueFront;
                allMusic[index]->next = allMusic[index];
                allMusic[index]->prev = allMusic[index];
            }
        }
    } else {
        int index = 0; // index = iterator
        for (std::vector<Music*>::iterator it = allMusic.begin(); it != allMusic.end(); ++it) {
            if (musicToDelete == allMusic[index]) {
                Music* tmp;
                if (musicToDelete == queueFront) {
                    queueFront = queueFront->next;
                } else if (musicToDelete == queueRear) {
                    queueRear = queueRear->prev;
                } else if (musicToDelete == queueCurrent) {
                    queueCurrent = queueCurrent->next;
                } else {
                    allMusic[index]->prev->next = allMusic[index]->next;
                    allMusic[index]->next->prev = allMusic[index]->prev;
                    allMusic.erase(it);
                }
            }
            index++;
        }
    }
}

// Return true if a music already exists in library, false otherwise
bool MusicQueue::musicInLibrary(std::vector<Music*> allMusic, std::string musicPath) {
    if (allMusic.size() == 0) {
        return false;
    }

    for (int i = 0; i < allMusic.size(); i++) {
        if (musicPath == allMusic[i]->path) {
            return true;
        }
    }

    return false;
}

// Create thread for checking if a music is finished playing. If yes, play next song in queue.
void MusicQueue::autoPlayNextMusic() {
    std::thread playInBack(&MusicQueue::autoPlayNextMusicDetails, this);
    if (playInBack.joinable()) {
        playInBack.detach();
    }
}

// Implementation details for autoPlayNextMusic()
void MusicQueue::autoPlayNextMusicDetails() {
    // Wait for notification that playback has finished
    char status[128];
    do {
        std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // Sleep briefly to avoid excessive CPU usage
        mciSendStringA("status mp3 mode", status, sizeof(status), nullptr);
        std::cout << status << std::endl;
    } while (strcmp(status, "stopped") != 0);

    queueCurrent->playStatus = notPlaying;
    mciSendStringW(L"close mp3", NULL, 0, NULL);
    queueCurrent = queueCurrent->next;
    playOrPauseCurrentMusic();
}

// Play or paused music based on the playback status
void MusicQueue::playOrPauseCurrentMusic() {
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
            DWORD_PTR instance = (DWORD_PTR)this;
            mciSendStringW(L"play mp3 notify", NULL, 0, NULL);
            queueCurrent->playStatus = playing;
        } else if(queueCurrent->playStatus == paused) {
            mciSendStringW(L"resume mp3", NULL, 0, NULL);
            queueCurrent->playStatus = playing;
        } else {
            mciSendStringW(L"pause mp3", NULL, 0, NULL);
            queueCurrent->playStatus = paused;
        }
    }
}

// Return Artist and Title of the music that's currently in queue (queueCurrent)
// Return value: vector[1] = title, vector[2] = artist
std::string MusicQueue::getCurrentMusic() {
    if (queueCurrent == nullptr) {
        return "";
    }
    return queueCurrent->title + " - " + queueCurrent->artist;
}

void MusicQueue::setCurrentMusic(int position) {
    if (position == 0) {
        mciSendStringW(L"close mp3", NULL, 0, NULL);
        queueCurrent->playStatus = notPlaying;
    } else if (position < 0) {
        for (int i = position; i < 0; i++) {
            mciSendStringW(L"close mp3", NULL, 0, NULL);
            queueCurrent->playStatus = notPlaying;
            queueCurrent = queueCurrent->prev;
        }
    } else {
        for (int i = position; i > 0; i--) {
            mciSendStringW(L"close mp3", NULL, 0, NULL);
            queueCurrent->playStatus = notPlaying;
            queueCurrent = queueCurrent->next;
        }
    }
}

void MusicQueue::setCurrentMusic(std::vector<Music*> queue, std::string musicPath) {
    for (int i = 0; i < queue.size(); i++) {
        if (musicPath == queue[i]->path) {
            mciSendStringW(L"close mp3", NULL, 0, NULL);
            queueCurrent->playStatus = notPlaying;
            queueCurrent = queue[i];
        }
    }
}

// Play next music in queue
void MusicQueue::playNextMusic() {
    if (queueFront == nullptr) {
        return;
    } else {
        mciSendStringW(L"close mp3", NULL, 0, NULL);
        queueCurrent->playStatus = notPlaying;
        queueCurrent = queueCurrent->next;
        playOrPauseCurrentMusic();
    }
}

// Play previous music in queue
void MusicQueue::playPrevMusic() {
    if (queueFront == nullptr) {
        return;
    } else {
        mciSendStringW(L"close mp3", NULL, 0, NULL);
        queueCurrent->playStatus = notPlaying;
        queueCurrent = queueCurrent->prev;
        playOrPauseCurrentMusic();
    }
}

// Set the doubly linked list of struct Music circular to mimic Repeat All feature
void MusicQueue::setQueueToCircular() {
    queueFront->prev = queueRear;
    queueRear->next = queueFront;
}

// Return list of music in queue (in vector of string)
std::vector<std::string> MusicQueue::getQueueList(int maxLength) {
    Music* originalCurrentMusic = queueCurrent;
    std::vector<std::string> queueList;

    if (queueCurrent == nullptr) {
        for (int i = 0; i < maxLength; i++) {
            queueList.insert(queueList.end(), "");
        }
        return queueList;
    }

    /* Set queuCurrent to select the previous song first because the currently played
    music should be in the middle of the list (not in the beginning of the list)
    */
    for (int i = 0; i < floor(maxLength/2); i++) {
        queueCurrent = queueCurrent->prev;
    }

    // Save song title to the queueList
    for (int i = 0; i < maxLength; i++) {
        queueList.insert(queueList.end(), std::to_string(i + 1) + ". " + queueCurrent->title);
        queueCurrent = queueCurrent->next;
    }

    queueCurrent = originalCurrentMusic;
    return queueList;
}

void MusicQueue::sort(std::vector<Music*> currentQueue) {
    Music * currentMusic = queueCurrent;
    std::sort(std::begin(currentQueue), std::end(currentQueue), compareMusicByTitle);

    for (int i = 0; i < currentQueue.size(); i++) {      
        // Reset queue
        if (i == 0) {
            queueCurrent = nullptr;
            queueFront = nullptr;
            queueRear = nullptr;
        }
        addMusic(currentQueue[i]);
    }
    queueCurrent = currentMusic;
    setQueueToCircular();
}

void MusicQueue::shuffle(std::vector<Music*> currentQueue) {
    Music * currentMusic = queueCurrent;
    auto rd = std::random_device {}; 
    auto rng = std::default_random_engine { rd() };
    std::shuffle(std::begin(currentQueue), std::end(currentQueue), rng);

    for (int i = 0; i < currentQueue.size(); i++) {      
        // Reset queue
        if (i == 0) {
            queueCurrent = nullptr;
            queueFront = nullptr;
            queueRear = nullptr;
        }
        addMusic(currentQueue[i]);
    }
    queueCurrent = currentMusic;
    setQueueToCircular();
}

// Convert single backslash in windows path to forward backslash
std::string convertPath(std::string path) {
    std::string convertedPath;

    for (char c : path) {
        if (c == '\\') {
            convertedPath += "/";
        } else {
            convertedPath += c;
        }
    }
    return convertedPath;
}

// For std::sort (from <algorithm>) 3rd parameter
bool compareMusicByTitle(const Music* a, const Music* b) {
    return a->title < b->title;
}