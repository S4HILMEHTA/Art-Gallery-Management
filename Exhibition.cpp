// Exhibition.cpp
#include "Exhibition.h"
#include "GalleryException.h"

#include <iomanip>
#include <iostream>
#include <sstream>

static const int INITIAL_CAPACITY = 4;

Exhibition::Exhibition()
    : Entity(), name(""), date(""), venue(""), artworkIds(new int[INITIAL_CAPACITY]), count(0),
      capacity(INITIAL_CAPACITY) {}

Exhibition::Exhibition(int exhibitionId, const std::string& name, const std::string& date,
                       const std::string& venue)
    : Entity(exhibitionId), name(name), date(date), venue(venue),
      artworkIds(new int[INITIAL_CAPACITY]), count(0), capacity(INITIAL_CAPACITY) {}

Exhibition::Exhibition(const Exhibition& other)
    : Entity(other.id), name(other.name), date(other.date), venue(other.venue),
      artworkIds(new int[other.capacity]), count(other.count), capacity(other.capacity) {
    for (int i = 0; i < count; ++i) artworkIds[i] = other.artworkIds[i];
}

Exhibition& Exhibition::operator=(const Exhibition& other) {
    if (this != &other) {
        int* fresh = new int[other.capacity];            // allocate first (exception safe)
        for (int i = 0; i < other.count; ++i) fresh[i] = other.artworkIds[i];
        delete[] artworkIds;                              // then release the old block
        artworkIds = fresh;
        id = other.id;
        name = other.name;
        date = other.date;
        venue = other.venue;
        count = other.count;
        capacity = other.capacity;
    }
    return *this;
}

Exhibition::~Exhibition() { delete[] artworkIds; }

void Exhibition::grow() {
    int newCapacity = capacity * 2;
    int* bigger = new int[newCapacity];
    for (int i = 0; i < count; ++i) bigger[i] = artworkIds[i];
    delete[] artworkIds;
    artworkIds = bigger;
    capacity = newCapacity;
}

const std::string& Exhibition::getName() const { return name; }
const std::string& Exhibition::getDate() const { return date; }
const std::string& Exhibition::getVenue() const { return venue; }
void Exhibition::setName(const std::string& newName) { name = newName; }
void Exhibition::setDate(const std::string& newDate) { date = newDate; }
void Exhibition::setVenue(const std::string& newVenue) { venue = newVenue; }

int Exhibition::getArtworkCount() const { return count; }

int Exhibition::getArtworkIdAt(int index) const {
    if (index < 0 || index >= count) throw GalleryException("Exhibition index out of range.");
    return artworkIds[index];
}

bool Exhibition::hasArtwork(int artworkId) const {
    for (int i = 0; i < count; ++i) {
        if (artworkIds[i] == artworkId) return true;
    }
    return false;
}

void Exhibition::addArtwork(int artworkId) {
    if (hasArtwork(artworkId)) {
        throw GalleryException("Artwork ID is already part of this exhibition.");
    }
    if (count == capacity) grow();
    artworkIds[count++] = artworkId;
}

bool Exhibition::removeArtwork(int artworkId) {
    for (int i = 0; i < count; ++i) {
        if (artworkIds[i] == artworkId) {
            for (int j = i; j < count - 1; ++j) artworkIds[j] = artworkIds[j + 1];
            --count;
            return true;
        }
    }
    return false;
}

void Exhibition::printHeader() {
    std::cout << std::left << std::setw(8) << "ID" << std::setw(36) << "Name" << std::setw(13)
              << "Date" << std::setw(26) << "Venue" << "Artworks\n"
              << std::string(90, '-') << "\n";
}

void Exhibition::display() const {
    std::cout << std::left << std::setw(8) << id << std::setw(36) << name << std::setw(13) << date
              << std::setw(26) << venue << count << "\n";
}

std::string Exhibition::serialize() const {
    std::ostringstream out;
    out << id << '|' << name << '|' << date << '|' << venue << '|' << count << '|';
    for (int i = 0; i < count; ++i) {
        if (i > 0) out << ',';
        out << artworkIds[i];
    }
    return out.str();
}

std::string Exhibition::typeName() const { return "Exhibition"; }

std::string Exhibition::searchText() const { return name + " " + venue + " " + date; }
