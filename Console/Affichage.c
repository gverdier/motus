#include "Affichage.h"
#include "Jeu.h"

int affichage_menu (void)
{
	int choix;

	printf("  ############################################  \n");
	printf("  #                  MOTUS                   #  \n");
	printf("  ############################################  \n");
	printf("  #     1 - NOUVELLE PARTIE                  #  \n");
	printf("  #     2 - NOUVELLE SUPER PARTIE            #  \n");
	printf("  #     3 - CHARGER PARTIE                   #  \n");
	printf("  #     4 - REGLES DU JEU                    #  \n");
	printf("  #     5 - QUITTER                          #  \n");
	printf("  ############################################  \n");
	printf("\n  VOTRE CHOIX : ");

	scanf("%d",&choix);
	while ((choix<1)||(choix>5)) {
		printf(" Choix non valide.");
		printf("\n  VOTRE CHOIX : ");
		scanf("%d",&choix);
	}

	return choix;
}

void affichage_bingo (Bingo bingo)
{
	int i,j;

	printf("   ---- ---- ---- ---- ---- \n");
	for(j=0;j<5;++j) {
		printf("  |");
		for(i=0;i<5;++i) {
			if (bingo.grilleBingo[i][j].gratte) {
				printf("  %c |",bingo.grilleBingo[i][j].lettre);
			}else{
				printf(" %d",(5*j+i+1));
				if ((5*j+i+1)<10) {
					printf(" ");
				}
				printf(" |");
			}
		}
		printf("\n");
		printf("   ---- ---- ---- ---- ---- \n");
	}

	printf("     --- --- --- --- --- \n");
	printf("    |");
	for(i=0;i<5;++i) {
		printf(" %c |",bingo.motusBingo[i]=='*'?' ':bingo.motusBingo[i]);
	}
	printf("\n");
	printf("     --- --- --- --- --- \n");
}

Options affichage_saisieOptions ()
{
	Options opt;

	opt.lettresParMot=6;

	printf("    SAISIE DES OPTIONS : \n");
	printf(" Veuillez choisir le nombre de lettres avec lequel vous voulez jouer (6,7,ou 8) : ");
	scanf("%ud",&(opt.lettresParMot));
	printf(" Veuillez choisir le nombre de joueurs avec lequel vous voulez jouer (1 ou 2) : ");
	scanf("%ud",&(opt.nbJoueurs));
	printf("Souhaîtez-vous activer le mode diabolique? (1-oui, 0-non) : ");
	scanf("%ud",&(opt.modeDiabolique));
	printf(" Veuillez choisir le temps que vous aurez pour trouver un mot en secondes (8,9,ou 10) : ");
	scanf("%ud",&(opt.tempsReponse));
	printf(" Veuillez indiquer si vous souhaîtez activer le bingo (1-oui, 0-non) : ");
	scanf("%ud",&(opt.bingo));
	printf("\n\n");

	printf(" RECAPITULATIF :\n %d lettres par mot\n %d joueurs\n mode diabolique : %s\n %d secondes par mot\n Bingo : %s\n",opt.lettresParMot,opt.nbJoueurs,opt.modeDiabolique?"Oui":"Non",opt.tempsReponse,opt.bingo?"Oui":"Non");

	return opt;
}

void affichage_saisieJoueur (char nom[],int nbjoueurs)
{
	printf("Veuillez entrer le nom du joueur %d : ",nbjoueurs);
	scanf("%s",nom);
}

void affichage_saisieMot (Mot* menu_mot)
{
	printf("Pour ce test, veuillez saisir le mot a trouver (6 lettres) : ");
	scanf("%s",menu_mot->mot);
}

void affichage_grilles (Mot menu_mot, int i)
{
	int j,k;

	printf(" --- --- --- --- --- ---            --- --- --- --- --- --- \n");
	for (j=0;j<i;j++) {
		printf("|");
		for(k=0;k<6;k++) {
			printf(" %c |",menu_mot.motsSaisis[j][k]);
		}

		printf("          ");
		printf("|");
		for(k=0;k<6;k++) {
			printf(" %c |",menu_mot.corrections[j][k]);
		}

		printf("\n");
		printf(" --- --- --- --- --- ---            --- --- --- --- --- --- \n");
	}

}

