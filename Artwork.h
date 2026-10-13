// Artwork.h
#ifndef ARTWORK_H
#define ARTWORK_H

#include "Entity.h"

class Artwork : public Entity {
private:
    std::string title;
    int artistId;
    double price;
    std::string status;     // "Available", "Exhibited" or "Sold"

public:
    static const std::string AVAILABLE;
    static const std::string EXHIBITED;
    static const std::string SOLD;

    Artwork();
    Artwork(int artworkId, const std::string& title, int artistId, double price,
            const std::string& status = "Available");

    // Getters / setters (encapsulation)
    const std::string& getTitle() const;
    int getArtistId() const;
    double getPrice() const;
    const std::string& getStatus() const;
    void setTitle(const std::string& newTitle);
    void setPrice(double newPrice);

    // Validates the status text, normalises its capitalisation and stores it.
    // Throws GalleryException for an unknown status.
    void updateStatus(const std::string& newStatus);
    static bool isValidStatus(const std::string& s);

    void display() const override;
    std::string serialize() const override;
    std::string typeName() const override;
    std::string searchText() const override;

    static void printHeader();
};

#endif
