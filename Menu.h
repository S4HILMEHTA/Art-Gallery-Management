// Menu.h
// Console user interface. It reads and validates user input and calls Gallery functions.
#ifndef MENU_H
#define MENU_H

#include "FileManager.h"
#include "Gallery.h"

class Menu {
private:
    Gallery& gallery;
    FileManager& fileManager;

    void showMainMenu() const;
    int chooseRecordType(const std::string& action, bool includeSales) const;

    void addRecord();
    void displayRecords();
    void searchRecord();
    void updateRecord();
    void deleteRecord();
    void transactionMenu();
    void reportMenu();
    void saveLoadMenu();

    // per-entity helpers
    void addArtist();
    void addArtwork();
    void addCustomer();
    void addExhibition();
    void updateArtist();
    void updateArtwork();
    void updateCustomer();
    void updateExhibition();
    void deleteByType(int type);
    void sellArtwork();
    void cancelSale();
    void organizeExhibition();
    void removeFromExhibition();

public:
    Menu(Gallery& gallery, FileManager& fileManager);
    void run();
};

#endif
