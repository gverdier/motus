#include "Jeu.h"

Options jeu_optionsDefaut (void)
{
	Options opt;

	opt.lettresParMot=6;
	opt.nbJoueurs=1;
	opt.nbEssais=5;
	opt.modeDiabolique=0;
	opt.tempsReponse=10;
	opt.bingo=1;

	return opt;
}


void jeu_initialiser (Partie* partie)
{
	partie->options=jeu_optionsDefaut();
	memset(&partie->motCourant,0,sizeof(Mot));
	partie->joueur1.bingoAutreJoueur=&partie->joueur2.bingo;
	partie->joueur2.bingoAutreJoueur=&partie->joueur1.bingo;
	partie->joueur1.nom[0]=partie->joueur2.nom[0]='\0';
	partie->joueur1.joueurCourant=partie->joueur2.joueurCourant=&partie->joueurCourant;
	partie->joueur1.nbJoueurs=partie->joueur2.nbJoueurs=&partie->options.nbJoueurs;
	partie->joueurCourant=1;
	jeu_bingo_initialiser(&partie->joueur1.bingo);
	jeu_bingo_initialiser(&partie->joueur2.bingo);
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
		memset(&partie->motCourant,0,sizeof(Mot));
	}
}

int jeu_initialiserNouvellePartie (Partie* partie)
{
	int i,j;
	
	/* Allocations dynamiques pour le mot courant. */
	partie->motCourant.mot=malloc(partie->options.lettresParMot+1);
	if (!partie->motCourant.mot)
		return 1;
	partie->motCourant.motTrouve=malloc(partie->options.lettresParMot+1);
	if (!partie->motCourant.mot)
		return 1;
	memset(partie->motCourant.motTrouve,0,partie->options.lettresParMot);
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
		for (j=0;j<partie->options.lettresParMot;++j)
			partie->motCourant.corrections[i][j]=CORRECTION_NON_PRESENT;
	}
	
	partie->joueur1.score=partie->joueur2.score=0;
	
	return 0;
}

