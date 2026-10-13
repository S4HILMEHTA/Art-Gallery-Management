// FileManager.cpp
#include "FileManager.h"
#include "GalleryException.h"
#include "Utils.h"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace {

const char* ARTISTS_FILE = "artists.txt";
const char* ARTWORKS_FILE = "artworks.txt";
const char* CUSTOMERS_FILE = "customers.txt";
const char* EXHIBITIONS_FILE = "exhibitions.txt";
const char* SALES_FILE = "sales.txt";

template <typename T>
void writeRegistry(const Registry<T>& registry, const std::string& path) {
    std::ofstream out(path.c_str());
    if (!out) throw GalleryException("Cannot open file for writing: " + path);
    for (int i = 0; i < registry.size(); ++i) {
        const Entity* record = &registry.at(i);       // polymorphic call to serialize()
        out << record->serialize() << "\n";
    }
    if (!out) throw GalleryException("Error while writing file: " + path);
}

}  // namespace

FileManager::FileManager() : directory("Data") {}

FileManager::FileManager(const std::string& dataDirectory) : directory(dataDirectory) {}

const std::string& FileManager::getDirectory() const { return directory; }

std::string FileManager::pathFor(const std::string& fileName) const {
    return directory + "/" + fileName;
}

void FileManager::save(const Gallery& gallery) const {
    std::error_code ec;
    std::filesystem::create_directories(directory, ec);
    if (ec) throw GalleryException("Cannot create data folder '" + directory + "'.");

    writeRegistry(gallery.getArtists(), pathFor(ARTISTS_FILE));
    writeRegistry(gallery.getArtworks(), pathFor(ARTWORKS_FILE));
    writeRegistry(gallery.getCustomers(), pathFor(CUSTOMERS_FILE));
    writeRegistry(gallery.getExhibitions(), pathFor(EXHIBITIONS_FILE));
    writeRegistry(gallery.getSales(), pathFor(SALES_FILE));
}

int FileManager::load(Gallery& gallery) const {
    gallery.clearAll();
    int skipped = 0;

    // Reads one file line by line; 'handler' turns the split fields into an object.
    // Any parsing problem is caught so one bad line cannot stop the whole load.
    auto readFile = [&](const std::string& fileName, auto handler) {
        std::ifstream in(pathFor(fileName).c_str());
        if (!in) return;                                   // first run: file does not exist yet
        std::string line;
        int lineNumber = 0;
        while (std::getline(in, line)) {
            ++lineNumber;
            line = Utils::trim(line);
            if (line.empty()) continue;
            try {
                handler(Utils::split(line, '|'));
            } catch (const std::exception&) {
                ++skipped;
                std::cout << "[WARNING] Skipped corrupted record at " << fileName << " line "
                          << lineNumber << ".\n";
            }
        }
    };

    readFile(ARTISTS_FILE, [&](const std::vector<std::string>& f) {
        if (f.size() != 3) throw GalleryException("bad field count");
        gallery.restoreArtist(Artist(Utils::parseInt(f[0]), f[1], f[2]));
    });

    readFile(CUSTOMERS_FILE, [&](const std::vector<std::string>& f) {
        if (f.size() != 3) throw GalleryException("bad field count");
        gallery.restoreCustomer(Customer(Utils::parseInt(f[0]), f[1], f[2]));
    });

    readFile(ARTWORKS_FILE, [&](const std::vector<std::string>& f) {
        if (f.size() != 5) throw GalleryException("bad field count");
        if (!Artwork::isValidStatus(f[4])) throw GalleryException("bad status");
        Artwork a(Utils::parseInt(f[0]), f[1], Utils::parseInt(f[2]), Utils::parseDouble(f[3]));
        a.updateStatus(f[4]);
        gallery.restoreArtwork(a);
    });

    readFile(EXHIBITIONS_FILE, [&](const std::vector<std::string>& f) {
        if (f.size() < 5 || f.size() > 6) throw GalleryException("bad field count");
        Exhibition e(Utils::parseInt(f[0]), f[1], f[2], f[3]);
        if (f.size() == 6 && !f[5].empty()) {
            std::vector<std::string> ids = Utils::split(f[5], ',');
            for (size_t i = 0; i < ids.size(); ++i) e.addArtwork(Utils::parseInt(ids[i]));
        }
        gallery.restoreExhibition(e);
    });

    readFile(SALES_FILE, [&](const std::vector<std::string>& f) {
        if (f.size() != 6) throw GalleryException("bad field count");
        gallery.restoreSale(Sale(Utils::parseInt(f[0]), Utils::parseInt(f[1]),
                                 Utils::parseInt(f[2]), Utils::parseDouble(f[3]), f[4], f[5]));
    });

    return skipped;
}
