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
        void addMusic(std::string title, std::string artist, std::string dir);
        void playOrPausedCurrentMusic();
        std::string getCurrentMusic();
};

std::string convertPath(std::string path);