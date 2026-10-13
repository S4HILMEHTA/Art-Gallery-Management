// Artist.h
#ifndef ARTIST_H
#define ARTIST_H

#include "Person.h"

class Artist : public Person {
private:
    std::string style;

public:
    Artist();
    Artist(int artistId, const std::string& name, const std::string& style);

    const std::string& getStyle() const;
    void setStyle(const std::string& newStyle);

    // Overrides of the virtual functions
    void display() const override;
    std::string serialize() const override;
    std::string typeName() const override;
    std::string searchText() const override;
    std::string role() const override;

    static void printHeader();
};

#endif
