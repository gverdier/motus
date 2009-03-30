# Makefile écrit par Guillaume Verdier pour le projet Motus

# Mettre à true pour compiler en mode debug (options de débogage)
DEBUG=true
# Mettre à true pour compiler en mode release (options d'optimisation)
RELEASE=false

CC=gcc # Le compilateur C
# Les options de compilation générales
CFLAGS=-ansi -pedantic -W -Wall -Wno-unused-parameter `pkg-config --cflags gtk+-2.0`
# Les options de compilation spécifiques au mode (debug/release)
# Si RELEASE et DEBUG sont tous deux à true, seul debug sera activé
ifeq ($(RELEASE), true)
	CFLAGS2=-03
endif
ifeq ($(DEBUG), true)
	CFLAGS2=-g
endif
# Les options de l'éditeur de liens
LDFLAGS=`pkg-config --libs gtk+-2.0`
EXEC=Motus # Le nom de l'exécutable
SRC=$(wildcard *.c) # Les fichiers source
OBJ=$(SRC:.c=.o) # Les fichiers objet

all: $(EXEC)

# Indique les dépendances (fichiers .h et Makefile) pour qu'en cas de modifications de ceux-ci,
# le programme puisse être recompilé sans avoir à faire de make clean
Motus.o: Jeu.h Affichage.h Types.h Makefile
Affichage.o: Jeu.h Affichage.h Types.h Makefile
Jeu.o: Affichage.h Types.h Makefile

$(EXEC): $(OBJ)
	@$(CC) $^ -o $@ $(LDFLAGS)

%.o:%.c
	@$(CC) -o $@ -c $< $(CFLAGS) $(CFLAGS2)

.PHONY: clean mrproper

clean:
	@rm -f *.o *~ ~*

mrproper: clean
	@rm -f $(EXEC)