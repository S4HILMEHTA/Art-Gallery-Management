// Gallery.cpp
#include "Gallery.h"
#include "GalleryException.h"
#include "Utils.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

namespace {

// Prints every record of a registry. Each element is used through an Entity pointer,
// so display() is resolved at run time (runtime polymorphism).
template <typename T>
void printAll(const Registry<T>& registry, const std::string& title) {
    std::cout << "\n--- " << title << " (" << registry.size() << ") ---\n";
    if (registry.size() == 0) {
        std::cout << "No records found.\n";
        return;
    }
    T::printHeader();
    for (int i = 0; i < registry.size(); ++i) {
        const Entity* record = &registry.at(i);
        record->display();
    }
}

// Prints all records whose ID or text matches the search term.
template <typename T>
int printMatches(const Registry<T>& registry, const std::string& term) {
    int found = 0;
    for (int i = 0; i < registry.size(); ++i) {
        const Entity* record = &registry.at(i);
        if (record->matches(term)) {
            if (found == 0) T::printHeader();
            record->display();
            ++found;
        }
    }
    return found;
}

std::string notFound(const std::string& what, int id) {
    return what + " ID " + std::to_string(id) + " not found.";
}

}  // namespace

// ------------------------------------------------------------ construction
Gallery::Gallery()
    : galleryName("Art Gallery"), artworks(4), artists(4), customers(4), exhibitions(4), sales(4),
      nextArtistId(101), nextArtworkId(1001), nextCustomerId(201), nextExhibitionId(301),
      nextSaleId(5001) {}

Gallery::Gallery(const std::string& name)
    : galleryName(name), artworks(4), artists(4), customers(4), exhibitions(4), sales(4),
      nextArtistId(101), nextArtworkId(1001), nextCustomerId(201), nextExhibitionId(301),
      nextSaleId(5001) {}

const std::string& Gallery::getName() const { return galleryName; }

// ---------------------------------------------------------------- helpers
bool Gallery::isInAnyExhibition(int artworkId) const {
    for (int i = 0; i < exhibitions.size(); ++i) {
        if (exhibitions.at(i).hasArtwork(artworkId)) return true;
    }
    return false;
}

// An "Exhibited" artwork that is no longer in any exhibition becomes "Available".
void Gallery::refreshExhibitedStatus(int artworkId) {
    Artwork* art = artworks.findById(artworkId);
    if (art != nullptr && art->getStatus() == Artwork::EXHIBITED && !isInAnyExhibition(artworkId)) {
        art->updateStatus(Artwork::AVAILABLE);
    }
}

// -------------------------------------------------------------------- add
int Gallery::addArtist(const std::string& name, const std::string& style) {
    for (int i = 0; i < artists.size(); ++i) {
        if (Utils::toLower(artists.at(i).getName()) == Utils::toLower(name)) {
            throw GalleryException("An artist named '" + name + "' already exists.");
        }
    }
    int id = nextArtistId++;
    artists.add(Artist(id, name, style));
    return id;
}

int Gallery::addArtwork(const std::string& title, int artistId, double price) {
    if (findArtist(artistId) == nullptr) throw GalleryException(notFound("Artist", artistId));
    if (price <= 0) throw GalleryException("Price must be greater than zero.");
    for (int i = 0; i < artworks.size(); ++i) {
        const Artwork& a = artworks.at(i);
        if (a.getArtistId() == artistId && Utils::toLower(a.getTitle()) == Utils::toLower(title)) {
            throw GalleryException("This artist already has an artwork titled '" + title + "'.");
        }
    }
    int id = nextArtworkId++;
    artworks.add(Artwork(id, title, artistId, price));
    return id;
}

int Gallery::addCustomer(const std::string& name, const std::string& phone) {
    for (int i = 0; i < customers.size(); ++i) {
        if (customers.at(i).getPhone() == phone) {
            throw GalleryException("A customer with phone number " + phone + " already exists.");
        }
    }
    int id = nextCustomerId++;
    customers.add(Customer(id, name, phone));
    return id;
}

