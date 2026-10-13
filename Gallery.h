// Gallery.h
// The SYSTEM class. It owns all records (in dynamic registries) and contains the
// business logic: adding, searching, updating, deleting, selling, organizing
// exhibitions and producing reports. It never talks to the keyboard - the Menu does.
#ifndef GALLERY_H
#define GALLERY_H

#include <string>

#include "Artist.h"
#include "Artwork.h"
#include "Customer.h"
#include "Exhibition.h"
#include "Registry.h"
#include "Sale.h"

class Gallery {
private:
    std::string galleryName;
    Registry<Artwork> artworks;
    Registry<Artist> artists;
    Registry<Customer> customers;
    Registry<Exhibition> exhibitions;
    Registry<Sale> sales;

    int nextArtistId;
    int nextArtworkId;
    int nextCustomerId;
    int nextExhibitionId;
    int nextSaleId;

    bool isInAnyExhibition(int artworkId) const;
    void refreshExhibitedStatus(int artworkId);

public:
    Gallery();                                       // default constructor
    explicit Gallery(const std::string& name);       // parameterized constructor

    const std::string& getName() const;

    // ---------- Add ----------
    int addArtist(const std::string& name, const std::string& style);
    int addArtwork(const std::string& title, int artistId, double price);
    int addCustomer(const std::string& name, const std::string& phone);
    int addExhibition(const std::string& name, const std::string& date, const std::string& venue);

    // ---------- Lookup ----------
    const Artist* findArtist(int id) const;
    const Artwork* findArtwork(int id) const;
    const Customer* findCustomer(int id) const;
    const Exhibition* findExhibition(int id) const;
    const Sale* findSale(int id) const;
    std::string artistName(int id) const;            // "Unknown" if missing
    std::string customerName(int id) const;
    std::string artworkTitle(int id) const;

    // ---------- Display ----------
    void displayArtists() const;
    void displayArtworks() const;
    void displayAvailableArtworks() const;
    void displayCustomers() const;
    void displayExhibitions() const;
    void displaySales() const;
    void displayExhibitionDetails(int exhibitionId) const;
    void displayAll() const;
    void displayReceipt(int saleId) const;

    // ---------- Search (returns number of matches) ----------
    int searchArtists(const std::string& term) const;
    int searchArtworks(const std::string& term) const;
    int searchCustomers(const std::string& term) const;
    int searchExhibitions(const std::string& term) const;

    // ---------- Update ----------
    void updateArtist(int id, const std::string& name, const std::string& style);
    void updateArtwork(int id, const std::string& title, double price, const std::string& status);
    void updateCustomer(int id, const std::string& name, const std::string& phone);
    void updateExhibition(int id, const std::string& name, const std::string& date,
                          const std::string& venue);

    // ---------- Delete ----------
    void deleteArtist(int id);
    void deleteArtwork(int id);
    void deleteCustomer(int id);
    void deleteExhibition(int id);

    // ---------- Main transactions ----------
    int sellArtwork(int artworkId, int customerId);          // returns the new sale ID
    void cancelSale(int saleId);
    void organizeExhibition(int exhibitionId, int artworkId);
    void removeFromExhibition(int exhibitionId, int artworkId);

    // ---------- Reports ----------
    void reportSales() const;
    void reportInventory() const;
    void reportArtworksByArtist() const;
    void reportExhibitions() const;

    // ---------- Support for FileManager ----------
    const Registry<Artist>& getArtists() const;
    const Registry<Artwork>& getArtworks() const;
    const Registry<Customer>& getCustomers() const;
    const Registry<Exhibition>& getExhibitions() const;
    const Registry<Sale>& getSales() const;
    void restoreArtist(const Artist& a);
    void restoreArtwork(const Artwork& a);
    void restoreCustomer(const Customer& c);
    void restoreExhibition(const Exhibition& e);
    void restoreSale(const Sale& s);
    void clearAll();
};

#endif
