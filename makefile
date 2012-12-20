CFLAGS=-Wall -g

all: turing

turing: errors.o

clean:
	rm *.o turing
