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
	int nbJoueurs; /**< \brief Le nombre de joueurs. */
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
 * \brief Une case de grille de bingo.
 */
typedef struct {
	char lettre; /**< \brief La lettre de la case. */
	char gratte; /**< \brief Booléen indique si la case a été grattée. */
} Case_bingo;

typedef struct {
	GtkWidget* fenetre; /**< \brief La fenêtre de bingo. */
	GtkWidget* box; /**< \brief La box (verticale) contenant les autres widgets. */
	GtkWidget* boxMotus; /**< \brief La box (horizontale) contenant les lettres pour former le mot MOTUS. */
	GtkWidget* nom; /**< \brief Label affichant le nom du joueur. */
	GtkWidget* table; /**< \brief La table contenant les éléments de la grille de motus. */
	GtkWidget* events[5][5]; /**< \brief Les "Event Box" utilisées pour récupérer les clics de l'utilisateur sur les case de la grille de bingo. */
	GtkWidget* layouts[6][5]; /**< \brief Les différents éléments de la grille de motus (images, lettres) y seront affichés. */
	/**
	 * \brief Les images de fond de la grille.
	 * 
	 * Les 5x5 premières cases correspondent à la grille de tirage.
	 * Le 6ème rang correspond aux images de fond du mot MOTUS (les lettres tirées).
	 */
	GtkWidget* images[6][5];
	/**
	 * \brief Les lettres de la grille.
	 * 
	 * Les 5x5 premières cases correspondent à la grille de tirage.
	 * Le 6ème rang correspond aux lettres tirées du mot MOTUS.
	 */
	GtkWidget* lettres[6][5];
	GtkWidget* terminer; /**< \brief Bouton "Terminer" apparaissant quand le joueur a tiré une lettre pour finir le bingo. */
} BingoWidgets;

/**
 * \brief Regroupe les informations sur le bingo d'un joueur.
 */
typedef struct {
	Case_bingo grilleBingo[5][5]; /**< \brief La grille de bingo (où on tire les lettres). */
	char motusBingo[5]; /**< \brief Les lettres tirées (forme MOTUS). */
	BingoWidgets widgets; /**< \brief Les widgets GTK+ pour le bingo. */
} Bingo;

/**
 * \brief Regroupe les données sur un joueur.
 */
typedef struct _Joueur {
	char nom[TAILLE_PSEUDO]; /**< \brief Le nom (pseudo) du joueur. */
	Bingo bingo; /**< \brief Pour le bingo. */
	int score; /**< \brief Le score du joueur. */
	/**
	 * \brief Pointeur sur la variable "joueurCourant" de la structure Partie. 
	 *
	 * Ce pointeur est utilisé dans le bingo pour pouvoir changer de joueur sans que la "Partie" complète soit passée en paramètre.
	 */
	int* joueurCourant;
	/**
	 * \brief Pointeur sur la variable "nbJoueurs" de la structure Options. 
	 *
	 * Ce pointeur est utilisé dans le bingo pour savoir si le mode 2 joueurs est activé ou non sans passer la "Partie" complète.
	 */
	const int* nbJoueurs;
	/**
	 * \brief Pointeur sur le bingo de l'autre joueur.
	 *
	 * Ce pointeur est utilisé dans le bingo pour réinitialiser toutes les grilles en cas de "MOTUS".
	 */
	Bingo* bingoAutreJoueur;
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
	GtkWidget* suivant; /**< \brief Le bouton "suivant" pour passer au mot suivant. */
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
	double tempsRestant; /**< \brief Le temps restant au joueur pour jouer. */
	Widgets widgets; /**< \brief Les widgets GTK+. */
} Partie;

#endif