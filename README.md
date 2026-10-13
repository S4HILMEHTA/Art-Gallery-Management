# Art Gallery Management System

**C++ Console-Based OOP Project (CPP64)**<br>**Name:** Sahil Mehta  |  **Enrollment No.:** IU2541230101  |  **Submission Date:** 13/10/2026

A console application that manages the day-to-day records of an art gallery: artists, artworks, customers,
exhibitions and sales. All data is stored in text files so records survive after the program is closed.

## Features
- Add / Display / Search / Update / Delete records for **Artists, Artworks, Customers and Exhibitions**
- **Main transaction:** sell an artwork to a customer (sale receipt), cancel a sale, organize exhibitions
- **Reports:** sales report, inventory summary, artworks by artist, exhibition report
- **Persistence:** records are loaded automatically at start-up and saved at exit (or from menu 8)
- Input validation (empty text, negative price, 10-digit phone, real calendar dates, unknown IDs) and
  exception handling for business rules (e.g. cannot sell an artwork twice, cannot delete an artist who still has artworks)

## Project structure
```
main.cpp                      Entry point (creates Gallery, FileManager, Menu)
Menu.h / Menu.cpp             Console menu and input handling
Gallery.h / Gallery.cpp       System class - business logic and reports
FileManager.h / .cpp          save() and load() of all records
Entity.h / .cpp               Abstract base class of every record
Person.h / .cpp               Abstract class (Entity -> Person)
Artist.h / .cpp               Artist  (Person -> Artist)
Customer.h / .cpp             Customer (Person -> Customer)
Artwork.h / .cpp              Artwork (Entity -> Artwork)
Exhibition.h / .cpp           Exhibition with its own dynamic array (Entity -> Exhibition)
Sale.h / .cpp                 Sale record (Entity -> Sale)
Registry.h                    Template class: dynamic array new T[] / delete[]
Utils.h / .cpp                Validation and safe console input helpers
GalleryException.h            Custom exception classes
Data/                         Data files (artists.txt, artworks.txt, customers.txt, exhibitions.txt, sales.txt)
docs/uml/                     Use Case, Class and 2 Sequence diagrams (.png and PlantUML .puml source)
docs/screenshots/             Program output screenshots
tests/                        15 automated test cases (run_tests.py)
```

## Build and run
**Windows (MinGW g++):** double-click `build.bat` or run
```
g++ -std=c++17 -Wall -Wextra -o ArtGallery.exe main.cpp Menu.cpp Gallery.cpp FileManager.cpp Artwork.cpp Artist.cpp Customer.cpp Person.cpp Entity.cpp Exhibition.cpp Sale.cpp Utils.cpp
ArtGallery.exe
```
**Linux / macOS:**
```
make
./ArtGallery
```
A C++17 compiler is required (g++ 8 or newer). Run the program from the project folder so it finds the `Data/` folder.
An optional argument selects another data folder: `ArtGallery.exe MyDataFolder`.

## Menu
1. Add Record  2. Display Records  3. Search Record  4. Update Record  5. Delete Record
6. Main Transaction / Process (Sell Artwork, Cancel Sale, Organize Exhibition)  7. Report  8. Save / Load  9. Exit

## OOP concepts demonstrated
| Concept | Where |
|---|---|
| Classes and objects, encapsulation | All entity classes use private/protected data with getters/setters |
| Default + parameterized constructors | Every entity class, `Gallery`, `FileManager` |
| Destructors | `Entity` (virtual), `Exhibition` (`delete[]`), `Registry` (`delete[]`) |
| Inheritance | `Entity -> Person -> Artist / Customer`, `Entity -> Artwork / Exhibition / Sale` |
| Runtime polymorphism | `Entity*` calls `display()`, `serialize()`, `matches()` (virtual functions) |
| Dynamic memory | `Registry<T>` (`new T[capacity]`, grows by doubling), `Exhibition::artworkIds` |
| File handling | `FileManager` - `ifstream` / `ofstream`, one text file per record type |
| Validation | `Utils` - names, price, phone, dates, menu choices |
| Exception handling | `GalleryException` thrown by `Gallery`, caught in `Menu::run()` |

## Data file format
One record per line, fields separated by `|`, for example `1001|Starry Night|101|250000.00|Sold`.

## Testing
`python3 tests/run_tests.py` runs the 15 test cases (T01-T15) against the compiled program and prints PASS/FAIL.
