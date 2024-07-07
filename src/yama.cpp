#include <iostream>
#include <ctime>    // For srand and rand
#include <cstdlib>  // For srand and rand

using namespace std;

// Node structure for the linked list
struct Node {
    string songName;
    string singer;
    Node* next;

    Node(string name, string singer) : songName(name), singer(singer), next(nullptr) {}
};

// Playlist class using linked list (queue)
class Playlist {
private:
    Node* front;  // Front of the playlist
    Node* rear;   // Rear of the playlist
    Node* nowPlaying; // Currently playing song

public:
    Playlist() : front(nullptr), rear(nullptr), nowPlaying(nullptr) {}

    // Function to add a song to the playlist (enqueue)
    void addSong(string songName, string singer) {
        Node* newNode = new Node(songName, singer);
        if (rear == nullptr) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }
        cout << "Added song: " << songName << " (by " << singer << ")" << endl;
    }

    // Function to delete a song from the playlist by index
    void deleteSong(int index) {
        if (front == nullptr) {
            cout << "Playlist is empty. No songs to delete.\n";
            return;
        }

        Node* current = front;
        Node* previous = nullptr;
        int currentIndex = 0;

        // Traverse the list to find the song at the given index
        while (current != nullptr && currentIndex != index) {
            previous = current;
            current = current->next;
            currentIndex++;
        }

        if (current == nullptr) {
            cout << "Invalid index. No song found at index " << index << ".\n";
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

        cout << "Deleted song at index " << index << ": " << current->songName << " (by " << current->singer << ")\n";
        delete current;
    }

    // Function to play the next song in the playlist (dequeue)
    void playNextSong() {
        if (front == nullptr) {
            cout << "Playlist is empty. Add songs first.\n";
            return;
        }
        if (nowPlaying != nullptr) {
            delete nowPlaying;
            nowPlaying = nullptr;
        }
        nowPlaying = front;
        front = front->next;
        cout << "Now playing: " << nowPlaying->songName << " (by " << nowPlaying->singer << ")\n";
    }

    // Function to shuffle the playlist
    //Code fix
// Function to shuffle the playlist
    void shufflePlaylist() {
        // Implementing Fisher-Yates shuffle algorithm
        if (front == nullptr) {
            cout << "Playlist is empty. Add songs first.\n";
            return;
        }

        // Convert linked list to array for easier shuffling
        Node* current = front;
        int length = 0;
        while (current != nullptr) {
            length++;
            current = current->next;
        }

        Node** playlistArray = new Node * [length];
        current = front;
        int i = 0;
        while (current != nullptr) {
            playlistArray[i++] = current;
            current = current->next;
        }

        // Shuffle the array
        srand(time(0));  // Seed for randomness
        for (int i = length - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            swap(playlistArray[i], playlistArray[j]);
        }

        // Reconstruct the playlist with shuffled order
        front = playlistArray[0];
        rear = playlistArray[length - 1];
        rear->next = nullptr;
        for (int i = 0; i < length - 1; i++) {
            playlistArray[i]->next = playlistArray[i + 1];
        }

        delete[] playlistArray;

        cout << "Playlist shuffled.\n";
    }


    // Function to display the current playlist
    void displayPlaylist() {
        if (front == nullptr) {
            cout << "Playlist is empty.\n";
            return;
        }
        cout << "Current Playlist:\n";
        Node* current = front;
        int index = 0;
        while (current != nullptr) {
            cout << index << ". " << current->songName << " (by " << current->singer << ")\n";
            current = current->next;
            index++;
        }
    }

    // Function to display the currently playing song
    void displayNowPlaying() {
        if (nowPlaying == nullptr) {
            cout << "No song is currently playing...";
        }
        else {
            cout << "Now playing: " << nowPlaying->songName << " (by " << nowPlaying->singer << ")";
        }
    }

    // Destructor to free memory
    ~Playlist() {
        Node* current = front;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        if (nowPlaying != nullptr) {
            delete nowPlaying;
        }
    }
};

int main() {
    Playlist myPlaylist;
    int choice;
    string songName, singer;
    int indexToDelete;

    do {
        cout << "\n--- Music Playlist Menu ---\n";
        cout << "================================\n";
        cout << "|"; myPlaylist.displayNowPlaying();cout << "| ";
        cout << " \n================================\n";
        cout << "1. Add a song\n";
        cout << "2. Delete a song\n";
        cout << "3. Play next song\n";
        cout << "4. Shuffle playlist\n";
        cout << "5. Display playlist\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter song name: ";
            cin.ignore(); // Ignore newline character left by previous cin
            cin >> songName;
            cout << "Enter singer: ";
            cin >> singer;
            myPlaylist.addSong(songName, singer);
            break;
        case 2:
            cout << "Enter index of song to delete: ";
            cin >> indexToDelete;
            myPlaylist.deleteSong(indexToDelete);
            break;
        case 3:
            myPlaylist.playNextSong();
            break;
        case 4:
            myPlaylist.shufflePlaylist();
            break;
        case 5:
            myPlaylist.displayPlaylist();
            break;
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Please enter a valid option.\n";
        }
    } while (choice != 0);

    return 0;
}
