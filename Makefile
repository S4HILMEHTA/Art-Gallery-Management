CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g
TARGET   = ArtGallery
SOURCES  = main.cpp Menu.cpp Gallery.cpp FileManager.cpp Artwork.cpp Artist.cpp Customer.cpp \
           Person.cpp Entity.cpp Exhibition.cpp Sale.cpp Utils.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe
