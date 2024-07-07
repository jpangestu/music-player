g++ src\main.cpp src\interface.cpp -o build\music-playlist.exe -l winmm
# -l winmm is to include Windows Multimedia API (winmm), which required for using mciSendString
Clear-Host
.\build\music-playlist.exe
