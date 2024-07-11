#include "playlist.hpp"

Playlist::Playlist(const std::string& name) : name(name) {}

void Playlist::addSong(const std::string& song) {
    songs.push_back(song);
}

void Playlist::removeSong(const std::string& song) {
    songs.erase(std::remove(songs.begin(), songs.end(), song), songs.end());
}

std::vector<std::string> Playlist::getSongs() const {
    return songs;
}

std::string Playlist::getName() const {
    return name;
}

void Playlist::setName(const std::string& newName) {
    name = newName;
}
