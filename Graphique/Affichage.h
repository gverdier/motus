/**
 * \file
 * \author Guillaume Verdier
 * \author Thomas Fock-Chow Tho
 * \brief Contient les définitions de fonctions et macros pour le module d'affichage.
 */

#ifndef __AFFICHAGE_H__
#define __AFFICHAGE_H__

#include "Jeu.h"

/**
 * \brief La taille des images de fond (carrées).
 */
#define TAILLE_IMAGE 32

/**
 * \brief Vérifie qu'un pointeur a correctement été alloué. Si ce n'est pas le cas, affiche un message d'erreur puis termine le programme.
 */
#define VERIFIER_ALLOCATION(pointeur,message,partie) if(pointeur==NULL) {\
	affichage_erreur(message);\
	affichage_terminer(NULL,partie);\
}

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
 * \brief Rafraichit le fond de la grille de motus (à appeler, notamment, après modifications des couleurs dans les options).
 * \param partie Les données sur la partie en cours.
 */
void affichage_rafraichirFond (Partie* partie);

/**
 * \brief Permet d'afficher un message d'erreur.
 * \param message Le message à afficher.
 *
 * On peut facilement changer l'affichage des messages d'erreurs avec cette fonction : affichage en console, écriture dans un fichier, boîte de dialogue, ...
 */
void affichage_erreur (const char* message);

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
 * \brief Demande les options pour une nouvelle partie.
 * \param partie Les données sur la partie en cours.
 * \return Booléen indiquant si le joueur a saisi des options ou annulé.
 */
int affichage_saisieOptions (Partie* partie);

/**
 * \brief Commence une nouvelle super partie.
 * \param appelant Le widget appelant la fonction de rappel (NULL si elle n'est pas appelée par callback GTK+).
 * \param partie Les données sur la partie en cours.
 */
void affichage_nouvelleSuperPartie (GtkWidget* appelant, gpointer partie);

/**
 * \brief Propose au joueur de modifier les couleurs pour la grille de motus.
 * \param appelant Le widget appelant la fonction de rappel (NULL si elle n'est pas appelée par callback GTK+).
 * \param partie Les données sur la partie en cours.
 */
void affichage_changerCouleurs (GtkWidget* appelant, gpointer partie);

/**
 * \brief Propose à l'utilisateur de changer la couleur de fond par défaut.
 * \param appelant Le widget appelant la fonction de rappel (NULL si elle n'est pas appelée par callback GTK+).
 * \param partie Les données sur la partie en cours.
 */
void affichage_changerCouleurDefaut (GtkWidget* appelant, gpointer partie);

/**
 * \brief Propose à l'utilisateur de changer la couleur de fond pour les lettres bien placées.
 * \param appelant Le widget appelant la fonction de rappel (NULL si elle n'est pas appelée par callback GTK+).
 * \param partie Les données sur la partie en cours.
 */
void affichage_changerCouleurOK (GtkWidget* appelant, gpointer partie);

/**
 * \brief Propose à l'utilisateur de changer la couleur de fond pour les lettres mal placées.
 * \param appelant Le widget appelant la fonction de rappel (NULL si elle n'est pas appelée par callback GTK+).
 * \param partie Les données sur la partie en cours.
 */
void affichage_changerCouleurMauvaisePos (GtkWidget* appelant, gpointer partie);

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
 * \brief Termine le programme.
 * \param appelant Le widget appelant la fonction de rappel (NULL si elle n'est pas appelée par callback GTK+).
 * \param partie Les données sur la partie en cours.
 *
 * Appelle terminer_jeu qui libère la mémoire, termine GTK+ et le programme.
 */
void affichage_terminer (GtkWidget* appelant, gpointer partie);

#endif