void jeu_ajouterLettre (Mot* menu_mot)
{
	int i=0;

	while (menu_mot->motTrouve[i++]);
	
	menu_mot->motTrouve[i-1]=1;
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

void jeu_reinitialiserMot (Mot* mot, int nbEssais, int taille_mot, int superPartie)
{
	int i,j;
	
	for (i=0;i<taille_mot;++i) {
		mot->motTrouve[i]=0;
		for(j=0;j<nbEssais;++j) {
			mot->corrections[j][i]=CORRECTION_NON_PRESENT;
			mot->motsSaisis[j][i]='\0';
		}
	}
	mot->motTrouve[0]=1;
	mot->motTrouve[3]=superPartie;
	mot->essaisRestants=nbEssais;
}

int jeu_sauvegarder (const Partie* partie, const char* nom_fich)
{
	FILE* fich;
	int i;
	
	fich=fopen(nom_fich,"wb");
	if (!fich)
		return 1;
	
	fwrite(&partie->options,sizeof(Options),1,fich);
	fwrite(&partie->joueur1,sizeof(Joueur),1,fich);
	fwrite(&partie->joueur2,sizeof(Joueur),1,fich);
	
	fwrite(partie->motCourant.mot,1,partie->options.lettresParMot,fich);
	for (i=0;i<partie->options.nbEssais;++i)
		fwrite(partie->motCourant.motsSaisis[i],1,partie->options.lettresParMot,fich);
	for (i=0;i<partie->options.nbEssais;++i)
		fwrite(partie->motCourant.corrections[i],1,partie->options.lettresParMot,fich);
	fwrite(partie->motCourant.motTrouve,1,partie->options.lettresParMot,fich);
	fwrite(&partie->motCourant.essaisRestants,sizeof(int),1,fich);
	
	fwrite(&partie->joueurCourant,sizeof(int),1,fich);
	fwrite(&partie->superPartie,sizeof(int),1,fich);
	fwrite(&partie->tempsRestant,sizeof(double),1,fich);
	
	fclose(fich);
	return 0;
}

int jeu_charger (Partie* partie, const char* nom_fich)
{
	FILE* fich;
	int i;
	int score1,score2;
	
	fich=fopen(nom_fich,"rb");
	if (!fich)
		return 1;
	
	jeu_terminer(partie); /* Désalloue la mémoire si besoin */
	
	fread(&partie->options,sizeof(Options),1,fich);
	fread(&partie->joueur1,sizeof(Joueur),1,fich);
	fread(&partie->joueur2,sizeof(Joueur),1,fich);
	/* Les scores vont êtres effacés par l'appel à jeu_initialiserNouvellePartie */
	score1=partie->joueur1.score;
	score2=partie->joueur2.score;
	
	if (jeu_initialiserNouvellePartie(partie))
		return 1;
	partie->joueur1.score=score1;
	partie->joueur2.score=score2;
	
	fread(partie->motCourant.mot,1,partie->options.lettresParMot,fich);
	for (i=0;i<partie->options.nbEssais;++i)
		fread(partie->motCourant.motsSaisis[i],1,partie->options.lettresParMot,fich);
	for (i=0;i<partie->options.nbEssais;++i)
		fread(partie->motCourant.corrections[i],1,partie->options.lettresParMot,fich);
	fread(partie->motCourant.motTrouve,1,partie->options.lettresParMot,fich);
	fread(&partie->motCourant.essaisRestants,sizeof(int),1,fich);
	
	fread(&partie->joueurCourant,sizeof(int),1,fich);
	fread(&partie->superPartie,sizeof(int),1,fich);
	fread(&partie->tempsRestant,sizeof(double),1,fich);
	
	fclose(fich);
	return 0;
}

void jeu_bingo_initialiser (Bingo *bingo)
{
	int i,j;

	memset(&bingo->motusBingo,'*',5);
	for(i=0;i<5;++i){
		for(j=0;j<5;++j){
			bingo->grilleBingo[i][j].gratte=0;
			bingo->grilleBingo[i][j].lettre='*';
		}
	}
	jeu_bingo_distribuer(bingo);
}

/* Utilisé seulement pour jeu_bingo_distribuer. */
void jeu_bingo_ajouterLettre (Bingo *bingo, char lettre)
{
	int numCase;

	do {
		numCase=(rand()%25);
	} while (bingo->grilleBingo[numCase/5][numCase%5].lettre!='*');

	bingo->grilleBingo[numCase/5][numCase%5].lettre=lettre;
}

void jeu_bingo_distribuer (Bingo *bingo)
{
	int k;

	for(k=0;k<4;++k) {
		jeu_bingo_ajouterLettre(bingo,'M');
		jeu_bingo_ajouterLettre(bingo,'O');
		jeu_bingo_ajouterLettre(bingo,'T');
		jeu_bingo_ajouterLettre(bingo,'U');
		jeu_bingo_ajouterLettre(bingo,'S');
	}

	for(k=0;k<5;++k) {
		jeu_bingo_ajouterLettre(bingo,'X');
	}
}

int jeu_bingo_gratter (Bingo *bingo, int numCase)
{
	int i,j;

	i=(numCase-1)%5;
	j=(numCase-1)/5;
	
	if (bingo->grilleBingo[i][j].gratte)
		return 1;
	bingo->grilleBingo[i][j].gratte=1;
	
	switch (bingo->grilleBingo[i][j].lettre) {
		case 'X':
			return 2;
		case '*':
			return 3;
		case 'M':
			bingo->motusBingo[0]='M';
			break;
		case 'O':
			bingo->motusBingo[1]='O';
			break;
		case 'T':
			bingo->motusBingo[2]='T';
			break;
		case 'U':
			bingo->motusBingo[3]='U';
			break;
		case 'S':
			bingo->motusBingo[4]='S';
			break;
	}

	return 0;
}

int jeu_historique_sauver (char* nom, int score)
{
	FILE* hist;
	int tmpscore; /* Pour stocker un score lu dans l'historique */
	
	hist=fopen("partie.hist", "rb");
	if (!hist) {
		/* Le fichier n'existe pas : on le crée et on y met le joueur. */
		hist=fopen("partie.hist", "wb");
		if (!hist)
			return -1;
		fwrite(nom,1,TAILLE_PSEUDO,hist);
		fwrite(&score,sizeof(int),1,hist);
		fclose(hist);
		return 1;
	}
	/* On récupère le score du moins bon joueur (le dernier enregistré) */
	fseek(hist,-sizeof(int),SEEK_END);
	fread(&tmpscore,sizeof(int),1,hist);
	if (score<tmpscore) {
		/* Le joueur n'est pas assez bon... */
		fclose(hist);
		return 0;
	}
	/* Il faut récupérer les données puis les réécrire en rajoutant le joueur
	 * et éventuellement en supprimant le moins bon joueur. */
	{
		FILE* hist2; /* On y recopie l'historique */
		char tmpnom[TAILLE_PSEUDO]; /* Pour stocker le nom d'un joueur récupéré dans l'historique */
		int nbecrits;
		char joueurEcrit=0;
		
		hist2=fopen(".temp.hist","wb");
		if (!hist2) {
			fclose(hist);
			return -1;
		}
		rewind(hist);
		for (nbecrits=0;nbecrits<TAILLE_HISTORIQUE&&fread(tmpnom,1,TAILLE_PSEUDO,hist);) {
			fread(&tmpscore,sizeof(int),1,hist);
			if(tmpscore<=score&&!joueurEcrit) {
				/* Il faut d'abord placer le joueur */
				fwrite(nom,1,TAILLE_PSEUDO,hist2);
				fwrite(&score,sizeof(int),1,hist2);
				++nbecrits;
				joueurEcrit=1;
			}
			if (nbecrits<TAILLE_HISTORIQUE) {
				fwrite(tmpnom,1,TAILLE_PSEUDO,hist2);
				fwrite(&tmpscore,sizeof(int),1,hist2);
				++nbecrits;
			}
		}
		fclose(hist2);
		fclose(hist);
		remove("partie.hist");
		rename(".temp.hist","partie.hist");
	}
	return 1;
}

int jeu_historique_donnees (char noms[TAILLE_HISTORIQUE][TAILLE_PSEUDO], int scores[TAILLE_HISTORIQUE])
{
	FILE* hist;
	int i;
	
	hist=fopen("partie.hist","rb");
	if (!hist)
		return 0;
	for (i=0;fread(noms[i],1,TAILLE_PSEUDO,hist);++i)
		fread(&scores[i],sizeof(int),1,hist);
	return i;
}
