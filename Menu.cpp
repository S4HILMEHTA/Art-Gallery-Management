// Menu.cpp
#include "Menu.h"
#include "GalleryException.h"
#include "Utils.h"

#include <iostream>

namespace {
const int MAX_ID = 999999;
std::string notFound(const std::string& what, int id) {
    return what + " ID " + std::to_string(id) + " not found.";
}
}  // namespace

Menu::Menu(Gallery& gallery, FileManager& fileManager)
    : gallery(gallery), fileManager(fileManager) {}

// ------------------------------------------------------------- main loop
void Menu::showMainMenu() const {
    std::cout << "\n==================================================\n"
              << "      " << gallery.getName() << " - MANAGEMENT SYSTEM\n"
              << "==================================================\n"
              << " 1. Add Record\n"
              << " 2. Display Records\n"
              << " 3. Search Record\n"
              << " 4. Update Record\n"
              << " 5. Delete Record\n"
              << " 6. Main Transaction / Process\n"
              << " 7. Report\n"
              << " 8. Save / Load\n"
              << " 9. Exit\n"
              << "--------------------------------------------------\n";
}

void Menu::run() {
    bool running = true;
    while (running) {
        try {
            showMainMenu();
            int choice = Utils::readInt("Enter your choice (1-9): ", 1, 9);
            switch (choice) {
                case 1: addRecord(); break;
                case 2: displayRecords(); break;
                case 3: searchRecord(); break;
                case 4: updateRecord(); break;
                case 5: deleteRecord(); break;
                case 6: transactionMenu(); break;
                case 7: reportMenu(); break;
                case 8: saveLoadMenu(); break;
                case 9:
                    fileManager.save(gallery);
                    std::cout << "\nAll records saved to '" << fileManager.getDirectory()
                              << "/'. Thank you - goodbye!\n";
                    running = false;
                    break;
            }
        } catch (const InputEndedException&) {
            std::cout << "\n\nInput closed - saving records and exiting.\n";
            try {
                fileManager.save(gallery);
            } catch (const std::exception& e) {
                std::cout << "[ERROR] " << e.what() << "\n";
            }
            running = false;
        } catch (const GalleryException& e) {
            std::cout << "\n[ERROR] " << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cout << "\n[UNEXPECTED ERROR] " << e.what() << "\n";
        }
    }
}

// ------------------------------------------------------------- selection
int Menu::chooseRecordType(const std::string& action, bool includeSales) const {
    std::cout << "\n" << action << " - select record type:\n"
              << " 1. Artist\n 2. Artwork\n 3. Customer\n 4. Exhibition\n";
    if (includeSales) std::cout << " 5. Sales\n 6. All records\n";
    std::cout << " 0. Back\n";
    return Utils::readInt("Choice: ", 0, includeSales ? 6 : 4);
}

// ----------------------------------------------------------------- add
void Menu::addRecord() {
    switch (chooseRecordType("ADD RECORD", false)) {
        case 1: addArtist(); break;
        case 2: addArtwork(); break;
        case 3: addCustomer(); break;
        case 4: addExhibition(); break;
        default: break;
    }
}

void Menu::addArtist() {
    std::string name = Utils::readText("Artist name", 30);
    std::string style = Utils::readText("Art style", 25);
    int id = gallery.addArtist(name, style);
    std::cout << "\n[OK] Artist added successfully with ID " << id << ".\n";
}

void Menu::addArtwork() {
    if (gallery.getArtists().size() == 0) {
        throw GalleryException("No artists registered. Please add an artist first.");
    }
    gallery.displayArtists();
    std::cout << "\n";
    std::string title = Utils::readText("Artwork title", 40);
    int artistId = Utils::readInt("Artist ID: ", 1, MAX_ID);
    if (gallery.findArtist(artistId) == nullptr) throw GalleryException(notFound("Artist", artistId));
    double price = Utils::readPrice("Price (Rs.)");
    int id = gallery.addArtwork(title, artistId, price);
    std::cout << "\n[OK] Artwork added successfully with ID " << id << ".\n";
}

