// FileManager.h
// Handles all file I/O. Each record type is stored in its own text file inside the
// data directory, one record per line, fields separated by '|'.
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>

#include "Gallery.h"

class FileManager {
private:
    std::string directory;

    std::string pathFor(const std::string& fileName) const;

public:
    FileManager();                                       // default: "Data"
    explicit FileManager(const std::string& dataDirectory);

    const std::string& getDirectory() const;

    // Writes every record of the gallery to the data files.
    // Throws GalleryException if a file cannot be written.
    void save(const Gallery& gallery) const;

    // Replaces the gallery contents with the records stored on disk.
    // Missing files are treated as "no records yet"; corrupted lines are skipped.
    // Returns the number of corrupted lines that were skipped.
    int load(Gallery& gallery) const;
};

#endif
