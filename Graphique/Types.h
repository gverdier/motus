/**
 * \file
 * \author Guillaume Verdier
 * \author Thomas Fock-Chow Tho
 * \brief Contient les définitions de types du jeu.
 */

#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gtk/gtk.h>

/**
 * \brief La taille maximale du pseudo (nom) d'un joueur.
 */
#define TAILLE_PSEUDO 20

/**
 * \brief Regroupe les options du jeu.
 * \warning Certaines options ne sont pas disponibles dans le mode console.
 * Elles sont seulement présentes pour ne pas avoir à modifier le type de données dans les versions suivantes.
 */
typedef struct _Options {
	int lettresParMot; /**< \brief Le nombre de lettres dans les mots tirés. */
	/**
	 * \brief Le nombre de joueurs.
	 * \warning Dans cette version, seul le mode 1 joueur est disponible.
	 */
	int nbJoueurs;
	int nbEssais; /**< \brief Le nombre d'essais laissés au joueur pour trouver un mot. */
	int modeDiabolique; /**< \brief Booléen indiquant si le mode diabolique est activé. */
	int tempsReponse; /**< \brief Le nombre de secondes laissées au joueur pour répondre en mode normal. */
	int bingo; /**< \brief Booléen indiquant si le bingo est activé. */
	/**
	 * \brief Booléen indiquant si les résultats doivent être ajoutés à l'historique. 
	 * \warning L'historique n'est pas disponible dans ce mode.
	 */
	int historique;
} Options;

/**
 * \brief Regroupe les données sur un joueur.
 */
typedef struct _Joueur {
	char nom[TAILLE_PSEUDO]; /**< \brief Le nom (pseudo) du joueur. */
	int grilleBingo[6][5]; /**< \brief La grille du bingo (5x5 cases de numéros, 3 cases de boules noires et 2 de neutres). */
	int score; /**< \brief Le score du joueur. */
} Joueur;

/**
 * \brief Les données sur le mot actuellement joue.
 */
typedef struct _Mot {
	char* mot; /**< \brief Le mot à trouver. */
	char** motsSaisis; /**< \brief Les mots saisis par l'utilisateur. */
	char** corrections; /**< \brief Corrections des mots saisis par l'utilisateur. */
	char* motTrouve; /**< \brief Tableau binaire indiquant pour chaque lettre si elle a été trouvée. */
	int essaisRestants; /**< \brief Le nombre d'essais restants. */
} Mot;

/**
 * \brief Les widgets pour la fenêtre GTK+.
 */
typedef struct _Widgets {
	GtkWidget* fenetre; /**< \brief La fenêtre principale. */
	GtkWidget* boxprincipale; /**< \brief La « boîte » principale de la fenêtre. */
	GtkWidget* scores; /**< \brief Label indiquant le(s) score(s) du (des) joueur(s). */
	GtkWidget* timerlabel; /**< \brief Label indiquant le temps restant pour répondre. */
	GtkWidget* table; /**< \brief Le tableau contenant la grille de motus. */
	GtkWidget*** casesevents; /**< \brief Tableau des « boîte d'évènement » utilisées pour changer la couleur de fond des cases de la grille de motus. */
	GtkWidget*** caseslabels; /**< \brief Tableau des labels utilisés pour afficher la lettre d'une case de motus. */
	GtkWidget* entree; /**< \brief La zone de saisie du mot. */
	GtkWidget* splash; /**< \brief L'écran de démarrage (splash screen).*/
	GTimer* timer; /**< \brief Timer pour pouvoir indiquer au joueur combien de temps il lui reste. */
} Widgets;

/**
 * \brief Regroupe toutes les données sur la partie en cours.
 */
typedef struct _Partie {
	Options options; /**< \brief Les options du jeu. */
	Joueur joueur1; /**< \brief Le premier joueur (le seul utilisé en mode solo). */
	Joueur joueur2; /**< \brief Le second joueur (pour le mode 2 joueurs seulement). */
	Mot motCourant; /**< \brief Le mot actuellement joué. */
	Widgets widgets; /**< \brief Les widgets GTK+. */
} Partie;

#endif