void Menu::addCustomer() {
    std::string name = Utils::readText("Customer name", 30);
    std::string phone = Utils::readPhone("Phone number (10 digits)");
    int id = gallery.addCustomer(name, phone);
    std::cout << "\n[OK] Customer added successfully with ID " << id << ".\n";
}

void Menu::addExhibition() {
    std::string name = Utils::readText("Exhibition name", 35);
    std::string date = Utils::readDate("Exhibition date");
    std::string venue = Utils::readText("Venue", 25);
    int id = gallery.addExhibition(name, date, venue);
    std::cout << "\n[OK] Exhibition created with ID " << id
              << ". Use menu 6 to add artworks to it.\n";
}

// ------------------------------------------------------------- display
void Menu::displayRecords() {
    switch (chooseRecordType("DISPLAY RECORDS", true)) {
        case 1: gallery.displayArtists(); break;
        case 2: gallery.displayArtworks(); break;
        case 3: gallery.displayCustomers(); break;
        case 4: gallery.displayExhibitions(); break;
        case 5: gallery.displaySales(); break;
        case 6: gallery.displayAll(); break;
        default: break;
    }
}

// -------------------------------------------------------------- search
void Menu::searchRecord() {
    int type = chooseRecordType("SEARCH RECORD", false);
    if (type == 0) return;
    std::string term = Utils::readText("Enter ID or keyword (name/title/style/phone/status)", 40);
    std::cout << "\n";
    int found = 0;
    switch (type) {
        case 1: found = gallery.searchArtists(term); break;
        case 2: found = gallery.searchArtworks(term); break;
        case 3: found = gallery.searchCustomers(term); break;
        case 4: found = gallery.searchExhibitions(term); break;
    }
    if (found == 0) {
        std::cout << "No record found for '" << term << "'.\n";
    } else {
        std::cout << "\n" << found << " record(s) found.\n";
    }
}

// -------------------------------------------------------------- update
void Menu::updateRecord() {
    switch (chooseRecordType("UPDATE RECORD", false)) {
        case 1: updateArtist(); break;
        case 2: updateArtwork(); break;
        case 3: updateCustomer(); break;
        case 4: updateExhibition(); break;
        default: break;
    }
}

void Menu::updateArtist() {
    int id = Utils::readInt("Enter Artist ID to update: ", 1, MAX_ID);
    const Artist* a = gallery.findArtist(id);
    if (a == nullptr) throw GalleryException(notFound("Artist", id));
    std::cout << "\nCurrent record:\n";
    Artist::printHeader();
    a->display();
    std::cout << "\n(Press Enter to keep the current value)\n";
    std::string name = Utils::readOptionalText("New name", 30);
    std::string style = Utils::readOptionalText("New style", 25);
    gallery.updateArtist(id, name.empty() ? a->getName() : name,
                         style.empty() ? a->getStyle() : style);
    std::cout << "\n[OK] Artist updated.\n";
}

void Menu::updateArtwork() {
    int id = Utils::readInt("Enter Artwork ID to update: ", 1, MAX_ID);
    const Artwork* a = gallery.findArtwork(id);
    if (a == nullptr) throw GalleryException(notFound("Artwork", id));
    std::cout << "\nCurrent record:\n";
    Artwork::printHeader();
    a->display();
    std::cout << "\n(Press Enter to keep the current value)\n";
    std::string title = Utils::readOptionalText("New title", 40);
    double price = Utils::readOptionalPrice("New price (Rs.)", a->getPrice());
    std::string status = Utils::readOptionalText("New status (Available/Exhibited)", 12);
    gallery.updateArtwork(id, title.empty() ? a->getTitle() : title, price,
                          status.empty() ? a->getStatus() : status);
    std::cout << "\n[OK] Artwork updated.\n";
}

