parseUpdate: main.o PocketBookUpdate.o
	g++ -g main.o PocketBookUpdate.o -o parseUpdate
PocketBook.o: PocketBookUpdate.cpp PocketBookUpdate.h
	g++ -g -c PocketBookUpdate.cpp -o PocketBookUpdate.o
main.o: main.cpp
	g++ -g -c main.cpp -o main.o
