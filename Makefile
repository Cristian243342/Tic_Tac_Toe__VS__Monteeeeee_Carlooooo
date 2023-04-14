# Copyright Lazar Cristian-Stefan 314CA 2022-2023
CC=gcc
CFLAGS=-Wall -Wextra -std=c99
DEPS= struct.h
OBJ= Monte.o
EXEC = monte

build: $(DEPS) $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS) -lm

%.o: %.c
	$(CC) -g -c -o $@ $< $(CFLAGS) -lm

.PHONY: clean

run:
	./$(EXEC)

clean:
	rm -f *.o $(EXEC)