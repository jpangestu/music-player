# Console-based Music Player

## How to Compile
Before compiling, make sure your Visual Studio Code are using MinGW that is installed via MSYS2; just like from the official guide, https://code.visualstudio.com/docs/cpp/config-mingw
Then, simply run the `compileAndRun.ps1` with PowerShell to compile it:
```sh
.\compileAndRun.ps1
```
Or
```sh
g++ .\src\main.cpp .\src\music_ui.cpp .\src\queue.cpp .\src\align_helper.hpp -o music-player.exe -l winmm
```
```sh
.\music-player.exe
```
## About
This program is created as a final project for the Data Structures class by
**Group 7:**
Tandang Pangestu - 23.61.0261
Zhafif Hylmi Naryama - 23.61.0263
Aditya Christopher Lallo - 23.61.0264