// Exhibition.h
// An exhibition keeps a dynamically allocated array of the artwork IDs on display.
// Because it owns heap memory it implements the Rule of Three
// (destructor, copy constructor, copy assignment).
#ifndef EXHIBITION_H
#define EXHIBITION_H

#include "Entity.h"

class Exhibition : public Entity {
private:
    std::string name;
    std::string date;       // dd/mm/yyyy
    std::string venue;
    int* artworkIds;        // dynamic array (new[] / delete[])
    int count;
    int capacity;

    void grow();

public:
    Exhibition();
    Exhibition(int exhibitionId, const std::string& name, const std::string& date,
               const std::string& venue);
    Exhibition(const Exhibition& other);                 // deep copy
    Exhibition& operator=(const Exhibition& other);      // deep copy
    virtual ~Exhibition();                               // releases the dynamic array

    const std::string& getName() const;
    const std::string& getDate() const;
    const std::string& getVenue() const;
    void setName(const std::string& newName);
    void setDate(const std::string& newDate);
    void setVenue(const std::string& newVenue);

    int getArtworkCount() const;
    int getArtworkIdAt(int index) const;
    bool hasArtwork(int artworkId) const;
    void addArtwork(int artworkId);          // throws if already present
    bool removeArtwork(int artworkId);       // returns false if not present

    void display() const override;
    std::string serialize() const override;
    std::string typeName() const override;
    std::string searchText() const override;

    static void printHeader();
};

#endif
