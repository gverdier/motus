# Makefile écrit par Guillaume Verdier pour le projet Motus

# Mettre à true pour compiler en mode debug (options de débogage)
DEBUG=true
# Mettre à true pour compiler en mode release (options d'optimisation)
RELEASE=true

CC=gcc # Le compilateur C
# Les options de compilation générales
CFLAGS=-ansi -pedantic -W -Wall -Wno-unused-parameter `pkg-config --cflags gtk+-2.0`
# Les options de compilation spécifiques au mode (debug/release)
ifeq ($(RELEASE), true)
	CFLAGS+=-O3
endif
ifeq ($(DEBUG), true)
	CFLAGS+=-g
endif
# Les options de l'éditeur de liens
LDFLAGS=`pkg-config --libs gtk+-2.0`
# Le nom de l'exécutable
EXEC=Motus
# Les fichiers source
SRC=$(wildcard *.c)
# Les fichiers objet
OBJ=$(SRC:.c=.o)

all: $(EXEC)

# Indique les dépendances (fichiers .h et Makefile) pour qu'en cas de modifications de ceux-ci,
# le programme puisse être recompilé sans avoir à faire de make clean
Motus.o: Jeu.h Affichage.h Types.h Makefile
Affichage.o: Jeu.h Affichage_Bingo.h Affichage.h Types.h Makefile
Affichage_Bingo.o: Jeu.h Affichage_Bingo.h Types.h Makefile
Jeu.o: Affichage.h Types.h Makefile

$(EXEC): $(OBJ)
	@$(CC) $^ -o $@ $(LDFLAGS)

%.o:%.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper full exec

full: clean all

exec: all
	@./"$(EXEC)"

clean:
	@rm -f *.o *~ ~*

mrproper: clean
	@rm -f $(EXEC)