int Gallery::addExhibition(const std::string& name, const std::string& date,
                           const std::string& venue) {
    if (!Utils::isValidDate(date)) throw GalleryException("Invalid exhibition date: " + date);
    int id = nextExhibitionId++;
    exhibitions.add(Exhibition(id, name, date, venue));
    return id;
}

// ----------------------------------------------------------------- lookup
const Artist* Gallery::findArtist(int id) const { return artists.findById(id); }
const Artwork* Gallery::findArtwork(int id) const { return artworks.findById(id); }
const Customer* Gallery::findCustomer(int id) const { return customers.findById(id); }
const Exhibition* Gallery::findExhibition(int id) const { return exhibitions.findById(id); }
const Sale* Gallery::findSale(int id) const { return sales.findById(id); }

std::string Gallery::artistName(int id) const {
    const Artist* a = findArtist(id);
    return a ? a->getName() : "Unknown";
}

std::string Gallery::customerName(int id) const {
    const Customer* c = findCustomer(id);
    return c ? c->getName() : "Unknown";
}

std::string Gallery::artworkTitle(int id) const {
    const Artwork* a = findArtwork(id);
    return a ? a->getTitle() : "Unknown";
}

// ---------------------------------------------------------------- display
void Gallery::displayArtists() const { printAll(artists, "ARTISTS"); }
void Gallery::displayArtworks() const { printAll(artworks, "ARTWORKS"); }
void Gallery::displayCustomers() const { printAll(customers, "CUSTOMERS"); }
void Gallery::displaySales() const { printAll(sales, "SALES"); }

void Gallery::displayExhibitions() const {
    printAll(exhibitions, "EXHIBITIONS");
}

void Gallery::displayAvailableArtworks() const {
    std::cout << "\n--- UNSOLD ARTWORKS (can be sold or exhibited) ---\n";
    int shown = 0;
    for (int i = 0; i < artworks.size(); ++i) {
        const Artwork& a = artworks.at(i);
        if (a.getStatus() != Artwork::SOLD) {
            if (shown == 0) Artwork::printHeader();
            a.display();
            ++shown;
        }
    }
    if (shown == 0) std::cout << "No unsold artworks.\n";
}

void Gallery::displayExhibitionDetails(int exhibitionId) const {
    const Exhibition* ex = findExhibition(exhibitionId);
    if (ex == nullptr) throw GalleryException(notFound("Exhibition", exhibitionId));
    std::cout << "\nExhibition: " << ex->getName() << " | " << ex->getDate() << " | "
              << ex->getVenue() << "\n";
    if (ex->getArtworkCount() == 0) {
        std::cout << "  (no artworks assigned yet)\n";
        return;
    }
    Artwork::printHeader();
    for (int i = 0; i < ex->getArtworkCount(); ++i) {
        const Artwork* a = findArtwork(ex->getArtworkIdAt(i));
        if (a != nullptr) a->display();
    }
}

void Gallery::displayAll() const {
    displayArtists();
    displayArtworks();
    displayCustomers();
    displayExhibitions();
    displaySales();
}

void Gallery::displayReceipt(int saleId) const {
    const Sale* sale = findSale(saleId);
    if (sale == nullptr) throw GalleryException(notFound("Sale", saleId));
    const Artwork* art = findArtwork(sale->getArtworkId());
    std::cout << "\n------------------ SALE RECEIPT ------------------\n"
              << "Sale ID    : " << sale->getId() << "\n"
              << "Date       : " << sale->getDate() << "\n"
              << "Artwork    : " << artworkTitle(sale->getArtworkId()) << " (ID "
              << sale->getArtworkId() << ")\n"
              << "Artist     : " << (art ? artistName(art->getArtistId()) : "Unknown") << "\n"
              << "Customer   : " << customerName(sale->getCustomerId()) << " (ID "
              << sale->getCustomerId() << ")\n"
              << "Amount     : " << Utils::formatMoney(sale->getAmount()) << "\n"
              << "Status     : " << sale->getStatus() << "\n"
              << "--------------------------------------------------\n";
}

// ----------------------------------------------------------------- search
int Gallery::searchArtists(const std::string& term) const { return printMatches(artists, term); }
int Gallery::searchArtworks(const std::string& term) const { return printMatches(artworks, term); }
int Gallery::searchCustomers(const std::string& term) const { return printMatches(customers, term); }
int Gallery::searchExhibitions(const std::string& term) const {
    return printMatches(exhibitions, term);
}

