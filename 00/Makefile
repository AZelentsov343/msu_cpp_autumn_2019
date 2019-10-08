CC=g++
OPTS=-c --std=c++14 -O2

all: my_prog

run : my_prog
	./my_prog

my_prog: main.o sum_by_rows.o sum_by_columns.o timer.o 
	$(CC) -o my_prog main.o timer.o sum_by_rows.o sum_by_columns.o 

main.o: main.cpp sum_by_columns.h sum_by_rows.h
	$(CC) main.cpp $(OPTS)

sum_by_rows.o: sum_by_rows.cpp timer.o sum_by_rows.h
	$(CC) sum_by_rows.cpp $(OPTS)

sum_by_columns.o: sum_by_columns.cpp timer.o sum_by_columns.h
	$(CC) sum_by_columns.cpp $(OPTS)

timer.o: timer.cpp
	$(CC) timer.cpp $(OPTS)

clean:
	rm -rf *.o my_prog
