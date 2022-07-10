CC=gcc
objects = ldpc.o parity.o

sim : $(objects)
	$(CC) -o  sim $(objects)
	
$(objects) : parity.h, ldpc.h


	
.PHONY : clean
clean :
	rm edit $(objects)
