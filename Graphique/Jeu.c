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
	int nbElt=taille_mot;
	int nbBonsElt=0;
	
	tab=malloc(taille_mot);
	memcpy(tab,menu_mot->mot,taille_mot);
	/*
	for (i=0;i<taille_mot;i++) {
		tab[i]=menu_mot->mot[i];
		nbElt++;
	}
	*/
	
	for (i=0;i<taille_mot;i++) {
		if (menu_mot->motsSaisis[ligne][i]==menu_mot->mot[i]) {
			menu_mot->corrections[ligne][i]=CORRECTION_BONNE_PLACE;
			menu_mot->motTrouve[i]=1;
			nbBonsElt++;
			jeu_enleverEltTab(tab,&nbElt,menu_mot->motsSaisis[ligne][i]);
		}
	}


	for (i=0;i<taille_mot;i++) {
			if ((jeu_appartientTab(tab,nbElt,(*menu_mot).motsSaisis[ligne][i]))&&(menu_mot->corrections[ligne][i]!='O')) {
				menu_mot->corrections[ligne][i]=CORRECTION_MAUVAISE_PLACE;
				jeu_enleverEltTab(tab,&nbElt,menu_mot->motsSaisis[ligne][i]);
			}else{
				if (menu_mot->corrections[ligne][i]!=CORRECTION_BONNE_PLACE) {
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

int jeu_tirerMot (char* mot, int taille_mot, int diabolique) {
	char nomDictio[19] ;
	FILE *dictio ;
	long taille ; /* La taille du fichier */
	long nblignes ; /* Le nombre de lignes dans le fichier */
	long numligne ; /* Le numero de la ligne choisie au hasard */
	
	if (diabolique)
		sprintf (nomDictio, "Dictionnaire%dD.txt", taille_mot) ;
	else
		sprintf (nomDictio, "Dictionnaire%d.txt", taille_mot) ;
	if (!(dictio = fopen (nomDictio, "rt"))) {
		fprintf (stderr, "Impossible d'ouvrir le dictionnaire %s.\n", nomDictio) ;
		return 1 ;
	}
	fseek (dictio, 0, SEEK_END) ;
	taille = ftell (dictio) ;
	nblignes = taille / (taille_mot + 1) ; /* Chaque lettre prend 1 octet + 1 octet pour le saut de ligne */
	numligne = rand () % nblignes ;
	fseek (dictio, numligne * (taille_mot + 1) - 1, SEEK_SET) ; /* On va a la ligne correspondante */
	fscanf (dictio, "%s", mot) ;
	fclose (dictio) ;
	return 0 ;
}

int jeu_motPresent(const char* mot, int taille_mot, int diabolique)
{
	char nomDictio[19];
	FILE *dictio;
	char* motCourant;
	long taille; /* La taille du fichier */
	long nblignes; /* Le nombre de lignes dans le fichier */
	long a,b; /* Pour la recherche dichotomique */
	
	motCourant=malloc(taille_mot + 1);
	if (!motCourant) {
		fprintf(stderr,"Impossible d'allouer dynamiquement un mot.\n");
		return -1;
	}
	if (diabolique)
		sprintf(nomDictio,"Dictionnaire%dD.txt",taille_mot);
	else
		sprintf(nomDictio,"Dictionnaire%d.txt",taille_mot);
	if (!(dictio=fopen(nomDictio,"rt"))) {
		fprintf(stderr,"Impossible d'ouvrir le dictionnaire %s.\n",nomDictio);
		return -1;
	}
	fseek(dictio,0,SEEK_END);
	taille=ftell(dictio);
	nblignes=taille /(taille_mot + 1); /* Chaque lettre prend 1 octet + 1 octet pour le saut de ligne */
	a=0;
	b=nblignes - 1;
	while (b-a>1) {
		long m=(a+b)/2;
		int res;
		
		fseek(dictio,m*(taille_mot + 1) - 1,SEEK_SET);
		fscanf(dictio,"%s",motCourant);
		res=strcmp(motCourant,mot);
		if (!res) {
			free(motCourant);
			fclose(dictio);
			return 1;
		}
		if (res < 0)
			a=m;
		else
			b=m;
	}
	free(motCourant);
	fclose(dictio);
	return 0;
}