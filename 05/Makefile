CC=g++
OPTS=-c --std=c++14 -O2

all: my_prog

test: my_prog
	./my_prog

my_prog: main.o Serializer.o
	$(CC) -o my_prog $^

main.o: main.cpp Serializer.o
	$(CC) main.cpp $(OPTS)

Serializer.o: Serializer.cpp
	$(CC) Serializer.cpp $(OPTS)

clean:
	rm -rf *.o *.dSYM my_prog