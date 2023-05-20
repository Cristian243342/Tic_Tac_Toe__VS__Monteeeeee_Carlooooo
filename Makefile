# Copyright Lazar Cristian-Stefan 314CA 2022-2023
CC=gcc
CFLAGS=-Wall -Wextra -std=c99
DEPS= struct.h
OBJ= Monte.o
OBJ1= sys_monte.o
EXEC = monte

.PHONY: clean, clean_build, clean_sys

build: $(DEPS) clean_sys $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -g -O3 -o $@ $^ $(CFLAGS) -lm

%.o: %.c
	$(CC) -g -c -O3 -o $@ $< $(CFLAGS) -lm

build_sys: $(DEPS) clean_build sys_$(EXEC)

sys_$(EXEC): $(OBJ1)
	$(CC) -g -O3 -o $(EXEC) $^ $(CFLAGS) -lm

run:
	./$(EXEC)

clean:
	rm -f *.o $(EXEC)

clean_sys:
	rm -f $(OBJ1)

clean_build:
	rm -f $(OBJ)
