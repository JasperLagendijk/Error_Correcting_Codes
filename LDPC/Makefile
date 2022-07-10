CC=gcc
CFLAGS=-I.
DEPS = ldpc.h parity.h
OBJ = parity.o ldpc.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ldpc: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
