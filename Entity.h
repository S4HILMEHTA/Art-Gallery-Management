// Entity.h
// Abstract base class for every record stored in the gallery system.
// Demonstrates: abstract class, pure virtual functions, runtime polymorphism.
#ifndef ENTITY_H
#define ENTITY_H

#include <string>

class Entity {
protected:
    int id;

public:
    Entity();                       // default constructor
    explicit Entity(int id);        // parameterized constructor
    virtual ~Entity();              // virtual destructor (safe delete through base pointer)

    int getId() const;

    // Pure virtual functions: every concrete record type must provide these.
    virtual void display() const = 0;                 // print one table row
    virtual std::string serialize() const = 0;        // one line for the data file
    virtual std::string typeName() const = 0;         // "Artist", "Artwork", ...
    virtual std::string searchText() const = 0;       // text used by keyword search

    // Common behaviour built on the virtual functions above.
    bool matches(const std::string& term) const;      // ID or keyword match
};

#endif
