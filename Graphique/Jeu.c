#include "Jeu.h"

Options jeu_optionsDefaut (void)
{
	Options opt;

	opt.lettresParMot=6;
	opt.nbJoueurs=1;
	opt.nbEssais=5;
	opt.modeDiabolique=0;
	opt.tempsReponse=10;
	opt.bingo=0;
	opt.historique=0;

	return opt;
}


void jeu_initialiser (Partie* partie)
{
	partie->options=jeu_optionsDefaut();
	partie->joueur1.nom[0]='\0';

}

int jeu_appartientTab (char tab[7], int nbElt, char elt)
{
	int i=0;

	while (i<nbElt) {
		if (tab[i]==elt) return 1;
		i++;
	}

	return 0;
}



void jeu_enleverEltTab (char tab[7], int* nbElt, char elt)
{
	int i;

	for (i=0;i<(*nbElt);i++) {
		if (tab[i]==elt) {
			for (i=i;i+1<(*nbElt);i++) {
				tab[i]=tab[i+1];
			}
		}
	}

	*nbElt-=1;

}


int jeu_corrigerMot (Mot* menu_mot, int ligne, int taille_mot)
{
	char* tab;
	int i;
	int nbElt=0;
	int nbBonsElt=0;
	
	tab=malloc(taille_mot);

	for (i=0;i<taille_mot;i++) {
		tab[i]=menu_mot->mot[i];
		nbElt++;
	}

	for (i=0;i<taille_mot;i++) {
		if (menu_mot->motsSaisis[ligne][i]==menu_mot->mot[i]) {
			menu_mot->corrections[ligne][i]=CORRECTION_BONNE_PLACE;
			menu_mot->motTrouve[i]=1;
			nbBonsElt++;
			jeu_enleverEltTab(tab,&nbElt,menu_mot->motsSaisis[ligne][i]);
		} else {
			if (jeu_appartientTab(tab,nbElt,(*menu_mot).motsSaisis[ligne][i])) {
				menu_mot->corrections[ligne][i]=CORRECTION_MAUVAISE_PLACE;
				jeu_enleverEltTab(tab,&nbElt,menu_mot->motsSaisis[ligne][i]);
			} else {
				menu_mot->corrections[ligne][i]=CORRECTION_NON_PRESENT;
			}
		}
	}
	
	free(tab);

	return nbBonsElt==taille_mot;
}

void jeu_terminer (Partie* partie)
{
	if (partie->motCourant.mot) {
		int i;
		
		free(partie->motCourant.mot);
		free(partie->motCourant.motTrouve);
		for (i=0;i<partie->options.nbEssais;++i) {
			free(partie->motCourant.motsSaisis[i]);
			free(partie->motCourant.corrections[i]);
		}
		free(partie->motCourant.motsSaisis);
		free(partie->motCourant.corrections);
	}
}

int jeu_initialiserNouvellePartie (Partie* partie)
{
	int i;
	
	/* Allocations dynamiques pour le mot courant. */
	partie->motCourant.mot=malloc(partie->options.lettresParMot+1);
	if (!partie->motCourant.mot)
		return 1;
	partie->motCourant.motTrouve=calloc(1,partie->options.lettresParMot+1);
	if (!partie->motCourant.mot)
		return 1;
	partie->motCourant.motsSaisis=malloc(partie->options.nbEssais*sizeof(char*));
	if (!partie->motCourant.motsSaisis)
		return 1;
	partie->motCourant.corrections=malloc(partie->options.nbEssais*sizeof(char*));
	if (!partie->motCourant.corrections)
		return 1;
	for (i=0;i<partie->options.nbEssais;++i) {
		partie->motCourant.motsSaisis[i]=malloc(partie->options.lettresParMot+1);
		if (!partie->motCourant.motsSaisis[i])
			return 1;
		partie->motCourant.corrections[i]=malloc(partie->options.lettresParMot+1);
		if (!partie->motCourant.corrections[i])
			return 1;
	}
	
	partie->joueur1.score=0;
	
	return 0;
}
