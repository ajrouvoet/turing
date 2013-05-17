CC=gcc
CFLAGS=-Wall -O0 -g -fprofile-arcs -ftest-coverage
LFLAGS=-Wall -O0 -g -fprofile-arcs -ftest-coverage

all: turing

turing: errors.o turingparser.o

clean:
	rm *.o *.gcno *.gcda turing