void Menu::updateCustomer() {
    int id = Utils::readInt("Enter Customer ID to update: ", 1, MAX_ID);
    const Customer* c = gallery.findCustomer(id);
    if (c == nullptr) throw GalleryException(notFound("Customer", id));
    std::cout << "\nCurrent record:\n";
    Customer::printHeader();
    c->display();
    std::cout << "\n(Press Enter to keep the current value)\n";
    std::string name = Utils::readOptionalText("New name", 30);
    std::string phone;
    while (true) {
        phone = Utils::readLine("New phone (10 digits): ");
        if (phone.empty() || (phone.size() == 10 && Utils::isDigits(phone))) break;
        std::cout << "  Invalid input: phone number must be exactly 10 digits.\n";
    }
    gallery.updateCustomer(id, name.empty() ? c->getName() : name,
                           phone.empty() ? c->getPhone() : phone);
    std::cout << "\n[OK] Customer updated.\n";
}

void Menu::updateExhibition() {
    int id = Utils::readInt("Enter Exhibition ID to update: ", 1, MAX_ID);
    const Exhibition* e = gallery.findExhibition(id);
    if (e == nullptr) throw GalleryException(notFound("Exhibition", id));
    std::cout << "\nCurrent record:\n";
    Exhibition::printHeader();
    e->display();
    std::cout << "\n(Press Enter to keep the current value)\n";
    std::string name = Utils::readOptionalText("New name", 35);
    std::string date;
    while (true) {
        date = Utils::readLine("New date (dd/mm/yyyy): ");
        if (date.empty() || Utils::isValidDate(date)) break;
        std::cout << "  Invalid input: enter a real calendar date in dd/mm/yyyy format.\n";
    }
    std::string venue = Utils::readOptionalText("New venue", 25);
    gallery.updateExhibition(id, name.empty() ? e->getName() : name,
                             date.empty() ? e->getDate() : date,
                             venue.empty() ? e->getVenue() : venue);
    std::cout << "\n[OK] Exhibition updated.\n";
}

// -------------------------------------------------------------- delete
void Menu::deleteRecord() {
    int type = chooseRecordType("DELETE RECORD", false);
    if (type != 0) deleteByType(type);
}

void Menu::deleteByType(int type) {
    const char* labels[] = {"", "Artist", "Artwork", "Customer", "Exhibition"};
    int id = Utils::readInt(std::string("Enter ") + labels[type] + " ID to delete: ", 1, MAX_ID);

    std::cout << "\n";
    int shown = 0;
    switch (type) {
        case 1: shown = gallery.searchArtists(std::to_string(id)); break;
        case 2: shown = gallery.searchArtworks(std::to_string(id)); break;
        case 3: shown = gallery.searchCustomers(std::to_string(id)); break;
        case 4: shown = gallery.searchExhibitions(std::to_string(id)); break;
    }
    if (shown == 0) throw GalleryException(notFound(labels[type], id));

    if (!Utils::readYesNo("Are you sure you want to delete this record?")) {
        std::cout << "Deletion cancelled.\n";
        return;
    }
    switch (type) {
        case 1: gallery.deleteArtist(id); break;
        case 2: gallery.deleteArtwork(id); break;
        case 3: gallery.deleteCustomer(id); break;
        case 4: gallery.deleteExhibition(id); break;
    }
    std::cout << "\n[OK] " << labels[type] << " deleted.\n";
}

// --------------------------------------------------------- transactions
void Menu::transactionMenu() {
    std::cout << "\nMAIN TRANSACTION / PROCESS\n"
              << " 1. Sell Artwork\n"
              << " 2. Cancel a Sale\n"
              << " 3. Organize Exhibition (add artwork)\n"
              << " 4. Remove Artwork from Exhibition\n"
              << " 0. Back\n";
    switch (Utils::readInt("Choice: ", 0, 4)) {
        case 1: sellArtwork(); break;
        case 2: cancelSale(); break;
        case 3: organizeExhibition(); break;
        case 4: removeFromExhibition(); break;
        default: break;
    }
}

