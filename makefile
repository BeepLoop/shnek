all: snek

snek: main.cpp
	g++ -o main main.cpp -lncurses
