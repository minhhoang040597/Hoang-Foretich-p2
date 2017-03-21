all: lab05

lab05: lab05.o
	g++ -Wall -g -lncurses -o lab05 lab05.o

stat.o: stat.cpp
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -lncurses -c lab05.cpp

clean: 
	rm -f lab05
	rm -f lab05.o