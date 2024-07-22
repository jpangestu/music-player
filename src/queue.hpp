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
#include <random> // random device and random engine
#include <future>
#include <codecvt>

// Playback Status
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
    PlayStatus playStatus; // Playback Status
    Music* next;
    Music* prev;

    // Constructor
    Music(std::string Title, std::string Artist, std::string Path);
};

struct Playlist {
    std::string name;
    std::vector<Music*> songs;

    Playlist(std::string Name);
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
        std::vector<Music*> searchMusic(std::vector<Music*> queue, std::string);
        bool musicInLibrary(std::vector<Music*> allMusic, std::string title, std::string artist);
        std::vector<Music*> removeMusic(std::vector<Music*> allMusic, Music* musicToDelete);
        void playOrPauseCurrentMusic();
        void autoPlayNextMusicDetails();
        void autoPlayNextMusic();
        std::string getCurrentMusic();
        void setCurrentMusic(int position);
        void setCurrentMusic(std::vector<Music*> queue, std::string musicPath);
        void playNextMusic();
        void playPrevMusic();
        void setQueueToCircular();
        std::vector<std::string> getQueueList(int maxLength);
        void sort(std::vector<Music*> currentQueue);
        void shuffle(std::vector<Music*> currentQueue);
};

std::string convertPath(std::string path);
bool compareMusicByTitle(const Music* a, const Music* b);
void monitorPlaybackStatus();


#endif // From ifndef