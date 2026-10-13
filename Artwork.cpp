// Artwork.cpp
#include "Artwork.h"
#include "GalleryException.h"
#include "Utils.h"

#include <iomanip>
#include <iostream>
#include <sstream>

const std::string Artwork::AVAILABLE = "Available";
const std::string Artwork::EXHIBITED = "Exhibited";
const std::string Artwork::SOLD = "Sold";

Artwork::Artwork() : Entity(), title(""), artistId(0), price(0.0), status(AVAILABLE) {}

Artwork::Artwork(int artworkId, const std::string& title, int artistId, double price,
                 const std::string& status)
    : Entity(artworkId), title(title), artistId(artistId), price(price), status(status) {}

const std::string& Artwork::getTitle() const { return title; }
int Artwork::getArtistId() const { return artistId; }
double Artwork::getPrice() const { return price; }
const std::string& Artwork::getStatus() const { return status; }

void Artwork::setTitle(const std::string& newTitle) { title = newTitle; }

void Artwork::setPrice(double newPrice) {
    if (newPrice <= 0) throw GalleryException("Price must be greater than zero.");
    price = newPrice;
}

bool Artwork::isValidStatus(const std::string& s) {
    std::string t = Utils::toLower(s);
    return t == "available" || t == "exhibited" || t == "sold";
}

void Artwork::updateStatus(const std::string& newStatus) {
    std::string t = Utils::toLower(Utils::trim(newStatus));
    if (t == "available") {
        status = AVAILABLE;
    } else if (t == "exhibited") {
        status = EXHIBITED;
    } else if (t == "sold") {
        status = SOLD;
    } else {
        throw GalleryException("Invalid status '" + newStatus +
                               "'. Use Available, Exhibited or Sold.");
    }
}

void Artwork::printHeader() {
    std::cout << std::left << std::setw(8) << "ID" << std::setw(42) << "Title" << std::setw(10)
              << "ArtistID" << std::setw(18) << "Price" << "Status\n"
              << std::string(88, '-') << "\n";
}

void Artwork::display() const {
    std::cout << std::left << std::setw(8) << id << std::setw(42) << title << std::setw(10)
              << artistId << std::setw(18) << Utils::formatMoney(price) << status << "\n";
}

std::string Artwork::serialize() const {
    std::ostringstream out;
    out << id << '|' << title << '|' << artistId << '|' << std::fixed << std::setprecision(2)
        << price << '|' << status;
    return out.str();
}

std::string Artwork::typeName() const { return "Artwork"; }

std::string Artwork::searchText() const { return title + " " + status; }
