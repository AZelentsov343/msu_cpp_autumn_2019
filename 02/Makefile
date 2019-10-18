CC=g++
OPTS=-c --std=c++14 -O2

all: my_prog

run: my_prog
	./my_prog

test: my_prog
	./my_prog

my_prog: test.o Timer.o LinearAllocator.o Exception.o
	$(CC) -o my_prog $^

test.o: test.cpp LinearAllocator.h Exception.h Timer.h
	$(CC) test.cpp $(OPTS)

LinearAllocator.o: LinearAllocator.cpp Exception.h
	$(CC) LinearAllocator.cpp $(OPTS)

Exception.o: Exception.cpp
	$(CC) Exception.cpp $(OPTS)

Timer.o: Timer.cpp
	$(CC) Timer.cpp $(OPTS)

clean:
	rm -rf *.o *.dSYM my_prog