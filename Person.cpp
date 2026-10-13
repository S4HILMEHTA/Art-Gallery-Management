// Person.cpp
#include "Person.h"

Person::Person() : Entity(), name("") {}

Person::Person(int id, const std::string& name) : Entity(id), name(name) {}

Person::~Person() {}

const std::string& Person::getName() const { return name; }

void Person::setName(const std::string& newName) { name = newName; }