// ----------------------------------------------------------------- update
void Gallery::updateArtist(int id, const std::string& name, const std::string& style) {
    Artist* a = artists.findById(id);
    if (a == nullptr) throw GalleryException(notFound("Artist", id));
    for (int i = 0; i < artists.size(); ++i) {
        const Artist& other = artists.at(i);
        if (other.getId() != id && Utils::toLower(other.getName()) == Utils::toLower(name)) {
            throw GalleryException("Another artist is already named '" + name + "'.");
        }
    }
    a->setName(name);
    a->setStyle(style);
}

void Gallery::updateArtwork(int id, const std::string& title, double price,
                            const std::string& status) {
    Artwork* a = artworks.findById(id);
    if (a == nullptr) throw GalleryException(notFound("Artwork", id));
    if (a->getStatus() == Artwork::SOLD) {
        throw GalleryException("Sold artworks cannot be updated. Cancel the sale first.");
    }
    if (price <= 0) throw GalleryException("Price must be greater than zero.");
    if (!Artwork::isValidStatus(status)) {
        throw GalleryException("Invalid status '" + status + "'. Use Available or Exhibited.");
    }
    if (Utils::toLower(status) == "sold") {
        throw GalleryException("Status 'Sold' can only be set by selling the artwork (menu 6).");
    }
    // All checks passed - apply the changes.
    a->setTitle(title);
    a->setPrice(price);
    a->updateStatus(status);
}

void Gallery::updateCustomer(int id, const std::string& name, const std::string& phone) {
    Customer* c = customers.findById(id);
    if (c == nullptr) throw GalleryException(notFound("Customer", id));
    for (int i = 0; i < customers.size(); ++i) {
        const Customer& other = customers.at(i);
        if (other.getId() != id && other.getPhone() == phone) {
            throw GalleryException("Another customer already uses phone number " + phone + ".");
        }
    }
    c->setName(name);
    c->setPhone(phone);
}

void Gallery::updateExhibition(int id, const std::string& name, const std::string& date,
                               const std::string& venue) {
    Exhibition* e = exhibitions.findById(id);
    if (e == nullptr) throw GalleryException(notFound("Exhibition", id));
    if (!Utils::isValidDate(date)) throw GalleryException("Invalid exhibition date: " + date);
    e->setName(name);
    e->setDate(date);
    e->setVenue(venue);
}

// ----------------------------------------------------------------- delete
void Gallery::deleteArtist(int id) {
    int index = artists.indexOfId(id);
    if (index < 0) throw GalleryException(notFound("Artist", id));
    for (int i = 0; i < artworks.size(); ++i) {
        if (artworks.at(i).getArtistId() == id) {
            throw GalleryException("Cannot delete: this artist still has artworks in the gallery.");
        }
    }
    artists.removeAt(index);
}

void Gallery::deleteArtwork(int id) {
    int index = artworks.indexOfId(id);
    if (index < 0) throw GalleryException(notFound("Artwork", id));
    for (int i = 0; i < sales.size(); ++i) {
        if (sales.at(i).getArtworkId() == id) {
            throw GalleryException("Cannot delete: this artwork has sale history.");
        }
    }
    for (int i = 0; i < exhibitions.size(); ++i) exhibitions.at(i).removeArtwork(id);
    artworks.removeAt(index);
}

void Gallery::deleteCustomer(int id) {
    int index = customers.indexOfId(id);
    if (index < 0) throw GalleryException(notFound("Customer", id));
    for (int i = 0; i < sales.size(); ++i) {
        if (sales.at(i).getCustomerId() == id) {
            throw GalleryException("Cannot delete: this customer has sale history.");
        }
    }
    customers.removeAt(index);
}

