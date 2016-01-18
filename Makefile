CC=gcc
CFLAGS=-I.
DEPS = Graph.h
OBJ = Graph.o GraphTest.o
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
GraphTest.exe: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
