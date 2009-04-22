#include "Jeu.h"
#include "Affichage.h"

void jeu_bingo_initialiser (Bingo *bingo)
{
	int i,j;

	/* partie mot */
	memset(&bingo->motusBingo,'*',5);

	/* partie grille */
	for(i=0;i<5;++i){
		for(j=0;j<5;++j){
			bingo->grilleBingo[i][j].gratte=0;
			bingo->grilleBingo[i][j].lettre='*';
		}
	}
}

void jeu_bingo_ajouterLettre (Bingo *bingo, char lettre)
{
	int numCase;

	do {
		numCase=(rand()%25);
	}while (bingo->grilleBingo[numCase/5][numCase%5].lettre!='*');

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

int jeu_bingo_motusTrouve (Bingo bingo)
{
	return ((bingo.motusBingo[0]=='M')&&(bingo.motusBingo[1]=='O')&&(bingo.motusBingo[2]=='T')&&(bingo.motusBingo[3]=='U')&&(bingo.motusBingo[4]=='S'));
}

int jeu_bingo_gratter (Bingo *bingo, int numCase)
{
	int i,j;

	i=(numCase-1)%5;
	j=(numCase-1)/5;

	bingo->grilleBingo[i][j].gratte=1;
	
	switch (bingo->grilleBingo[i][j].lettre) {
		case 'X':
			printf("Dommage, la case était noire.\n");
			return 2;
		case 'M':
			bingo->motusBingo[0]='M';
			printf("Sous la case il y avait un 'M'.\n");
			break;
		case 'O':
			bingo->motusBingo[1]='O';
			printf("Sous la case il y avait un 'O'.\n");
			break;
		case 'T':
			bingo->motusBingo[2]='T';
			printf("Sous la case il y avait un 'T'.\n");
			break;
		case 'U':
			bingo->motusBingo[3]='U';
			printf("Sous la case il y avait un 'U'.\n");
			break;
		case 'S':
			bingo->motusBingo[4]='S';
			printf("Sous la case il y avait un 'S'.\n");
			break;
	}

	return jeu_bingo_motusTrouve(*bingo);
}

Options jeu_optionsDefaut (void)
{
	Options opt;

	opt.lettresParMot=6;
	opt.nbJoueurs=1;
	opt.modeDiabolique=0;
	opt.tempsReponse=10;
	opt.bingo=0;

	return opt;
}

Partie jeu_initialiserPartie (int mode_2joueurs)
{
	Partie partie;

	partie.options=jeu_optionsDefaut();
	affichage_saisieJoueur(partie.joueur1.nom,1);	
	if (mode_2joueurs) affichage_saisieJoueur(partie.joueur2.nom,2);
	partie.joueur1.score=0;
	partie.joueur2.score=0;
	partie.joueurCourant=1;
	jeu_bingo_initialiser(&(partie.joueur1.bingo));
	jeu_bingo_distribuer(&(partie.joueur1.bingo));
	jeu_bingo_initialiser(&(partie.joueur2.bingo));
	jeu_bingo_distribuer(&(partie.joueur2.bingo));

	return partie;
}

int jeu_jeu_appartientTab (char tab[7], int nbElt, char elt)
{
	int i=0;

	while (i<nbElt) {
		if (tab[i]==elt) return 1;
		i++;
	}

	return 0;
}

void jeu_jeu_enleverEltTab (char tab[7],int* nbElt, char elt)
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

void jeu_jeu_initialiserTabMot (Mot* menu_mot)
{
	int i;
	int j;

	for (i=0;i<7;++i) menu_mot->mot[i]=0;
	for (i=0;i<6;++i) menu_mot->motTrouve[i]=0;
	for (i=0;i<5;++i) {
		for (j=0;j<7;++j) {
			menu_mot->motsSaisis[i][j]=0;
			menu_mot->corrections[i][j]=0;
		}
	}
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

int jeu_motPresent(const char* mot,int taille_mot,int diabolique)
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
		long m=(a + b) / 2;
		int res;
		
		fseek(dictio,m *(taille_mot + 1) - 1,SEEK_SET);
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

void jeu_jeu_initMotTrouve (Mot* menu_mot)
{
	int i;

	menu_mot->motTrouve[0]=1;

	for(i=1;i<6;i++) {
		menu_mot->motTrouve[i]=0;
	}
}

void jeu_ajouterLettre (Mot* menu_mot)
{
	int i=0;

	while (menu_mot->motTrouve[i]) {
		i++;
	}

	menu_mot->motTrouve[i]=1;
}

int jeu_jeu_corrigerMot (Mot* menu_mot,int ligne)
{
	char tab[7];
	int i;
	int nbElt=0;
	int nbBonsElt=0;

	for (i=0;i<6;i++) {
		tab[i]=menu_mot->mot[i];
		nbElt++;
	}

	if (!jeu_motPresent(menu_mot->motsSaisis[ligne],6,0)) {
		for (i=0;i<6;i++) {
			menu_mot->corrections[ligne][i]='*';
		}
		return 0;
	}	

	for (i=0;i<6;i++) {
		if (menu_mot->motsSaisis[ligne][i]==menu_mot->mot[i]) {
			menu_mot->corrections[ligne][i]='O';
			menu_mot->motTrouve[i]=1;
			nbBonsElt++;
			jeu_jeu_enleverEltTab(tab,&nbElt,menu_mot->motsSaisis[ligne][i]);
		}
	}

	for (i=0;i<6;i++) {
			if ((jeu_jeu_appartientTab(tab,nbElt,(*menu_mot).motsSaisis[ligne][i]))&&(menu_mot->corrections[ligne][i]!='O')) {
				menu_mot->corrections[ligne][i]='X';
				jeu_jeu_enleverEltTab(tab,&nbElt,menu_mot->motsSaisis[ligne][i]);
			}else{
				if (menu_mot->corrections[ligne][i]!='O') {
					menu_mot->corrections[ligne][i]='_';
				}
			}
	}

	return (nbBonsElt==6) ;
}
