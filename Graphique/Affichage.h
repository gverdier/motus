/**
 * \file
 * \author Guillaume Verdier
 * \author Thomas Fock-Chow Tho
 * \brief Contient les définitions de fonctions et macros pour le module d'affichage.
 */

#ifndef __AFFICHAGE_H__
#define __AFFICHAGE_H__

#include "Jeu.h"
#include "Affichage_Bingo.h"

/**
 * \brief Vérifie qu'un pointeur a correctement été alloué. Si ce n'est pas le cas, affiche un message d'erreur puis termine le programme.
 */
#define VERIFIER_ALLOCATION(pointeur,message,partie) BINGO_VERIFIER_ALLOCATION(pointeur,message,affichage_terminer(NULL,partie);)
/* Ancienne implémentation
#define VERIFIER_ALLOCATION(pointeur,message,partie) if(pointeur==NULL) {\
	affichage_erreur(message);\
	affichage_terminer(NULL,partie);\
}
*/

/**
 * \brief Lance le jeu de Motus.
 * \param argc Le premier paramètre du main.
 * \param argv Le second paramètre du main.
 */
void lancer_motus (int* argc, char*** argv);

/**
 * \brief Initialise une partie.
 * \param partie La partie à initialiser.
 *
 * Cette fonction se charge d'initialiser la partie affichage de partie et appelle initialiser_jeu pour le reste.
 */
void affichage_initialiser (Partie* partie);

/**
 * \brief Crée le menu de la fenêtre.
 * \param partie Les données sur la partie en cours.
 */
void affichage_creerMenu (Partie* partie);

/**
 * \brief Tire un nouveau mot et réinitialise la structure Mot.
 * \param partie Les données sur la partie en cours.
 */
void affichage_nouveauMot (Partie* partie);

/**
 * \brief Affiche les indications sur le mot à trouver (les lettres déjà trouvées).
 * \param partie Les données sur la partie en cours.
 * \param ligne Le numéro de la ligne sur laquelle afficher les indications.
 */
void affichage_indications (Partie* partie, int ligne);

/**
 * \brief Termine une partie (enlève les grilles, etc.).
 * \param partie Les données sur la partie à terminer.
 *
 * Attention, il ne faut appeler cette fonction que si la partie est déjà commencée !
 */
void affichage_terminerPartie (Partie* partie);

/**
 * \brief Demande les options pour une nouvelle partie.
 * \param partie Les données sur la partie en cours.
 * \return Booléen indiquant si le joueur a saisi des options ou annulé.
 */
int affichage_saisieOptions (Partie* partie);

/**
 * \brief Demande les options pour une nouvelle super partie.
 * \param partie Les données sur la partie en cours.
 * \return Booléen indiquant si le joueur a saisi les options ou annulé.
 *
 * Cette fonction demande la saisie de toutes les options de la super-partie.
 * Dans le cas du passage d'une partie à une super-partie, il faut utiliser affichage_passagePartieSuperPartie()
 * qui demandera seulement les options différant de celles de la partie (temps de réponse).
 */
int affichage_saisieOptionsSuperPartie (Partie* partie);

/**
 * \brief Passe d'une partie à une super-partie.
 * \param partie Les données sur la partie en cours.
 */
void affichage_passagePartieSuperPartie (Partie* partie);

/**
 * \brief Met à jour les widgets affichant le score et le nom du joueur courant.
 * \param partie Les données sur la partie en cours (contenant les widgets, scores, noms, ...)
 */
void affichage_miseAJourScoreNom (const Partie* partie);

/* Fonctions de rappel (callback) */

/**
 * \brief Détruit l'écran de démarrage et lance la partie.
 * \param partie Les données sur la partie en cours.
 * \return Booléen utilisé par GTK+ pour savoir s'il faut arrêter le timer appelant cette fonction.
 */
gboolean affichage_splashDestroy (gpointer partie);

/**
 * \brief Affiche une à une les lettres du mot saisi par l'utilisateur.
 * \param partie Les données sur la partie en cours.
 * \return Booléen utilisé par GTK+ pour savoir s'il faut arrêter le timer appelant cette fonction.
 */