void Gallery::deleteExhibition(int id) {
    int index = exhibitions.indexOfId(id);
    if (index < 0) throw GalleryException(notFound("Exhibition", id));
    // Remember the artworks that were on display so their status can be refreshed.
    int total = exhibitions.at(index).getArtworkCount();
    int* shown = new int[total > 0 ? total : 1];     // temporary dynamic array
    for (int i = 0; i < total; ++i) shown[i] = exhibitions.at(index).getArtworkIdAt(i);
    exhibitions.removeAt(index);
    for (int i = 0; i < total; ++i) refreshExhibitedStatus(shown[i]);
    delete[] shown;
}

// ----------------------------------------------------- main transactions
int Gallery::sellArtwork(int artworkId, int customerId) {
    Artwork* art = artworks.findById(artworkId);
    if (art == nullptr) throw GalleryException(notFound("Artwork", artworkId));
    if (findCustomer(customerId) == nullptr) throw GalleryException(notFound("Customer", customerId));
    if (art->getStatus() == Artwork::SOLD) {
        throw GalleryException("Artwork '" + art->getTitle() + "' is already sold.");
    }
    int saleId = nextSaleId++;
    sales.add(Sale(saleId, artworkId, customerId, art->getPrice(), Utils::todayDate()));
    art->updateStatus(Artwork::SOLD);
    return saleId;
}

void Gallery::cancelSale(int saleId) {
    Sale* sale = sales.findById(saleId);
    if (sale == nullptr) throw GalleryException(notFound("Sale", saleId));
    if (!sale->isCompleted()) throw GalleryException("This sale is already cancelled.");
    sale->cancel();
    Artwork* art = artworks.findById(sale->getArtworkId());
    if (art != nullptr) {
        art->updateStatus(isInAnyExhibition(art->getId()) ? Artwork::EXHIBITED
                                                            : Artwork::AVAILABLE);
    }
}

void Gallery::organizeExhibition(int exhibitionId, int artworkId) {
    Exhibition* ex = exhibitions.findById(exhibitionId);
    if (ex == nullptr) throw GalleryException(notFound("Exhibition", exhibitionId));
    Artwork* art = artworks.findById(artworkId);
    if (art == nullptr) throw GalleryException(notFound("Artwork", artworkId));
    if (art->getStatus() == Artwork::SOLD) {
        throw GalleryException("A sold artwork cannot be added to an exhibition.");
    }
    ex->addArtwork(artworkId);                 // throws if it is already in this exhibition
    art->updateStatus(Artwork::EXHIBITED);
}

void Gallery::removeFromExhibition(int exhibitionId, int artworkId) {
    Exhibition* ex = exhibitions.findById(exhibitionId);
    if (ex == nullptr) throw GalleryException(notFound("Exhibition", exhibitionId));
    if (!ex->removeArtwork(artworkId)) {
        throw GalleryException("That artwork is not part of this exhibition.");
    }
    refreshExhibitedStatus(artworkId);
}

// ---------------------------------------------------------------- reports
void Gallery::reportSales() const {
    std::cout << "\n=============== SALES REPORT ===============\n";
    if (sales.size() == 0) {
        std::cout << "No sales recorded yet.\n";
        return;
    }
    double revenue = 0;
    int completed = 0, cancelled = 0;
    std::cout << std::left << std::setw(8) << "SaleID" << std::setw(30) << "Artwork"
              << std::setw(22) << "Customer" << std::setw(18) << "Amount" << std::setw(12)
              << "Date" << "Status\n"
              << std::string(100, '-') << "\n";
    for (int i = 0; i < sales.size(); ++i) {
        const Sale& s = sales.at(i);
        std::cout << std::left << std::setw(8) << s.getId() << std::setw(30)
                  << artworkTitle(s.getArtworkId()).substr(0, 28) << std::setw(22)
                  << customerName(s.getCustomerId()).substr(0, 20) << std::setw(18)
                  << Utils::formatMoney(s.getAmount()) << std::setw(12) << s.getDate()
                  << s.getStatus() << "\n";
        if (s.isCompleted()) {
            revenue += s.getAmount();
            ++completed;
        } else {
            ++cancelled;
        }
    }
    std::cout << std::string(100, '-') << "\nCompleted sales : " << completed
              << "\nCancelled sales : " << cancelled
              << "\nTotal revenue   : " << Utils::formatMoney(revenue) << "\n";
}