void affichage_eltTrouves (Mot menu_mot)
{
	int i;

	printf("                   --- --- --- --- --- --- \n");
	printf("                  |");
	for(i=0;i<6;i++) {
		if (menu_mot.motTrouve[i]!=0) {
			printf(" %c |",menu_mot.mot[i]);
		}else{
			printf("   |");
		}
	}
	
	printf("\n");
	printf("                   --- --- --- --- --- --- \n");
}

int affichage_sousMenu (Partie partie, int num_mot, int mode_2joueurs)
{
	int choix;

	printf("\n\n");
	printf(" ########################################################\n");
	printf(" ### MOT NUMERO %d  #####################################%c\n",num_mot,num_mot<10?'#':0);
	printf(" ########################################################\n");
	printf(" #    1-Chercher le mot                                 #    %s : %d points\n",partie.joueur1.nom,partie.joueur1.points);
	if (mode_2joueurs) {
		printf(" #    2-Sauvegarder                                     #    %s : %d points\n",partie.joueur2.nom,partie.joueur2.points);
	}else{
		printf(" #    2-Sauvegarder                                     #\n");
	}
	printf(" #    3-Charger une partie                              #\n");
	printf(" ########################################################\n");

	printf("\nVotre choix :");
	scanf("%d",&choix);

	while (choix<1&&choix>3) {
		printf("Erreur : ce choix n'est pas valable.\nVotre choix :");
		scanf("%d",&choix);
	}

	printf("\n");

	return choix;
}

int affichage_sousMenu2 (int num_mot)
{
	int choix;

	printf("\n\n");
	printf(" ########################################################\n");
	printf(" ### MOT NUMERO %d  #####################################%c\n",num_mot,num_mot<10?'#':0);
	printf(" ########################################################\n");
	printf(" #    1-Chercher le mot                                 #\n");
	printf(" #    2-Abandonner                                      #\n");
	printf(" ########################################################\n");

	printf("\nVotre choix :");
	scanf("%d",&choix);

	while (choix<1&&choix>2) {
		printf("Erreur : ce choix n'est pas valable.\nVotre choix :");
		scanf("%d",&choix);
	}

	printf("\n");

	return choix;
}

int affichage_saisieProp (char* prop)
{
	char tab[7];
	int i;
	int petit=0; /* Pour vérifier si le mot n'est pas trop petit  */

	scanf("%s",tab);
	fgetc(stdin);

	for(i=0;i<6;++i) {
		if (tab[i]=='\0') petit=1;
	}

	if (tab[6]!='\0') petit=1; /* En réalité ici le mot est trop grand, mais on veut lui faire la même chose que s'il était trop petit */

	if (petit==1) {
		for(i=0;i<6;i++) {
			tab[i]=' ';
		}
		tab[6]=0;
	}

	prop=strcpy(prop,tab);

	return jeu_motPresent(tab,6,0);
}

int affichage_phaseBingo (Joueur *joueur)
{
	int numCase;

	affichage_bingo(joueur->bingo);
	printf("Quelle case souhaîtez-vous gratter? ");
	scanf("%d",&numCase);
	while (numCase<1||numCase>25||joueur->bingo.grilleBingo[(numCase-1)%5][(numCase-1)/5].gratte==1) {
		printf("Cette case ne peut etre choisie, veuillez choisir une nouvelle case : ");
		scanf("%d",&numCase);
	}

	return jeu_bingo_gratter(&(joueur->bingo),numCase);

}

void affichage_pointsBingo (Partie *partie, int mode_2joueurs)
{
	int resBingo;

	if (partie->joueurCourant) {
		resBingo=affichage_phaseBingo(&(partie->joueur1));
	}else{  resBingo=affichage_phaseBingo(&(partie->joueur2)); }
	
	if (resBingo==2) printf("Vous avez gratté une case qui était noire. Si vous êtes en mode 2 joueurs, alors la main passe.\n");

	if (resBingo==2&&mode_2joueurs) {
		if (partie->joueurCourant) {
			partie->joueurCourant=0;
		}else{
			partie->joueurCourant=1;
		}
	}

	if (resBingo==1) {
		printf("Félicitations ! VOus venez de reconstituer le mot \"MOTUS\", les grilles vont maintenant être réinitialisées.\n");
		if (partie->joueurCourant) {
			partie->joueur1.points+=100;
		}else{ partie->joueur2.points+=100; }
		jeu_bingo_initialiser(&(partie->joueur1.bingo));
		jeu_bingo_distribuer(&(partie->joueur1.bingo));
		if (mode_2joueurs) {
			jeu_bingo_initialiser(&(partie->joueur2.bingo));
			jeu_bingo_distribuer(&(partie->joueur2.bingo));
		}
	}
}

