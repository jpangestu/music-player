g++ .\src\main.cpp .\src\music_ui.cpp .\src\queue.cpp .\src\align_helper.hpp -o music-player.exe -l winmm
# -l winmm is to include Windows Multimedia API (winmm), which required for using mciSendString

.\music-player.exe