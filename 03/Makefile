CC=g++
OPTS=-c --std=c++14 -O2

all: my_prog

test: my_prog
	./my_prog

my_prog: test.o Matrix.o
	$(CC) -o my_prog $^

test.o: test.cpp Matrix.h
	$(CC) test.cpp $(OPTS)

Matrix.o: Matrix.cpp
	$(CC) Matrix.cpp $(OPTS)

clean:
	rm -rf *.o *.dSYM my_prog