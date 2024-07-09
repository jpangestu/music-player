#ifndef MUSIC_QUEUE_H_
#define MUSIC_QUEUE_H_

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <algorithm> // sort
#include <filesystem>
#include <thread>
#include <cmath> // floor

enum PlayStatus {
    notPlaying,
    playing,
    paused
};

struct Music
{
    std::string title;
    std::string artist;
    std::string path; // Directory of the music file
    PlayStatus playStatus;
    Music* next;
    Music* prev;

    Music(std::string Title, std::string Artist, std::string Path);
};

class MusicQueue
{
    private:
        Music* queueFront;  // Front of the queue
        Music* queueRear;   // Rear of the queue
        Music* queueCurrent; // Currently in queue (ready to be played)
    public:
        MusicQueue();
        void addMusic(Music* newMusic);
        void playOrPausedCurrentMusic();
        std::string getCurrentMusic();
        void playNextMusic();
        void playPrevMusic();
        void setQueueToCircular();
        std::vector<std::string> getQueueList(int maxLength);
};

std::string convertPath(std::string path);
bool compareMusicByTitle(const Music* a, const Music* b);


#endif // From ifndef