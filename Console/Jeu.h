/**
 * \file
 * \author Guillaume Verdier
 * \author Thomas Fock-Chow Tho
 * \brief Contient les définitions de fonctions et macros pour le module Jeu.
 */

#ifndef __JEU_H__
#define __JEU_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Types.h"

/**
 * \brief Initialise une grille de bingo.
 * \param bingo La grille à initialiser.
 */
void jeu_bingo_initialiser (Bingo *bingo);

/**
 * \brief Ajoute une lettre à une grille de Bingo non remplie.
 * \param bingo La grille en cours de remplissage.
 * \param lettre La lettre à ajouter.
 */
void jeu_bingo_ajouterLettre (Bingo *bingo, char lettre);

/**
 * \brief Distribue le bingo.
 * \param bingo La grille de bingo à distribuer.
 */
void jeu_bingo_distribuer (Bingo *bingo);

/**
 * \brief Indique si le mot MOTUS est complété sur une grille.
 * \param bingo La grille à analyser.
 * \return 
 * -  1 si le mot MOTUS est complété.
 * -  2 sinon.
 */
int jeu_bingo_motusTrouve (Bingo bingo);

/** 
 * \brief Gratte une case de bingo.
 * \param bingo La grille de bingo dans laquelle on gratte.
 * \param numCase Le numéro de la case grattée (entre 1 et 25).
 * \return
 * - 1 si la case grattée permet de finir le mot MOTUS.
 * - 2 si c'est un jeton "noir" (les boules noires).
 * - 0 sinon.
 */
int jeu_bingo_gratter (Bingo *bingo, int numCase);

/** 
 * \brief Génère automatiquement des options de jeu dites classiques.
 * - 6 lettres par mot
 * - 1 joueur
 * - Mode diabolique desactivé
 * - Un temps de réponse initialisé à 10 par défaut (non utilisé en mode Console)
 * - Mode bingo initialisé à 1 par défaut (toujours utilisé en mode Console).
 * \return Une structure Options contenant les options classiques.
 */
Options jeu_optionsDefaut (void);

/**
 * \brief Demande à l'utilisateur le nom des joueurs et initialise les options par défaut (sert aux tests).
 * \param mode_2joueurs indique si le mode 2 joueurs doit etre activé ou pas.
 * \return une structure Partie contenant les options par défaut.
 */
Partie jeu_initialiserPartie (int mode_2joueurs);

/** 
 * \brief Savoir si un élément donné appartient à un tableau d'éléments (appliqué ici aux char).
 * \param tab Le tableau d'éléments.
 * \param nbElt Le nombre d'éléments que contient tab
 * \param.elt L'élément donné.
 * \return
 * - 1 si l'élément appartient au tableau.
 * - 0 sinon.
 */
int jeu_jeu_appartientTab (char tab[7], int nbElt, char elt);

/** 
 * \brief Enlever un élément d'un tableau d'élément (celui-ci doit appartenir au tableau).
 * \param tab Le tableau d'éléments.
 * \param nbElt Le nombre d'éléments que contient tab
 * \param elt L'élément donné.
 */
void jeu_jeu_enleverEltTab (char tab[7],int* nbElt, char elt);

/** 
 * \brief Initialise une structure Mot (à utiliser avant chaque recherche de mot dans le jeu).
 * \param menu_mot La structure à initialiser.
 */
void jeu_jeu_initialiserTabMot (Mot* menu_mot);

/**
 * \brief Tire un mot au hasard dans le dictionnaire.
 * \param mot Il y est mis le mot tiré.
 * \param taille_mot La taille du mot que l'on veut.
 * \param diabolique Booléen indiquant si le mode diabolique est activé ou non.
 * \return 0 en cas de fin normale, un nombre positif en cas d'erreur.
 * \warning Cette fonction utilise la fonction rand. Il faut penser à appeler srand au début du programme.
 */
int jeu_tirerMot (char* mot, int taille_mot, int diabolique);

/**
 * \brief Indique si un mot est présent dans le dictionnaire.
 * \param mot Le mot dont on veut savoir s'il est dans le dictionnaire.
 * \param taille_mot La taille du mot.
 * \param diabolique Booléen indiquant si le mode diabolique est activé.
 * \return
 * - -1 en cas d'erreur
 * - 0 si le mot n'est pas présent
 * - 1 si le mot est présent
 */
int jeu_motPresent(const char* mot,int taille_mot,int diabolique);

/**
 * \brief Sert à initialiser les variables servant à la phase de recherche d'un mot (pour éviter les erreurs). Il faut l'utiliser avant chaque recherche.
 * \param menu_mot structure à initialiser.
 */
void jeu_jeu_initMotTrouve (Mot* menu_mot);

/** 
 * \brief Ajoute une lettre supplémentaire aux lettres affichées.
 * \param menu_mot Le mot joué.
 *
 * Cette fonction est appelée, dans le mode 2 joueurs, quand la main passe :
 * une lettre supplémentaire est alors dévoilée.
 */
void jeu_ajouterLettre (Mot* menu_mot);

/**
 * \brief Corrige un mot saisi par l'utilisateur.
 * \param menu_mot Le mot actuellement joué.
 * \param ligne Le numéro de l'essai du joueur.
 * \return Booléen indiquant si le joueur a trouvé le mot.
 */
int jeu_jeu_corrigerMot (Mot* menu_mot,int ligne);

#endif 