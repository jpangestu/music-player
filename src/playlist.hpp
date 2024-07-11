#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP

#include <vector>
#include <string>
#include <algorithm> // For std::remove

class Playlist {
private:
    std::string name;
    std::vector<std::string> songs;

public:
    Playlist(const std::string& name);
    void addSong(const std::string& song);
    void removeSong(const std::string& song);
    std::vector<std::string> getSongs() const;
    std::string getName() const;
    void setName(const std::string& newName);
};

#endif // PLAYLIST_HPP
