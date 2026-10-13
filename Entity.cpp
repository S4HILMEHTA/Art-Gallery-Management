// Entity.cpp
#include "Entity.h"
#include "Utils.h"

Entity::Entity() : id(0) {}

Entity::Entity(int id) : id(id) {}

Entity::~Entity() {}

int Entity::getId() const { return id; }

bool Entity::matches(const std::string& term) const {
    std::string t = Utils::trim(term);
    if (t.empty()) return false;
    if (Utils::isDigits(t) && t.size() <= 9 && Utils::parseInt(t) == id) return true;
    return Utils::containsIgnoreCase(searchText(), t);
}
