# Copyright 2019 Lucia Fuentes Villodres
# This file is licensed under MIT License, as specified in the file LISENSE located at the root folder of this repository.

EXE = lcInstanceBinary
CC = gcc
FLAGS = -g -Wall

all: $(EXE)

lcInstanceBinary: lcInstanceBinary.o printLCInstance.o
	$(CC) $(FLAGS) $^ -o $@ 

lcInstanceBinary.o: lcInstanceBinary.c listColoring.h odd.h
	$(CC) -c $(FLAGS) $< -o $@ 

printLCInstance.o: printLCInstance.c listColoring.h
	$(CC) -c $(FLAGS) $< -o $@ 

clean:
	rm -f *.o *.exe
	rm -f lcInstanceBinary
