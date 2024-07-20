#include "playlist.hpp"

Playlist::Playlist(std::string Name) {
    name = Name;
}

MusicPlaylist::MusicPlaylist() {
    playlist->songs.clear();
}

void MusicPlaylist::addPlaylist(std::string name){
    playlist = new Playlist(name);
}

void MusicPlaylist::addSong(Music* song) {
    playlist->songs.emplace_back(song);
}

void MusicPlaylist::removeSong(Music* song) {
    playlist->songs.erase(std::remove(playlist->songs.begin(), playlist->songs.end(), song), playlist->songs.end());
}

std::vector<Music*> MusicPlaylist::getSongs() const {
    return playlist->songs;
}

std::string MusicPlaylist::getName() const {
    return playlist->name;
}

void MusicPlaylist::setName(const std::string& newName) {
    playlist->name = newName;
}