void Menu::sellArtwork() {
    if (gallery.getCustomers().size() == 0) {
        throw GalleryException("No customers registered. Please add a customer first.");
    }
    gallery.displayAvailableArtworks();
    int artworkId = Utils::readInt("\nEnter Artwork ID to sell: ", 1, MAX_ID);
    const Artwork* art = gallery.findArtwork(artworkId);
    if (art == nullptr) throw GalleryException(notFound("Artwork", artworkId));
    gallery.displayCustomers();
    int customerId = Utils::readInt("\nEnter Customer ID (buyer): ", 1, MAX_ID);
    int saleId = gallery.sellArtwork(artworkId, customerId);
    std::cout << "\n[OK] Sale completed successfully.";
    gallery.displayReceipt(saleId);
}

void Menu::cancelSale() {
    gallery.displaySales();
    int saleId = Utils::readInt("\nEnter Sale ID to cancel: ", 1, MAX_ID);
    gallery.cancelSale(saleId);
    std::cout << "\n[OK] Sale " << saleId << " cancelled. The artwork is available again.\n";
}

void Menu::organizeExhibition() {
    if (gallery.getExhibitions().size() == 0) {
        throw GalleryException("No exhibitions created. Add an exhibition first (menu 1).");
    }
    gallery.displayExhibitions();
    int exhibitionId = Utils::readInt("\nEnter Exhibition ID: ", 1, MAX_ID);
    if (gallery.findExhibition(exhibitionId) == nullptr) {
        throw GalleryException(notFound("Exhibition", exhibitionId));
    }
    gallery.displayAvailableArtworks();
    int artworkId = Utils::readInt("\nEnter Artwork ID to add: ", 1, MAX_ID);
    gallery.organizeExhibition(exhibitionId, artworkId);
    std::cout << "\n[OK] Artwork '" << gallery.artworkTitle(artworkId) << "' added to exhibition '"
              << gallery.findExhibition(exhibitionId)->getName() << "'.\n";
    gallery.displayExhibitionDetails(exhibitionId);
}

void Menu::removeFromExhibition() {
    gallery.displayExhibitions();
    int exhibitionId = Utils::readInt("\nEnter Exhibition ID: ", 1, MAX_ID);
    gallery.displayExhibitionDetails(exhibitionId);
    int artworkId = Utils::readInt("\nEnter Artwork ID to remove: ", 1, MAX_ID);
    gallery.removeFromExhibition(exhibitionId, artworkId);
    std::cout << "\n[OK] Artwork removed from the exhibition.\n";
}

// --------------------------------------------------------------- report
void Menu::reportMenu() {
    std::cout << "\nREPORTS\n"
              << " 1. Sales Report\n"
              << " 2. Inventory Summary\n"
              << " 3. Artworks by Artist\n"
              << " 4. Exhibition Report\n"
              << " 0. Back\n";
    switch (Utils::readInt("Choice: ", 0, 4)) {
        case 1: gallery.reportSales(); break;
        case 2: gallery.reportInventory(); break;
        case 3: gallery.reportArtworksByArtist(); break;
        case 4: gallery.reportExhibitions(); break;
        default: break;
    }
}

// ------------------------------------------------------------ save/load
void Menu::saveLoadMenu() {
    std::cout << "\nSAVE / LOAD\n"
              << " 1. Save all records to file\n"
              << " 2. Load records from file (unsaved changes are discarded)\n"
              << " 0. Back\n";
    switch (Utils::readInt("Choice: ", 0, 2)) {
        case 1:
            fileManager.save(gallery);
            std::cout << "\n[OK] Records saved to '" << fileManager.getDirectory() << "/'.\n";
            break;
        case 2:
            if (Utils::readYesNo("Reload from file and discard unsaved changes?")) {
                int skipped = fileManager.load(gallery);
                std::cout << "\n[OK] Records loaded: " << gallery.getArtists().size()
                          << " artists, " << gallery.getArtworks().size() << " artworks, "
                          << gallery.getCustomers().size() << " customers, "
                          << gallery.getExhibitions().size() << " exhibitions, "
                          << gallery.getSales().size() << " sales.\n";
                if (skipped > 0) std::cout << skipped << " corrupted line(s) were skipped.\n";
            }
            break;
        default: break;
    }
}
