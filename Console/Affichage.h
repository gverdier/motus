/**
 * \file
 * \author Guillaume Verdier
 * \author Thomas Fock-Chow Tho
 * \brief Contient les définitions de fonctions et macros pour le module d'affichage.
 */

#ifndef __AFFICHAGE_H__
#define __AFFICHAGE_H__

#include <stdio.h>

#include "Types.h"

/** 
 * \brief Affiche le menu principal du programme et demande un choix à l'utilisateur.
 * \return Le choix de l'utilisateur.
 */
int affichage_menu (void);

/**
 * \brief Affiche une grille de bingo.
 * \param bingo La grille à afficher.
 */
void affichage_bingo (Bingo bingo);

/** 
 * \brief Permet la saisie d'options du jeu.
 * \return Une structure Options contenant les options saisies.
 */
Options affichage_saisieOptions (void);

/** 
 * \brief Permet la saisie d'un nom de joueur.
 * \param nom La variable contenant le nom du joueur.
 * \param nbjoueur Le numéro d'identification du joueur concerné.
 */
void affichage_saisieJoueur (char nom[], int nbjoueurs);

/** 
 * \brief Saisir un mot à chercher (pour les tests).
 * \param menu_mot La structure Mot contenant le mot qui sera saisi.
 */
void affichage_saisieMot (Mot* menu_mot);

/**
 * \brief Affiche sur la console la grille de jeu.
 * \param menu_mot Le mot actuellement joué.
 * \param i Indique que l'on joue la i-ème proposition (rend l'affichage optimal).
 */
void affichage_grilles (Mot menu_mot, int i);

/**
 * \brief Affiche sur la console un récapitulatif des lettres trouvées.
 * \param menu_mot Le mot actuellement joué.
 */
void affichage_eltTrouves (Mot menu_mot);

/**
 * \brief Affiche sur la console un sous menu (à faire apparaitre entre chaque recherche de mot). L'utilisateur doit ensuite taper son choix.
 * \param partie La partie actuellement jouée.
 * \param num_mot Le i-ème mot que le doit maintenant jouer.
 * \param mode_2joueurs Indique si le mode 2 joueurs est activé.
 * \return La valeur du choix de l'utilisateur.
 */
int affichage_sousMenu (Partie partie, int num_mot, int mode_2joueurs);

/**
 * \brief Affiche sur la console un sous menu (uniquement pour la super partie)..
 * \param num_mot le i-ème mot que l'on doit maintenant jouer.
 * \return La valeur du choix de l'utilisateur.
 */
int affichage_sousMenu2 (int num_mot);

/**
 * \brief Gère la saisie d'une proposition de mot lors d'une recherche (vérifie notamment sa validité).
 * \param prop la variable qui contiendra la proposition.
 * \return
 * - -1 en cas d'erreur
 * - 0 si le mot n'est pas valide
 * - 1 si le mot est valide.
 */
int affichage_saisieProp (char* prop);

/**
 * \brief Lance une phase de bingo (est utilisée par points_bingo).
 * \param joueur la structure Joueur concernée par la phase de Bingo
 * \return 
 * - 1 si la case grattée permet de finir le mot MOTUS.
 * - 2 si c'est un jeton "noir" (les boules noires).
 * - 0 sinon.
 */
int affichage_phaseBingo (Joueur *joueur);

/**
 * \brief Lance une phase de Bingo et attribue des points ou pénalise.
 * \param partie la partie concernée.
 * \param mode_2joueurs indique si le mode 2 joueurs est actif ou pas.
 */
void affichage_pointsBingo (Partie *partie, int mode_2joueurs);

/**
 * \brief Lance une phase de de recherche d'un mot (dans sa globalité).
 * \param partie la partie concernée.
 * \param mode_2joueurs indique si le mode 2 joueurs est actif ou pas.
 * \return
 * - 1 si le mot a été trouvé.
 * - 0 sinon.
 */
int affichage_trouverMot (Partie* partie, int mode_2joueurs);

/**
 * \brief Lance une partie normale dans sa globalité.
 * \return Le nombre de points fait par le joueur 1.
 */
int affichage_lancerPartie (void);

/**
 * \brief Lance une super partie dans sa globalité.
 * \param points la variable qui contient le nombre de points du joueur.
 * \return le nombre de mots qui ont été joué.
 */
int affichage_lancerSuperPartie (int* points);

/**
 * \brief Lance le jeu dans sa globalité.
 */
void affichage_lancerJeu (void);

#endif