gboolean affichage_tableLettres (gpointer partie);

/**
 * \brief Mets à jour le label indiquant le temps de réponse restant et vérifie que le temps de réponse n'est pas écoulé.
 * \param partie Les données sur la partie en cours.
 * \return Booléen utilisé par GTK+ pour savoir s'il faut arrêter le timer appelant cette fonction.
 */
gboolean affichage_rafraichissementTimer (gpointer partie);

/**
 * \brief Commence une nouvelle partie.
 * \param appelant Le widget appelant la fonction de rappel (NULL si elle n'est pas appelée par callback GTK+).
 * \param partie Les données sur la partie en cours.
 */
void affichage_nouvellePartie (GtkWidget* appelant, gpointer partie);

/**
 * \brief Commence une nouvelle super partie.
 * \param appelant Le widget appelant la fonction de rappel (NULL si elle n'est pas appelée par callback GTK+).
 * \param partie Les données sur la partie en cours.
 *
 * Cette fonction ne doit être appelée que pour une nouvelle super-partie seule.
 * Pour une super-partie suivant une partie simple, il faut utiliser affichage_passagePartieSuperPartie().
 */
void affichage_nouvelleSuperPartie (GtkWidget* appelant, gpointer partie);

/**
 * \brief Appelé quand l'utilisateur demande la fin de la partie en cours
 * \param appelant Le widget appelant la fonction de rappel (NULL si elle n'est pas appelée par callback GTK+).
 * \param partie Les données sur la partie en cours.
 */
void affichage_menuTerminerPartie (GtkWidget* appelant, gpointer partie);

/**
 * \brief Affiche les meilleurs scores.
 * \param appelant Le widget appelant la fonction de rappel (NULL si elle n'est pas appelée par callback GTK+).
 * \param fenetre La fenêtre parente.
 */
void affichage_meilleursScores (GtkWidget* appelant, gpointer fenetre);

/**
 * \brief Affiche les règles du jeu.
 * \param appelant Le widget appelant la fonction de rappel (NULL si elle n'est pas appelée par callback GTK+).
 * \param partie Les données sur la partie en cours.
 */
void affichage_reglesJeu (GtkWidget* appelant, gpointer partie);

/**
 * \brief Affiche des informations à propos du jeu.
 * \param appelant Le widget appelant la fonction de rappel.
 * \param fenetre La fenêtre principale.
 */
void affichage_aPropos (GtkWidget* appelant, gpointer fenetre);

/**
 * \brief Fonction appelée quand le joueur saisi un mot : le vérifie et modifie l'affichage en conséquence.
 * \param entry La zone de saisie ayant appelée la fonction de rappel.
 * \param partie Les données sur la partie en cours.
 */
void affichage_saisieMot (GtkWidget* entry, gpointer partie);

/**
 * \brief Passe au mot suivant ou termine la partie si on a atteint les 10 mots.
 * \param appelant Le widget appelant la fonction de rappel (NULL si elle n'est pas appelée par callback GTK+).
 * \param partie Les données sur la partie en cours.
 */
void affichage_motSuivant (GtkWidget* appelant, gpointer partie);

/**
 * \brief Sauvegarder la partie.
 * \param appelant Le widget ayant appelé la fonction de rappel.
 * \param partie La partie à sauvegarder. 
 */
void affichage_sauvegarder (GtkWidget* appelant, gpointer partie);

/**
 * \brief Charge une partie.
 * \param appelant Le widget ayant appelé la fonction de rappel.
 * \param partie La partie où charger les données.
 */
void affichage_charger (GtkWidget* appelant, gpointer partie);

/**
 * \brief Termine le programme.
 * \param appelant Le widget appelant la fonction de rappel (NULL si elle n'est pas appelée par callback GTK+).
 * \param partie Les données sur la partie en cours.
 *
 * Appelle terminer_jeu qui libère la mémoire, termine GTK+ et le programme.
 */
void affichage_terminer (GtkWidget* appelant, gpointer partie);

#endif
