#ifndef PLAYLIST_H_
#define PLAYLIST_H_

#include <vector>
#include <string>
#include <algorithm>

#include "queue.hpp"

struct Playlist {
    std::string name;
    std::vector<Music*> songs;

    Playlist(std::string Name);
};

class MusicPlaylist {
private:
    Playlist* playlist;
public:
    MusicPlaylist();
    void addPlaylist(std::string name);
    void addSong(Music* song);
    void removeSong(Music* song);
    std::vector<Music*> getSongs() const;
    std::string getName() const;
    void setName(const std::string& newName);
};

#endif // PLAYLIST_HPP
