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
	/**
	 * \brief Le temps que la joueur a pour répondre.
	 * 
	 * En partie normale, il indiquera le temps (en secondes) laissé au joueur pour donner un mot.
	 * En super-partie, il indiquera le nombre de secondes total laissé au joueur pour trouver 10 mots.
	 */
	int tempsReponse;
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
	GtkWidget* joueur; /**< \brief Affiche le nom du joueur devant jouer. */
	GtkWidget* scores; /**< \brief Label indiquant le(s) score(s) du (des) joueur(s). */
	GtkWidget* affichageTimer; /**< \brief Pour afficher le timer (barre de progression). */
	GtkWidget* layout; /**< \brief Les différents éléments de la grille (images, lettres) y sont affichés. */
	GtkWidget*** casesimages; /**< \brief Les images de fond (elles seront créées à partir de imageDefaut, imageOK et imageMauvaisePos). */
	GtkWidget*** caseslabels; /**< \brief Tableau des labels utilisés pour afficher la lettre d'une case de motus. */
	GtkWidget* entree; /**< \brief La zone de saisie du mot. */
	GtkWidget* splash; /**< \brief L'écran de démarrage (splash screen).*/
	GdkPixbuf* imgDefaut; /**< \brief Correspond à l'image de fond par défaut. */
	GdkPixbuf* imgOK; /**< \brief Correspond à l'image de fond pour une lettre bien placée. */
	GdkPixbuf* imgMauvaisePos; /**< \brief Correspond à l'image de fond pour une lettre mal placée. */
	GTimer* timer; /**< \brief Timer pour pouvoir indiquer au joueur combien de temps il lui reste. */
} Widgets;

/**
 * \brief Regroupe toutes les données sur la partie en cours.
 */
typedef struct _Partie {
	Options options; /**< \brief Les options du jeu. */
	Joueur joueur1; /**< \brief Le premier joueur (le seul utilisé en mode solo). */
	Joueur joueur2; /**< \brief Le second joueur (pour le mode 2 joueurs seulement). */
	int joueurCourant; /**< \brief Le joueur courant : 1 <=> joueur1, 0 <=> joueur2. */
	Mot motCourant; /**< \brief Le mot actuellement joué. */
	int superPartie; /**< \brief Booléen indiquant si on joue une partie simple ou une super-partie. */
	Widgets widgets; /**< \brief Les widgets GTK+. */
} Partie;

#endif