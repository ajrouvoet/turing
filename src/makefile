CC=gcc
CFLAGS=-Wall
LFLAGS=-Wall

default: all

coverage: CFLAGS=-Wall -O0 -g -fprofile-arcs -ftest-coverage
coverage: LFLAGS=-Wall -O0 -g -fprofile-arcs -ftest-coverage
coverage: all

all: turing

turing: errors.o turingparser.o

clean:
	rm *.o *.gcno *.gcda turing
