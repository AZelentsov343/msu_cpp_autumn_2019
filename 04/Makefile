CC=g++
OPTS=-c --std=c++14 -O2

all: my_prog

test: my_prog
	./my_prog

my_prog: main.o BigInt.o
	$(CC) -o my_prog $^

main.o: main.cpp BigInt.h
	$(CC) main.cpp $(OPTS)

BigInt.o: BigInt.cpp
	$(CC) BigInt.cpp $(OPTS)

clean:
	rm -rf *.o *.dSYM my_prog