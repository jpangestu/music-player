// Playlist class using linked list (queue)
class Queue {
private:
    Music* front;  // Front of the playlist
    Music* rear;   // Rear of the playlist
    Music* nowPlaying; // Currently playing song

public:
    Queue() : front(nullptr), rear(nullptr), nowPlaying(nullptr) {}

    // Function to add a song to the playlist (enqueue)
    void addSong(std::string title, std::string artist, std::string dir) {
        Music* newNode = new Music(title, artist, dir);
        if (rear == nullptr) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }
        std::cout << "Added song: " << title << " (by " << artist << ")" << std::endl;
    }

    // Function to delete a song from the playlist by index
    void deleteSong(int index) {
        if (front == nullptr) {
            std::cout << "Playlist is empty. No songs to delete.\n";
            return;
        }

        Music* current = front;
        Music* previous = nullptr;
        int currentIndex = 0;

        // Traverse the list to find the song at the given index
        while (current != nullptr && currentIndex != index) {
            previous = current;
            current = current->next;
            currentIndex++;
        }

        if (current == nullptr) {
            std::cout << "Invalid index. No song found at index " << index << ".\n";
            return;
        }

        // Remove the song from the playlist
        if (current == front) {
            front = front->next;
        }
        else {
            previous->next = current->next;
            if (current == rear) {
                rear = previous;
            }
        }

        std::cout << "Deleted song at index " << index << ": " << current->title << " (by " << current->artist << ")\n";
        delete current;
    }

    void playCurrentSong(bool paused) {
        if (front == nullptr) {
            std::cout << "Queue is empty. Add songs first.\n";
            return;
        } else {
            if (paused == false) {
                mciSendStringW(L"pause mp3", NULL, 0, NULL);

            } else {   
                mciSendStringW(L"resume mp3", NULL, 0, NULL);
            }
        }
    }

    // Function to play the next song in the playlist (dequeue)
    void playNextSong() {
        if (front == nullptr) {
            std::cout << "Queue is empty. Add songs first.\n";
            return;
        }
        if (nowPlaying != nullptr) {
            mciSendStringW(L"stop mp3", NULL, 0, NULL);
            mciSendStringW(L"close mp3", NULL, 0, NULL);
        }
        nowPlaying = front;
        front = front->next;

        std::string path = convertPath(nowPlaying->dir);
        std::cout << nowPlaying->dir << std::endl;

         // Convert std::string to std::wstring
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring pathConverted = converter.from_bytes(path);

        std::wstring finalPath = L"open \"" + pathConverted + L"\" type mpegvideo alias mp3";

        mciSendStringW(finalPath.c_str(), NULL, 0, NULL);
        mciSendStringW(L"play mp3", NULL, 0, NULL);

        std::cout << "Now playing: " << nowPlaying->title << " (by " << nowPlaying->artist << ")\n";
    }

    // Function to shuffle the playlist
    void shuffleQueue() {
        // Implementing Fisher-Yates shuffle algorithm
        if (front == nullptr) {
            std::cout << "Playlist is empty. Add songs first.\n";
            return;
        }

        // Convert linked list to array for easier shuffling
        Music* current = front;
        int length = 0;
        while (current != nullptr) {
            length++;
            current = current->next;
        }

        Music** queueArray = new Music * [length];
        current = front;
        int i = 0;
        while (current != nullptr) {
            queueArray[i++] = current;
            current = current->next;
        }

        // Shuffle the array
        srand(time(0));  // Seed for randomness
        for (int i = length - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            std::swap(queueArray[i], queueArray[j]);
        }

        // Reconstruct the playlist with shuffled order
        front = queueArray[0];
        rear = queueArray[length - 1];
        rear->next = nullptr;
        for (int i = 0; i < length - 1; i++) {
            queueArray[i]->next = queueArray[i + 1];
        }

        delete[] queueArray;
    }


    // Function to display the current playlist
    void displayPlaylist() {
        if (front == nullptr) {
            std::cout << "Queue is empty.\n";
            return;
        }
        std::cout << "Queue:\n";
        Music* current = front;
        int index = 0;
        while (current != nullptr) {
            std::cout << index << ". " << current->title << " (by " << current->artist << ")\n";
            current = current->next;
            index++;
        }
    }

    // Function to display the currently playing song
    void displayNowPlaying() {
        if (nowPlaying == nullptr) {
            std::cout << "No song is currently playing...";
        }
        else {
            std::cout << "Now playing: " << nowPlaying->title << " (by " << nowPlaying->artist << ")";
        }
    }

    std::string musicInQue() {
        if (nowPlaying == nullptr) {
            return "";
        }
        else {
            std::string tmp = nowPlaying->title + " - " + nowPlaying->artist;
            return tmp;
        }
    }

    // Destructor to free memory
    ~Queue() {
        Music* current = front;
        while (current != nullptr) {
            Music* temp = current;
            current = current->next;
            delete temp;
        }
        if (nowPlaying != nullptr) {
            delete nowPlaying;
        }
    }
};