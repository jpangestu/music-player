#ifndef PLAYLIST_H_
#define PLAYLIST_H_

#include <vector>
#include <string>
#include <algorithm>

class MusicPlaylist {
private:
    std::string name;
    std::vector<std::string> songs;

public:
    MusicPlaylist(const std::string& name);
    void addSong(const std::string& song);
    void removeSong(const std::string& song);
    std::vector<std::string> getSongs() const;
    std::string getName() const;
    void setName(const std::string& newName);
};

#endif // PLAYLIST_HPP
