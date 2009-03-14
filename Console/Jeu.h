/**
 * \file
 * \author Guillaume Verdier
 * \author Thomas Fock-Chow Tho
 */

#ifndef __JEU_H__
#define __JEU_H__

/**
 * \brief La taille maximale du pseudo (nom) d'un joueur.
 */
#define TAILLE_PSEUDO 20

/**
 * \brief Pour simplifier les notations.
 */
typedef unsigned int uint ;

/**
 * \brief Regroupe les options du jeu.
 * \warning Certaines options ne sont pas disponibles dans le mode console.
 * Elles sont seulement présentes pour ne pas avoir à modifier le type de données dans les versions suivantes.
 */
typedef struct _Options {
	uint lettresParMot ; /**< \brief Le nombre de lettres dans les mots tirés. */
	/**
	 * \brief Le nombre de joueurs.
	 * \warning Dans cette version, seul le mode 1 joueur est disponible.
	 */
	uint nbJoueurs ;
	uint nbEssais ; /**< \brief Le nombre d'essais laissés au joueur pour trouver un mot. */
	uint modeDiabolique ; /**< \brief Booléen indiquant si le mode diabolique est activé. */
	uint tempsReponse ; /**< \brief Le nombre de secondes laissées au joueur pour répondre en mode normal. */
	uint bingo ; /**< \brief Booléen indiquant si le bingo est activé. */
	/**
	 * \brief Booléen indiquant si les résultats doivent être ajoutés à l'historique. 
	 * \warning L'historique n'est pas disponible dans ce mode.
	 */
	uint historique ;
} Options ;

/**
 * \brief Regroupe les données sur un joueur.
 */
typedef struct _Joueur {
	char nom[TAILLE_PSEUDO] ; /**< \brief Le nom (pseudo) du joueur. */
	int grilleBingo[6][5] ; /**< \brief La grille du bingo (5x5 cases de numéros, 3 cases de boules noires et 2 de neutres). */
} Joueur ;

/**
 * \brief Les données sur le mot actuellement joué.
 */
typedef struct _Mot {
	/**
	 * \brief Le mot à trouver.
	 * \warning Selon le type d'implémentation (statique ou dynamique), ce membre est susceptible de changer (char mot[...]).
	 */
	char* mot ;
	/**
	 * \brief Les mots saisis par l'utilisateur.
	 * \warning Selon le type d'implémentation (statique ou dynamique), ce membre est susceptible de changer (char mot[...][...]).
	 */
	char** motsSaisis ;
	uint essaisRestants ; /**< \brief Le nombre d'essais restants. */
} Mot ;

/**
 * \brief Regroupe toutes les données sur la partie en cours.
 */
typedef struct _Partie {
	Options options ; /**< \brief Les options du jeu. */
	Joueur joueur1 ; /**< \brief Le premier joueur (le seul utilisé en mode solo. */
	Joueur joueur2 ; /**< \brief Le second joueur (pour le mode 2 joueurs seulement). */
	Mot motCourant ; /**< \brief Le mot actuellement joué. */
} Partie ;

#endif
