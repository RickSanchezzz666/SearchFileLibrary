CC = g++

CXXFLAGS = -std=c++17

LIBS = -lstdc++fs -lpthread -latomic

OBJECTS = main.o ToSearchFile.o

TARGET = app.exe

all: app

app: $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $(TARGET)

main.o: SearchFileApp/main.cpp
	$(CC) $(CXXFLAGS) -c SearchFileApp/main.cpp -o main.o

ToSearchFile.o: SearchFileLibrary/ToSearchFile.cpp SearchFileLibrary/ToSearchFile.hpp
	$(CC) $(CXXFLAGS) -c SearchFileLibrary/ToSearchFile.cpp -o ToSearchFile.o

clean:
	rm -f $(OBJECTS) $(TARGET)