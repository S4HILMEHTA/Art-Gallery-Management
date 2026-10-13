// Person.h
// Intermediate abstract class shared by Artist and Customer (multilevel inheritance:
// Entity -> Person -> Artist / Customer).
#ifndef PERSON_H
#define PERSON_H

#include "Entity.h"

class Person : public Entity {
protected:
    std::string name;

public:
    Person();
    Person(int id, const std::string& name);
    virtual ~Person();

    const std::string& getName() const;
    void setName(const std::string& newName);

    virtual std::string role() const = 0;   // "Artist" or "Customer"
};

#endif
