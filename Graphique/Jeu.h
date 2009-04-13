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
 * \brief La taille de l'historique, c'est-à-dire le nombre de joueurs enregistrés dans l'historique.
 */
#define TAILLE_HISTORIQUE 10

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
 * \return 0 en cas de fin normale, 1 en cas d'erreur.
 *
 * Il ne faut pas confondre jeu_initialiser avec cette fonction. 
 * jeu_initialiser doit être appelé au début du programme et initialise les options et joueurs avec les options par défaut.
 * Cette fonction initialise une partie au commencement de la partie (en fonction des options choisies par le joueur).
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
 * \brief Ajoute une lettre supplémentaire aux lettres affichées.
 * \param menu_mot Le mot joué.
 *
 * Cette fonction est appelée, dans le mode 2 joueurs, quand la main passe :
 * une lettre supplémentaire est alors dévoilée.
 */
void jeu_ajouterLettre (Mot* menumot);

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

/**
 * \brief Indique si un mot est présent dans le dictionnaire.
 * \param mot Le mot dans on veut savoir s'il est dans le dictionnaire.
 * \param taille_mot La taille du mot.
 * \param diabolique Booléen indiquant si le mode diabolique est activé.
 * \return
 * - -1 en cas d'erreur
 * - 0 si le mot n'est pas présent
 * - 1 si le mot est présent
 */
int jeu_motPresent(const char* mot, int taille_mot, int diabolique);

/**
 * \brief Réinitialise un mot (déjà alloué).
 * \param mot Le mot à réinitialiser.
 * \param nbEssais Le nombre d'essais qu'à le joueur pour trouver le mot.
 * \param taille_mot La taille d'un mot.
 * \param superPartie Booléen. Indique s'il s'agit d'une partie simple ou d'une super-partie.
 */
void jeu_reinitialiserMot (Mot* mot, int nbEssais, int taille_mot, int superPartie);

/**
 * \brief Sauvegarde la partie.
 * \param partie La partie à sauvegarder.
 * \param nom_fich Le nom du fichier dans lequel sauvegarder la partie.
 * \return 0 en cas de fin normale, 1 en cas d'erreur. 
 */
int jeu_sauvegarder (const Partie* partie, const char* nom_fich);

/**
 * \brief Charge une partie précédemment sauvegardée.
 * \param partie La partie où mettre les données chargées.
 * \param nom_fich Le nom du fichier dans lequel sont sauvegardées les données.
 * \return 0 en cas de fin normale, 1 en cas d'erreur.
 */
int jeu_charger (Partie* partie, const char* nom_fich);

/**
 * \brief Initialise une grille de bingo.
 * \param bingo La grille à initialiser.
 */
void jeu_bingo_initialiser (Bingo* bingo);

/**
 * \brief Distribue le bingo.
 * \param bingo La grille de bingo à distribuer.
 */
void jeu_bingo_distribuer (Bingo* bingo);

/** 
 * \brief Gratte une case de bingo.
 * \param bingo La grille de bingo dans laquelle on gratte.
 * \param numCase Le numéro de la case grattée (entre 1 et 25).
 * \return
 * - 1 si la case a déjà été grattée.
 * - 2 si c'est un jeton "noir" (les boules noires).
 * - 3 si c'est un jeton "joker" (le joueur rejoue).
 * - 0 sinon.
 */
int jeu_bingo_gratter (Bingo* bingo, int numCase);

/**
 * \brief Indique si un joueur a marqué un des meilleurs scores et si oui l'ajoute aux meilleurs scores.
 * \param nom Le nom du joueur.
 * \param score Le score du joueur.
 * \return 
 * - 1 si le joueur a marqué un des meilleurs scores.
 * - 0 si le joueur n'a pas marqué un des meilleurs scores.
 * - -1 en cas d'erreur.
 */
int jeu_historique_sauver (char* nom, int score);

/**
 * \brief Permet de récupérer les données de l'historique.
 * \param noms Les noms des joueurs.
 * \param scores Les scores des joueurs.
 * \return Le nombre de joueurs dans l'historique.
 */
int jeu_historique_donnees (char noms[TAILLE_HISTORIQUE][TAILLE_PSEUDO], int scores[TAILLE_HISTORIQUE]);

#endif
