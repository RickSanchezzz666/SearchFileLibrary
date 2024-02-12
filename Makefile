CC = g++
CFLAGS = -std=c++17 -Wall
LIBS = -lpthread -lc++fs

all: app

app: SearchFileApp/main.o SearchFileLibrary/ToSearchFile.o
    $(CC) $(CFLAGS) -o app SearchFileApp/main.o SearchFileLibrary/ToSearchFile.o $(LIBS)

SearchFileLibrary/ToSearchFile.o: SearchFileLibrary/ToSearchFile.cpp SearchFileLibrary/ToSearchFile.hpp
    $(CC) $(CFLAGS) -c SearchFileLibrary/ToSearchFile.cpp -o SearchFileLibrary/ToSearchFile.o

SearchFileApp/main.o: SearchFileApp/main.cpp
    $(CC) $(CFLAGS) -c SearchFileApp/main.cpp -o SearchFileApp/main.o

clean:
    rm -f SearchFileApp/*.o SearchFileLibrary/*.o app
