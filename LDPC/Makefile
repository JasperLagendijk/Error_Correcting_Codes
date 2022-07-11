CC=gcc
CFLAGS=-I. -march=native -mtune=native
DEPS = ldpc.h parity.h
OBJ = parity.o ldpc.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ldpc: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -O3

.PHONY clean:

clean: 
	rm *.o
