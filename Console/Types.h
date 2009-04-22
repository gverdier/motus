/**
 * \file
 * \author Guillaume Verdier
 * \author Thomas Fock-Chow Tho
 * \brief Contient les définitions de types du jeu.
 */

#ifndef __TYPEMOTUS_H__
#define __TYPEMOTUS_H__


/**
 * \brief La taille maximale du pseudo (nom) d'un joueur.
 */
#define TAILLE_PSEUDO 20

/**
 * \brief Pour simplifier les notations.
 */
typedef unsigned int uint ;

/**
 * \brief Une case de grille de bingo.
 */
typedef struct {
	char lettre; /**< \brief La lettre de la case. */
	char gratte; /**< \brief Booléen indique si la case a été grattée. */
} Case_bingo;

/**
 * \brief Regroupe les informations sur le bingo d'un joueur.
 */
typedef struct {
	Case_bingo grilleBingo[5][5]; /**< \brief La grille de bingo (où on tire les lettres). */
	char motusBingo[5]; /**< \brief Les lettres tirées (forme MOTUS). */
} Bingo;

/**
 * \brief Regroupe les options du jeu.
 * \warning Certaines options ne sont pas disponibles dans le mode console.
 * Elles sont seulement présentes pour ne pas avoir à modifier le type de données dans les versions suivantes.
 */
typedef struct _Options {
	uint lettresParMot; /**< \brief Le nombre de lettres dans les mots tirés. */
	/**
	 * \brief Le nombre de joueurs.
	 * \warning Dans cette version, seul le mode 1 joueur est disponible.
	 */
	uint nbJoueurs;
	uint nbEssais; /**< \brief Le nombre d'essais laissés au joueur pour trouver un mot. */
	uint modeDiabolique; /**< \brief Booléen indiquant si le mode diabolique est activé. */
	uint tempsReponse; /**< \brief Le nombre de secondes laissées au joueur pour répondre en mode normal. */
	uint bingo; /**< \brief Booléen indiquant si le bingo est activé. */
	/**
	 * \brief Booléen indiquant si les résultats doivent être ajoutés à l'historique. 
	 * \warning L'historique n'est pas disponible dans ce mode.
	 */
	uint historique;
} Options ;

/**
 * \brief Regroupe les données sur un joueur.
 */
typedef struct _Joueur {
	char nom[TAILLE_PSEUDO] ; /**< \brief Le nom (pseudo) du joueur. */
	Bingo bingo ; /**< \brief La grille du bingo (5x5) et le tableau récupitulatif. */
	int points ;
} Joueur ;

/**
 * \brief Les données sur le mot actuellement joue.
 */
typedef struct _Mot {
	char mot[7]; /**< \brief Le mot à trouver. */
	char motsSaisis[5][7]; /**< \brief Les mots saisis par l'utilisateur. */
	char corrections[5][7]; /**< \brief Corrections des mots saisis par l'utilisateur. */
	char motTrouve[6]; /**< \brief Tableau binaire indiquant pour chaque lettre si elle a été trouvée. */
	uint essaisRestants; /**< \brief Le nombre d'essais restants. */
} Mot ;

/**
 * \brief Regroupe toutes les données sur la partie en cours.
 */
typedef struct _Partie {
	Options options ; /**< \brief Les options du jeu. */
	Joueur joueur1 ; /**< \brief Le premier joueur (le seul utilisé en mode solo). */
	Joueur joueur2 ; /**< \brief Le second joueur (pour le mode 2 joueurs seulement). */
	int joueurCourant ; /**< \brief Indique si le joueur 1 joue (prend pour valeur 1 ou 0)   */
	Mot motCourant ; /**< \brief Le mot actuellement joué. */
} Partie ;

#endif