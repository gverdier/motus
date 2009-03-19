# Makefile écrit par Guillaume Verdier pour le projet Motus

DEBUG=true# Mettre à true pour compiler en mode debug (options de débogage)
RELEASE=false# Mettre à true pour compiler en mode release (options d'optimisation)

CC=gcc # Le compilateur C
#Les options de compilation
ifeq ($(DEBUG), true)
	ifeq ($(RELEASE), true)
		CFLAGS=-ansi -pedantic -W -Wall -Wno-unused-parameter `pkg-config --cflags gtk+-2.0` -g -O3
	else
		CFLAGS=-ansi -pedantic -W -Wall -Wno-unused-parameter `pkg-config --cflags gtk+-2.0` -g
	endif
else
	ifeq ($(RELEASE), true)
		CFLAGS=-ansi -pedantic -W -Wall -Wno-unused-parameter `pkg-config --cflags gtk+-2.0` -03
	else
		CFLAGS=-ansi -pedantic -W -Wall -Wno-unused-parameter `pkg-config --cflags gtk+-2.0`
	endif
endif
LDFLAGS=`pkg-config --libs gtk+-2.0` # Les options de l'éditeur de liens
EXEC=Motus # Le nom de l'exécutable
SRC=$(wildcard *.c) # Les fichiers source
OBJ=$(SRC:.c=.o) # Les fichiers objet

all: $(EXEC)

Motus.o: Jeu.h Affichage.h Types.h
Affichage.o: Jeu.h Affichage.h Types.h
Jeu.o: Affichage.h Types.h

$(EXEC): $(OBJ)
	@$(CC) $^ -o $@ $(LDFLAGS)

%.o:%.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -f *.o *~ ~*

mrproper: clean
	@rm -f $(EXEC)