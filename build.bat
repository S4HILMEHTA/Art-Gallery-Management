@echo off
REM Build script for Windows (MinGW g++)
g++ -std=c++17 -Wall -Wextra -o ArtGallery.exe main.cpp Menu.cpp Gallery.cpp FileManager.cpp Artwork.cpp Artist.cpp Customer.cpp Person.cpp Entity.cpp Exhibition.cpp Sale.cpp Utils.cpp
if %errorlevel% neq 0 (echo Build FAILED) else (echo Build successful: ArtGallery.exe)
