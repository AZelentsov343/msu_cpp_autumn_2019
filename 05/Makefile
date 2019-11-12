CC=g++
OPTS=-c --std=c++14 -O2

all: my_prog

test: my_prog
	./my_prog

my_prog: main.o
	$(CC) -o my_prog $^

main.o: main.cpp
	$(CC) main.cpp $(OPTS)

clean:
	rm -rf *.o *.dSYM my_prog