int affichage_trouverMot (Partie* partie, int mode_2joueurs)
{
	int i;
	int leBonMot=0;
	int mot_valide;

	jeu_jeu_initMotTrouve(&((*partie).motCourant));

	for (i=0;(i<5)&&(leBonMot==0);i++) {
		printf(" ### Tentative %d\n",i+1);
		affichage_eltTrouves(partie->motCourant);
		printf("Joueur courant : %s\nVotre proposition : ",partie->joueurCourant?partie->joueur1.nom:partie->joueur2.nom);
		mot_valide=affichage_saisieProp(partie->motCourant.motsSaisis[i]);
		printf("  %s.\n",mot_valide?"Ce mot est valide":"Ce mot n'est pas valide");
		leBonMot=jeu_jeu_corrigerMot(&((*partie).motCourant),i);
		affichage_grilles(partie->motCourant,i+1);
		if ((!mot_valide)&&(mode_2joueurs)) {
			if (partie->joueurCourant) {
				partie->joueurCourant=0;
			}else{
				partie->joueurCourant=1;
			}
			printf("  Une nouvelle lettre est révélée.\n");
			jeu_ajouterLettre(&(partie->motCourant));
		}
	}

	if (leBonMot==1) {
		printf("Bravo, vous avez trouvé le bon mot qui était : %s\n",partie->motCourant.mot);
		if (partie->joueurCourant)  {(*partie).joueur1.points+=50;}
		else { (*partie).joueur2.points+=50;}
		return 1;
	}else{
		printf("Dommage, le mot était : %s\n",partie->motCourant.mot);
		if (mode_2joueurs) {
			if (partie->joueurCourant) {
				partie->joueurCourant=0;
			}else{
				partie->joueurCourant=1;
			}
		}
		return 0;
	}
}

int affichage_lancerPartie ()
{
	Partie partie;
	int i=0;
	int choix;
	int mode_2joueurs;

	printf("Voulez-vous activer le mode 2 joueurs? ( 0-Non , 1-Oui )  : ");
	scanf("%d",&mode_2joueurs);
	while ((mode_2joueurs!=0)&&(mode_2joueurs!=1)) {
		printf("Mauvais choix ( 0-Non , 1-Oui )   : ");
		scanf("%d",&mode_2joueurs);
	}

	partie=jeu_initialiserPartie(mode_2joueurs);

	while (i<20) {
		jeu_jeu_initialiserTabMot(&partie.motCourant);
		jeu_tirerMot(partie.motCourant.mot,6,0);
		printf("Le mot cherché est : %s.\n",partie.motCourant.mot);
		/*affichage_saisieMot(&partie.motCourant);*/
		choix=affichage_sousMenu(partie,i+1,mode_2joueurs);
		if (affichage_trouverMot(&partie,mode_2joueurs)) {
			affichage_pointsBingo(&partie,mode_2joueurs);
		}
			
		i++;
	}

	return partie.joueur1.points;
}

int affichage_lancerSuperPartie (int* points)
{
	Partie partie;
	int i=0;
	int choix=0;

	partie=jeu_initialiserPartie(0);
	*points=0;

	do {
		choix=affichage_sousMenu2(i+1);
		if (choix!=2) {
			jeu_jeu_initialiserTabMot(&partie.motCourant);
			printf(" #### Vous avez pour l'instant %d points.\n",*points);
			jeu_tirerMot(partie.motCourant.mot,6,0);
			(*points)+=affichage_trouverMot(&partie,0);
			i++;
		}
	}while ((choix!=2)&&(*points<10));

	return i;
}

void affichage_lancerJeu (void)
{
	int choix;
	int nbpoints;

	choix=affichage_menu();
	if (choix==1) {
		nbpoints=affichage_lancerPartie();
		printf("Fin de la partie. Vous finissez avec un total de %d points.\n",nbpoints);
	}
	if (choix==2) {
		int nb_essais=affichage_lancerSuperPartie(&nbpoints);
		printf("Fin de la super partie. Vous finissez avec un total de %d points en %d mots.\n",nbpoints,nb_essais);
	}
	

}

