CC = gcc -Wall -Wextra
all: MORPION

MORPION: contexte.o etats.o historique.o interface.o main.o plateau.o safeInput.o
	$(CC) $^ -o $@

%.o : %.c %.h
	$(CC) -c $< -o $@
