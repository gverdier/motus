/**
 * \file
 * \author Guillaume Verdier
 * \author Thomas Fock-Chow Tho
 * \brief Contient les définitions de fonctions et macros pour le module Jeu.
 */

#ifndef __JEU_H__
#define __JEU_H__

#include "Types.h"

/**
 * \brief Dans la correction d'un mot, indique d'une lettre qu'elle est à la bonne place.
 */
#define CORRECTION_BONNE_PLACE 'O'
/**
 * \brief Dans la correction d'un mot, indique d'une lettre qu'elle est présente (dans le motà mais mal placée.
 */
#define CORRECTION_MAUVAISE_PLACE 'X'
/**
 * \brief Dans la correction d'un mot, indique d'une lettre qu'elle n'est pas présente (dans le mot). 
 */
#define CORRECTION_NON_PRESENT '_'

/**
 * \brief Initialise une partie (options et joueurs).
 * \param partie La partie à initialiser.
 */
void jeu_initialiser (Partie* partie);

/**
 * \brief Appelé avant de terminer le programme, libère la mémoire allouée.
 * \param partie La partie dont on veut libérer la mémoire.
 */
void jeu_terminer (Partie* partie);

/**
 * \brief Initialise une partie en fonction des options.
 * \param partie La partie à initialiser.
 *
 * Il ne faut pas confondre jeu_initialiser avec cette fonction. 
jeu_initialiser doit être appelé au début du programme et initialise les options et joueurs avec les options par défaut.
Cette fonction initialise une partie au commencement de la partie (en fonction des options choisies par le joueur).
 */
int jeu_initialiserNouvellePartie (Partie* partie);

/**
 * \brief Fourni des options par défaut.
 * \return Les options par défaut.
 */
Options jeu_optionsDefaut (void);

/**
 * \brief Indique si un élément appartient à un tableau.
 * \param tab Le tableau dans lequel on cherche l'élément.
 * \param nbElt Le nombre d'éléments dans le tableau.
 * \param elt L'élément à enlever.
 */
int jeu_appartientTab (char tab[7], int nbElt, char elt);

/**
 * \brief Enlève un élément d'un tableau.
 * \param tab Le tableau dans lequel enlever l'élément.
 * \param nbElt Le nombre d'éléments dans le tableau.
 * \param elt L'élément à enlever.
 */
void jeu_enleverEltTab (char tab[7], int* nbElt, char elt);

/**
 * \brief Corrige un mot saisi par l'utilisateur.
 * \param menu_mot Le mot actuellement joué.
 * \param ligne Le numéro de l'essai du joueur.
 * \param taille_mot La taille d'un mot.
 * \return Booléen indiquant si le joueur a trouvé le mot.
 */
int jeu_corrigerMot (Mot* menu_mot, int ligne, int taille_mot);

/**
 * \brief Tire un mot au hasard dans le dictionnaire.
 * \param mot Il y est mis le mot tiré.
 * \param taille_mot La taille du mot que l'on veut.
 * \param diabolique Booléen indiquant si le mode diabolique est activé ou non.
 * \return 0 en cas de fin normale, un nombre positif en cas d'erreur.
 * \warning Cette fonction utilise la fonction rand. Il faut penser à appeler srand au début du programme.
 */
int jeu_tirerMot (char* mot, int taille_mot, int diabolique);

#endif
