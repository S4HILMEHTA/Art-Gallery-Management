// Registry.h
// Generic container that stores records in a DYNAMICALLY allocated array.
//   items = new T[capacity];     (allocated in the constructor)
//   grows by doubling when full  (allocate bigger block, copy, delete[] old block)
//   delete[] items;              (released in the destructor)
// Used by Gallery for artworks, artists, customers, exhibitions and sales.
#ifndef REGISTRY_H
#define REGISTRY_H

#include "GalleryException.h"

template <typename T>
class Registry {
private:
    T* items;
    int count;
    int capacity;

    void grow() {
        int newCapacity = capacity * 2;
        T* bigger = new T[newCapacity];
        for (int i = 0; i < count; ++i) bigger[i] = items[i];
        delete[] items;
        items = bigger;
        capacity = newCapacity;
    }

public:
    explicit Registry(int initialCapacity = 4)
        : items(new T[initialCapacity]), count(0), capacity(initialCapacity) {}

    ~Registry() { delete[] items; }

    // A registry owns its memory, so copying is disabled (prevents double delete).
    Registry(const Registry&) = delete;
    Registry& operator=(const Registry&) = delete;

    int size() const { return count; }
    int getCapacity() const { return capacity; }

    void add(const T& item) {
        if (count == capacity) grow();
        items[count++] = item;
    }

    T& at(int index) {
        if (index < 0 || index >= count) throw GalleryException("Record index out of range.");
        return items[index];
    }

    const T& at(int index) const {
        if (index < 0 || index >= count) throw GalleryException("Record index out of range.");
        return items[index];
    }

    int indexOfId(int id) const {
        for (int i = 0; i < count; ++i) {
            if (items[i].getId() == id) return i;
        }
        return -1;
    }

    T* findById(int id) {
        int index = indexOfId(id);
        return index < 0 ? nullptr : &items[index];
    }

    const T* findById(int id) const {
        int index = indexOfId(id);
        return index < 0 ? nullptr : &items[index];
    }

    void removeAt(int index) {
        if (index < 0 || index >= count) throw GalleryException("Record index out of range.");
        for (int i = index; i < count - 1; ++i) items[i] = items[i + 1];
        --count;
        items[count] = T();     // reset the freed slot
    }

    void clear() {
        for (int i = 0; i < count; ++i) items[i] = T();
        count = 0;
    }
};

#endif
