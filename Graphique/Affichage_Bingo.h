/**
 * \file
 * \author Guillaume Verdier
 * \author Thomas Fock-Chow Tho
 * \brief Contient les définitions de fonctions et macros pour le module d'affichage, partie d'affichage de bingo.
 */

#ifndef __AFFICHAGE_BINGO_H__
#define __AFFICHAGE_BINGO_H__

#include "Jeu.h"

/**
 * \brief La taille des images de fond (carrées).
 */
#define TAILLE_IMAGE 32

/**
 * \brief Vérifie l'allocation d'un pointeur.
 * \param pointeur Le pointeur à vérifier.
 * \param message Le message à afficher en cas d'erreur.
 * \param action L'action à effectuer après affichage du message d'erreur en cas d'erreur.
 *
 * Le paramètre action doit être une ou plusieurs instructions C valides.
 * Par exemple, BINGO_VERIFIER_ALLOCATION(ptr,"Erreur",return 1;) affichera "Erreur" et renverra 1.
 */
#define BINGO_VERIFIER_ALLOCATION(pointeur,message,action) if (pointeur==NULL) {\
	affichage_erreur(message) ;\
	action\
}

/**
 * \brief Lance le bingo.
 * \param joueur Le joueur jouant le bingo.
 * \param parent La fenêtre parente.
 */
void affichage_bingo_lancer (Joueur* joueur, GtkWindow* parent);

/**
 * \brief Raffiche toutes les cases du bingo.
 * \param bingo Le bingo joué.
 *
 * Utilisé quand le joueur fait un MOTUS pour remettre l'affichage à 0.
 * Pour mettre à jour la grille après que le joueur ait tiré un mot, préférer
 * affichage_bingo_rafraichirLettres() qui est plus rapide.
 */
void affichage_bingo_rafficher (Bingo* bingo);

/**
 * \brief Appelée quand le joueur clique sur une case de la grille de bingo.
 * \param appelant Le widget ayant appelée la fonction de rappel.
 * \param bouton Le bouton sur lequel cliqué l'utilisateur.
 * \param joueur Le joueur jouant le bingo.
 */
void affichage_bingo_clicCase (GtkWidget* appelant, GdkEventButton* bouton, gpointer joueur);

/**
 * \brief Appelée quand le joueur a joué, affiche le bouton pour quitter le bingo.
 * \param joueur Le joueur jouant le bingo.
 */
void affichage_bingo_aJoue (Joueur* joueur);

/**
 * \brief Raffraichit l'affichage des lettres de la grille de bingo et du mot "MOTUS".
 * \param joueur Le joueur jouant le bingo.
 */
void affichage_bingo_rafraichirLettres (Joueur* joueur);

/**
 * \brief Terminer le bingo (ferme la fenêtre et rend le focus à la fenêtre principale).
 * \param appelant Le widget ayant appelé la fonction de rappel.
 * \param joueur Le joueur jouant le bingo.
 */
void affichage_bingo_terminer (GtkWidget* appelant, gpointer joueur);

/**
 * \brief Appelée quand le joueur essaie de fermer la fenêtre.
 * \param appelant Le widget ayant appelé la fonction de rappel.
 * \param joueur Le joueur jouant le bingo.
 */
gboolean affichage_bingo_terminerFenetre (GtkWidget* appelant, gpointer joueur);

/**
 * \brief Permet d'afficher un message d'erreur.
 * \param message Le message à afficher.
 *
 * On peut facilement changer l'affichage des messages d'erreurs avec cette fonction : affichage en console, écriture dans un fichier, boîte de dialogue, ...
 */
void affichage_erreur (const char* message);

#endif