// main.cpp
// Entry point: creates the system objects, loads saved data and starts the menu.
// All the real logic lives in the Gallery, FileManager and Menu classes.
//
// Usage:  ArtGallery [data_folder]      (default data folder: Data)
#include <iostream>

#include "FileManager.h"
#include "Gallery.h"
#include "GalleryException.h"
#include "Menu.h"

int main(int argc, char* argv[]) {
    Gallery gallery("Modern Art Gallery");
    FileManager fileManager(argc > 1 ? argv[1] : "Data");

    std::cout << "Loading saved records from '" << fileManager.getDirectory() << "/' ...\n";
    int skipped = fileManager.load(gallery);
    std::cout << "Loaded: " << gallery.getArtists().size() << " artists, "
              << gallery.getArtworks().size() << " artworks, " << gallery.getCustomers().size()
              << " customers, " << gallery.getExhibitions().size() << " exhibitions, "
              << gallery.getSales().size() << " sales.\n";
    if (skipped > 0) std::cout << skipped << " corrupted line(s) were skipped.\n";

    Menu menu(gallery, fileManager);
    menu.run();
    return 0;
}
