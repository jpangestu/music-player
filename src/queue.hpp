struct Music
{
    std::string title;
    std::string artist;
    std::string path; // Directory of the music file
    Music* next;
    Music* prev;

    Music(std::string Title, std::string Artist, std::string Path) {
        title = Title;
        artist = Artist;
        path = Path;
    }
};

class MusicQueue
{
    private:
        Music* queueFront;  // Front of the queue
        Music* queueRear;   // Rear of the queue
        Music* queueCurrent; // Currently in queue (ready to be played)
    public:
        void addMusic();

};

class Queue;
std::string convertPath(std::string path);