void Gallery::reportInventory() const {
    int available = 0, exhibited = 0, sold = 0;
    double unsoldValue = 0, soldValue = 0;
    for (int i = 0; i < artworks.size(); ++i) {
        const Artwork& a = artworks.at(i);
        if (a.getStatus() == Artwork::SOLD) {
            ++sold;
            soldValue += a.getPrice();
        } else {
            unsoldValue += a.getPrice();
            if (a.getStatus() == Artwork::EXHIBITED) ++exhibited; else ++available;
        }
    }
    std::cout << "\n============== INVENTORY REPORT ==============\n"
              << "Total artworks        : " << artworks.size() << "\n"
              << "Available             : " << available << "\n"
              << "Exhibited             : " << exhibited << "\n"
              << "Sold                  : " << sold << "\n"
              << "Value of unsold stock : " << Utils::formatMoney(unsoldValue) << "\n"
              << "Value of sold works   : " << Utils::formatMoney(soldValue) << "\n"
              << "Registered artists    : " << artists.size() << "\n"
              << "Registered customers  : " << customers.size() << "\n"
              << "Exhibitions organized : " << exhibitions.size() << "\n";
}

void Gallery::reportArtworksByArtist() const {
    std::cout << "\n=========== ARTWORKS BY ARTIST ===========\n";
    if (artists.size() == 0) {
        std::cout << "No artists registered.\n";
        return;
    }
    for (int i = 0; i < artists.size(); ++i) {
        const Artist& artist = artists.at(i);
        int total = 0;
        for (int j = 0; j < artworks.size(); ++j) {
            if (artworks.at(j).getArtistId() == artist.getId()) ++total;
        }
        std::cout << "\n" << artist.getName() << " (ID " << artist.getId() << ", "
                  << artist.getStyle() << ") - " << total << " artwork(s)\n";
        for (int j = 0; j < artworks.size(); ++j) {
            const Artwork& a = artworks.at(j);
            if (a.getArtistId() == artist.getId()) {
                std::cout << "   " << std::left << std::setw(7) << a.getId() << std::setw(36)
                          << a.getTitle() << std::setw(16) << Utils::formatMoney(a.getPrice())
                          << a.getStatus() << "\n";
            }
        }
    }
}

void Gallery::reportExhibitions() const {
    std::cout << "\n============ EXHIBITION REPORT ============\n";
    if (exhibitions.size() == 0) {
        std::cout << "No exhibitions organized yet.\n";
        return;
    }
    for (int i = 0; i < exhibitions.size(); ++i) {
        displayExhibitionDetails(exhibitions.at(i).getId());
    }
}

// ------------------------------------------------- FileManager support
const Registry<Artist>& Gallery::getArtists() const { return artists; }
const Registry<Artwork>& Gallery::getArtworks() const { return artworks; }
const Registry<Customer>& Gallery::getCustomers() const { return customers; }
const Registry<Exhibition>& Gallery::getExhibitions() const { return exhibitions; }
const Registry<Sale>& Gallery::getSales() const { return sales; }

void Gallery::restoreArtist(const Artist& a) {
    artists.add(a);
    nextArtistId = std::max(nextArtistId, a.getId() + 1);
}

void Gallery::restoreArtwork(const Artwork& a) {
    artworks.add(a);
    nextArtworkId = std::max(nextArtworkId, a.getId() + 1);
}

void Gallery::restoreCustomer(const Customer& c) {
    customers.add(c);
    nextCustomerId = std::max(nextCustomerId, c.getId() + 1);
}

void Gallery::restoreExhibition(const Exhibition& e) {
    exhibitions.add(e);
    nextExhibitionId = std::max(nextExhibitionId, e.getId() + 1);
}

void Gallery::restoreSale(const Sale& s) {
    sales.add(s);
    nextSaleId = std::max(nextSaleId, s.getId() + 1);
}

void Gallery::clearAll() {
    artists.clear();
    artworks.clear();
    customers.clear();
    exhibitions.clear();
    sales.clear();
    nextArtistId = 101;
    nextArtworkId = 1001;
    nextCustomerId = 201;
    nextExhibitionId = 301;
    nextSaleId = 5001;
}
