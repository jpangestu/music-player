#include "playlist.hpp"

MusicPlaylist::MusicPlaylist(const std::string& name) : name(name) {}

void MusicPlaylist::addSong(const std::string& song) {
    songs.push_back(song);
}

void MusicPlaylist::removeSong(const std::string& song) {
    songs.erase(std::remove(songs.begin(), songs.end(), song), songs.end());
}

std::vector<std::string> MusicPlaylist::getSongs() const {
    return songs;
}

std::string MusicPlaylist::getName() const {
    return name;
}

void MusicPlaylist::setName(const std::string& newName) {
    name = newName;
}
