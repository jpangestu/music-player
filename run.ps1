g++ src\main.cpp src\music_ui.cpp src\queue.cpp -o build\music-player.exe -l winmm
# -l winmm is to include Windows Multimedia API (winmm), which required for using mciSendString

.\build\music-player.exe
