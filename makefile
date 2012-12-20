CFLAGS=-Wall -g

all: turing

turing: errors.o turingparser.o

clean:
	rm *.o turing
