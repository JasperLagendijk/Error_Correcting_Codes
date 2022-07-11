CC=gcc
CFLAGS=-I. -march=native -mtune=native
DEPS = ldpc.h parity.h
OBJ = parity.o ldpc.o 

%.o: %.c $(DEPS)
	$(CC) -O3 -c -o $@ $< $(CFLAGS)

ldpc: $(OBJ)
	$(CC) -O3 -o $@ $^ $(CFLAGS) 

.PHONY clean:

clean: 
	rm *.o ldpc
