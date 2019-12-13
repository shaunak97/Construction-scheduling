risingCity: main.o
	g++ main.o -o risingCity

main.o: main.cpp
	g++ -c main.cpp

target: dependencies
	action
