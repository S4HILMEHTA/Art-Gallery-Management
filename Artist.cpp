// Artist.cpp
#include "Artist.h"

#include <iomanip>
#include <iostream>
#include <sstream>

Artist::Artist() : Person(), style("") {}

Artist::Artist(int artistId, const std::string& name, const std::string& style)
    : Person(artistId, name), style(style) {}

const std::string& Artist::getStyle() const { return style; }

void Artist::setStyle(const std::string& newStyle) { style = newStyle; }

void Artist::printHeader() {
    std::cout << std::left << std::setw(8) << "ID" << std::setw(32) << "Name" << "Style\n"
              << std::string(65, '-') << "\n";
}

void Artist::display() const {
    std::cout << std::left << std::setw(8) << id << std::setw(32) << name << style << "\n";
}

std::string Artist::serialize() const {
    std::ostringstream out;
    out << id << '|' << name << '|' << style;
    return out.str();
}

std::string Artist::typeName() const { return "Artist"; }

std::string Artist::searchText() const { return name + " " + style; }

std::string Artist::role() const { return "Artist"; }
