#include "Affichage.h"

void lancer_motus (int* argc, char*** argv)
{
	Partie partie;
	gtk_init(argc,argv);
	srand(time(NULL));
	affichage_initialiser(&partie);
	gtk_main();
}

void affichage_initialiser (Partie* partie)
{
	/* Création des widgets */
	partie->widgets.fenetre=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	VERIFIER_ALLOCATION(partie->widgets.fenetre,"Impossible de créer la fenêtre.\n",partie)
	gtk_window_set_default_size(GTK_WINDOW(partie->widgets.fenetre),200,200);
	partie->widgets.boxprincipale=gtk_vbox_new(FALSE,0);
	VERIFIER_ALLOCATION(partie->widgets.boxprincipale,"Impossible de créer la box principale.\n",partie)
	
	affichage_creerMenu(partie);
	
	partie->widgets.scores=gtk_label_new("");
	VERIFIER_ALLOCATION(partie->widgets.scores,"Impossible de créer le label d'affichage des scores.\n",partie)
	
	/* La grille de motus et les cases ne seront crées qu'au début de la partie (pas au début du jeu). */
	partie->widgets.table=NULL;
	partie->widgets.casesevents=NULL;
	partie->widgets.caseslabels=NULL;
	
	/* Connexion des signaux aux fonctions de rappel */
	g_signal_connect(G_OBJECT(partie->widgets.fenetre),"delete-event",G_CALLBACK(affichage_terminer),partie);
	
	gtk_box_pack_start(GTK_BOX(partie->widgets.boxprincipale),partie->widgets.scores,FALSE,FALSE,0);
	gtk_container_add(GTK_CONTAINER(partie->widgets.fenetre),partie->widgets.boxprincipale);
	gtk_widget_show_all(partie->widgets.fenetre);
	
	jeu_initialiser(partie);
}

void affichage_creerMenu (Partie* partie)
{
	GtkWidget* barremenu;
	GtkWidget* menu;
	GtkWidget* eltmenu;
	
	barremenu=gtk_menu_bar_new();
	VERIFIER_ALLOCATION(barremenu,"Impossible de créer la barre de menu.\n",partie)
	
	menu=gtk_menu_new();
	VERIFIER_ALLOCATION(menu,"Impossible de créer le menu pour le menu \"Jeu\".\n",partie)
	
	eltmenu=gtk_menu_item_new_with_label("Nouvelle partie");
	VERIFIER_ALLOCATION(eltmenu,"Impossible de créer l'élément de menu \"Nouvelle partie\".\n",partie)
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),eltmenu);
	g_signal_connect(G_OBJECT(eltmenu),"activate",G_CALLBACK(affichage_nouvellePartie),partie);
	
	eltmenu=gtk_menu_item_new_with_label("Nouvelle super partie");
	VERIFIER_ALLOCATION(eltmenu,"Impossible de créer l'élément de menu \"Nouvelle super partie\".\n", partie)
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),eltmenu);
	g_signal_connect(G_OBJECT(eltmenu),"activate",G_CALLBACK(affichage_nouvelleSuperPartie),partie);
	
	eltmenu=gtk_menu_item_new_with_label("Quitter");
	VERIFIER_ALLOCATION(eltmenu,"Impossible de créer l'élément de menu \"Quitter\".\n",partie)
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),eltmenu);
	g_signal_connect(G_OBJECT(eltmenu),"activate",G_CALLBACK(affichage_terminer),partie);
	
	eltmenu=gtk_menu_item_new_with_label("Jeu");
	VERIFIER_ALLOCATION(eltmenu,"Impossible de créer le menu \"Jeu\".\n",partie)
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(eltmenu),menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(barremenu),eltmenu);
	
	menu=gtk_menu_new();
	VERIFIER_ALLOCATION(menu,"Impossible de créer le menu pour le menu \"Aide\".\n",partie)
	
	eltmenu=gtk_menu_item_new_with_label("Règles du jeu");
	VERIFIER_ALLOCATION(menu,"Impossible de créer l'élément de menu \"Règles du jeu\".\n",partie)
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),eltmenu);
	g_signal_connect(G_OBJECT(eltmenu),"activate",G_CALLBACK(affichage_reglesJeu),partie);
	
	eltmenu=gtk_menu_item_new_with_label("À propos");
	VERIFIER_ALLOCATION(menu,"Impossible de créer l'élément de menu \"À propos\".\n",partie)
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),eltmenu);
	g_signal_connect(G_OBJECT(eltmenu),"activate",G_CALLBACK(affichage_aPropos),partie->widgets.fenetre);
	
	eltmenu=gtk_menu_item_new_with_label("Aide");
	VERIFIER_ALLOCATION(eltmenu,"Impossible de créer le menu \"Aider\".\n",partie);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(eltmenu),menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(barremenu),eltmenu);
	
	gtk_box_pack_start(GTK_BOX(partie->widgets.boxprincipale),barremenu,FALSE,FALSE,0);
}

void affichage_erreur (const char* message)
{
	g_printerr(message);
}

void affichage_nouvellePartie (GtkWidget* appelant, gpointer param_partie)
{
	Partie* partie=(Partie*)param_partie;
	GdkColor OK,mauvaisePosition,defaut;
	char scoresLabel[12];
	int i,j;
	
	if (!affichage_saisieOptions (partie))
		return;
	
	/* Allocations dynamiques des tableaux nécessaires au jeu. */
	if (jeu_initialiserNouvellePartie(partie))
		affichage_erreur("Erreur à l'allocation dynamique des champs de la structure Mot.\n");
	partie->widgets.casesevents=malloc(partie->options.nbEssais*sizeof(GtkWidget**));
	VERIFIER_ALLOCATION(partie->widgets.casesevents,"Impossible d'allouer le tableau de GtkEventBox.\n",partie);
	partie->widgets.caseslabels=malloc(partie->options.nbEssais*sizeof(GtkWidget**));
	VERIFIER_ALLOCATION(partie->widgets.caseslabels,"Impossible d'allouer le tableau de GtkEventBox.\n",partie);
	for(i=0;i<partie->options.nbEssais;++i) {
		partie->widgets.casesevents[i]=malloc(partie->options.lettresParMot*sizeof(GtkWidget*));
		VERIFIER_ALLOCATION(partie->widgets.casesevents[i],"Impossible d'allouer le tableau de GtkEventBox.\n",partie);
		partie->widgets.caseslabels[i]=malloc(partie->options.lettresParMot*sizeof(GtkWidget*));
		VERIFIER_ALLOCATION(partie->widgets.caseslabels[i],"Impossible d'allouer le tableau de GtkEventBox.\n",partie);
	}
	
	affichage_nouveauMot(partie);
	
	OK.red=COULEUR_OK_RED;
	OK.green=COULEUR_OK_GREEN;
	OK.blue=COULEUR_OK_BLUE;
	mauvaisePosition.red=COULEUR_MAUVAISE_POS_RED;
	mauvaisePosition.green=COULEUR_MAUVAISE_POS_GREEN;
	mauvaisePosition.blue=COULEUR_MAUVAISE_POS_BLUE;
	defaut.red=COULEUR_DEFAUT_RED;
	defaut.green=COULEUR_DEFAUT_GREEN;
	defaut.blue=COULEUR_DEFAUT_BLUE;
	
	sprintf(scoresLabel,"Score : %d",partie->joueur1.score);
	gtk_label_set_label(GTK_LABEL(partie->widgets.scores),scoresLabel);
	
	partie->widgets.table=gtk_table_new(partie->options.nbEssais,partie->options.lettresParMot,TRUE);
	VERIFIER_ALLOCATION(partie->widgets.table,"Impossible d'allouer le tableau.\n",partie);
	for (i=0;i<partie->options.nbEssais;++i) {
		for(j=0;j<partie->options.lettresParMot;++j) {
			partie->widgets.casesevents[i][j]=gtk_event_box_new();
			VERIFIER_ALLOCATION(partie->widgets.casesevents[i][j],"Impossible de créer une « EventBox ».\n",partie);
			partie->widgets.caseslabels[i][j]=gtk_label_new(" ");
			VERIFIER_ALLOCATION(partie->widgets.caseslabels[i][j],"Impossible de créer un label.\n",partie);
			
			gtk_widget_modify_bg(partie->widgets.casesevents[i][j],GTK_STATE_NORMAL,&defaut);
			gtk_container_add(GTK_CONTAINER(partie->widgets.casesevents[i][j]),partie->widgets.caseslabels[i][j]);
			gtk_table_attach_defaults(GTK_TABLE(partie->widgets.table),partie->widgets.casesevents[i][j],j,j+1,i,i+1);
		}
	}
	gtk_box_pack_start(GTK_BOX(partie->widgets.boxprincipale),partie->widgets.table,FALSE,FALSE,0);
	
	partie->widgets.entree=gtk_entry_new();
	g_signal_connect(G_OBJECT(partie->widgets.entree),"activate",G_CALLBACK(affichage_saisieMot),partie);
	gtk_box_pack_start(GTK_BOX(partie->widgets.boxprincipale),partie->widgets.entree,FALSE,FALSE,0);
	
	gtk_widget_show_all(partie->widgets.fenetre);
	
	affichage_indications(partie,0); /* Indications pour la première saisie. */
}

void affichage_nouveauMot (Partie* partie)
{
	/* Dans le mode final, le mot sera tiré dans un dictionnaire. */
	/*
	GtkWidget* dialogue;
	GtkWidget* entry;

	dialogue=gtk_dialog_new_with_buttons("Mot à chercher",GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_STOCK_OK,GTK_RESPONSE_OK,NULL);
	VERIFIER_ALLOCATION(dialogue,"Impossible de créer la boîte de dialogue.\n",partie);
	entry=gtk_entry_new_with_max_length(partie->options.lettresParMot);
	VERIFIER_ALLOCATION(entry,"Impossible de créer la zone de saisie.\n",partie);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialogue)->vbox),entry,FALSE,FALSE,0);

	gtk_widget_show_all(GTK_DIALOG(dialogue)->vbox);
	gtk_dialog_run(GTK_DIALOG(dialogue));

	strcpy(partie->motCourant.mot,(char*)gtk_entry_get_text(GTK_ENTRY(entry)));

	gtk_widget_destroy(dialogue);
	*/
	jeu_tirerMot(partie->motCourant.mot,partie->options.lettresParMot,partie->options.modeDiabolique);
	
	partie->motCourant.essaisRestants=partie->options.nbEssais;
	partie->motCourant.motTrouve[0]=1; /* On connait dès le début la première lettre du mot. */
}

int affichage_saisieOptions (Partie* partie)
{
	/* ATTENTION : pour l'instant, seul le mode UN JOUEUR est disponible. */
	/* Boîte de dialogue demandant au joueur ses options. */
	GtkWidget* dialogue;
	GtkWidget* hbox;
	GtkWidget* label;
	GtkWidget* nom;
	GtkWidget *radio1,*radio2,*radio3;
	GSList* nblettres;
	GSList* nbessais;
	GtkWidget* diabolique;
	GtkWidget* tempsReponse;
	GtkWidget* bingo;
	int i;
	
	dialogue=gtk_dialog_new_with_buttons("Choix des options",GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,
			GTK_STOCK_OK,GTK_RESPONSE_OK,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,NULL);
	VERIFIER_ALLOCATION(dialogue,"Impossible de créer la boîte de dialogue de saisie des options.\n",partie);
	
	/* Le pseudo */
	hbox=gtk_hbox_new(FALSE,10);
	VERIFIER_ALLOCATION(hbox,"Impossible de créer la boîte horizontale.\n",partie);
	label=gtk_label_new("Nom :");
	VERIFIER_ALLOCATION(label,"Impossible de créer le label \"Nom :\"\n",partie);
	gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,0);
	nom=gtk_entry_new();
	VERIFIER_ALLOCATION(nom,"Impossible de créer la zone de saisie du nom.\n",partie);
	gtk_box_pack_start(GTK_BOX(hbox),nom,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialogue)->vbox),hbox,FALSE,FALSE,0);
	
	/* Le nombre de lettres */
	hbox=gtk_hbox_new(FALSE,10);
	VERIFIER_ALLOCATION(hbox,"Impossible de créer la boîte horizontale.\n",partie);
	label=gtk_label_new("Nombre de lettres par mot :");
	VERIFIER_ALLOCATION(label,"Impossible de créer le label \"Nombre de lettres par mot :\"\n",partie);
	gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,0);
	radio1=gtk_radio_button_new_with_label(NULL,"6");
	VERIFIER_ALLOCATION(radio1,"Impossible de créer le bouton radio \"6\".\n",partie);
	gtk_box_pack_start(GTK_BOX(hbox),radio1,FALSE,FALSE,0);
	radio2=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio1),"7");
	VERIFIER_ALLOCATION(radio2,"Impossible de créer le bouton radio \"7\".\n",partie);
	gtk_box_pack_start(GTK_BOX(hbox),radio2,FALSE,FALSE,0);
	radio3=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio2),"8");
	VERIFIER_ALLOCATION(radio3,"Impossible de créer le bouton radio \"8\".\n",partie);
	gtk_box_pack_start(GTK_BOX(hbox),radio3,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialogue)->vbox),hbox,FALSE,FALSE,0);
	nblettres=gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio1));
	switch (partie->options.lettresParMot) {
		case 6:
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio1),TRUE);
			break;
		case 7:
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio2),TRUE);
			break;
		case 8:
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio3),TRUE);
			break;
		default:
			affichage_erreur("Le nombre de lettres dans les options est mauvais.\n");
	}
	
	/* Le nombre d'essais */
	hbox=gtk_hbox_new(FALSE,10);
	VERIFIER_ALLOCATION(hbox,"Impossible de créer la boîte horizontale.\n",partie);
	label=gtk_label_new("Nombre d'essais :");
	VERIFIER_ALLOCATION(label,"Impossible de créer le label \"Nombre d'essais :\"\n",partie);
	gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,0);
	radio1=gtk_radio_button_new_with_label(NULL,"5");
	VERIFIER_ALLOCATION(radio1,"Impossible de créer le bouton radio \"5\".\n",partie);
	gtk_box_pack_start(GTK_BOX(hbox),radio1,FALSE,FALSE,0);
	radio2=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio1),"6");
	VERIFIER_ALLOCATION(radio2,"Impossible de créer le bouton radio \"6\".\n",partie);
	gtk_box_pack_start(GTK_BOX(hbox),radio2,FALSE,FALSE,0);
	radio3=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio2),"7");
	VERIFIER_ALLOCATION(radio3,"Impossible de créer le bouton radio \"7\".\n",partie);
	gtk_box_pack_start(GTK_BOX(hbox),radio3,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialogue)->vbox),hbox,FALSE,FALSE,0);
	nbessais=gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio1));
	switch (partie->options.nbEssais) {
		case 5:
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio1),TRUE);
			break;
		case 6:
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio2),TRUE);
			break;
		case 7:
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio3),TRUE);
			break;
		default:
			affichage_erreur("Le nombre d'essais dans les options est mauvais.\n");
	}
	
	/* Temps de réponse */
	hbox=gtk_hbox_new(FALSE,10);
	VERIFIER_ALLOCATION(hbox,"Impossible de créer la boîte horizontale.\n",partie);
	label=gtk_label_new("Temps de réponse :");
	VERIFIER_ALLOCATION(label,"Impossible de créer le label \"Temps de réponse :\"\n",partie);
	gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,0);
	tempsReponse=gtk_spin_button_new_with_range(5,20,1);
	VERIFIER_ALLOCATION(tempsReponse,"Impossible de créer le « spin button ».\n",partie);
	gtk_box_pack_start(GTK_BOX(hbox),tempsReponse,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialogue)->vbox),hbox,FALSE,FALSE,0);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(tempsReponse),partie->options.tempsReponse);
	
	/* Diabolique */
	diabolique=gtk_check_button_new_with_label("Mode diabolique ?");
	VERIFIER_ALLOCATION(diabolique,"Impossible de créer la case à cocher \"Mode diabolique ?\".\n",partie);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialogue)->vbox),diabolique,FALSE,FALSE,0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(diabolique),(partie->options.modeDiabolique ? TRUE : FALSE));
	
	/* Bingo */
	bingo=gtk_check_button_new_with_label("Bingo ?");
	VERIFIER_ALLOCATION(bingo,"Impossible de créer la case à cocher \"Bingo ?\".\n",partie);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialogue)->vbox),bingo,FALSE,FALSE,0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bingo),(partie->options.bingo ? TRUE : FALSE));
	
	gtk_widget_show_all(GTK_DIALOG(dialogue)->vbox);
	if (gtk_dialog_run(GTK_DIALOG(dialogue))==GTK_RESPONSE_OK) {
		strcpy(partie->joueur1.nom,(char*)gtk_entry_get_text(GTK_ENTRY(nom)));
		for (i=8;nblettres;--i,nblettres=g_slist_next(nblettres)) {
			if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(nblettres->data))) {
				partie->options.lettresParMot=i;
				break;
			}
		}
		for (i=7;nbessais;--i,nbessais=g_slist_next(nbessais)) {
			if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(nbessais->data))) {
				partie->options.nbEssais=i;
				break;
			}
		}
		partie->options.tempsReponse=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(tempsReponse));
		partie->options.modeDiabolique=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(diabolique));
		partie->options.bingo=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(bingo));
		gtk_widget_destroy(dialogue);
		return 1;
	}
	gtk_widget_destroy(dialogue);
	return 0;
}

void affichage_indications (Partie* partie, int ligne) {
	int i;
	
	for (i=0;i<partie->options.lettresParMot;++i) {
		if (partie->motCourant.motTrouve[i]) {
			char str[2];
			
			str[0]=partie->motCourant.mot[i];
			str[1]='\0';
			gtk_label_set_label(GTK_LABEL(partie->widgets.caseslabels[ligne][i]),str);
		}
	}
}

void affichage_nouvelleSuperPartie (GtkWidget* appelant, gpointer param_partie)
{
	GtkWidget* dialogue;
	
	dialogue=gtk_message_dialog_new(GTK_WINDOW(((Partie*)(param_partie))->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,
			"Le mode super partie n'est pas encore disponible.");
	gtk_dialog_run(GTK_DIALOG(dialogue));
	gtk_widget_destroy(dialogue);
}

void affichage_reglesJeu (GtkWidget* appelant, gpointer param_partie)
{
	GtkWidget* dialogue;
	
	dialogue=gtk_message_dialog_new(GTK_WINDOW(((Partie*)(param_partie))->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,
			"Non disponible");
	gtk_dialog_run(GTK_DIALOG(dialogue));
	gtk_widget_destroy(dialogue);
}

void affichage_aPropos (GtkWidget* appelant, gpointer fenetre)
{
	GtkWidget* dialogue;
	
	dialogue=gtk_message_dialog_new(GTK_WINDOW(fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,
			"Jeu programmé par Guillaume Verdier et Thomas Fock-Chow-Tho.");
	gtk_dialog_run(GTK_DIALOG(dialogue));
	gtk_widget_destroy(dialogue);
}

void affichage_saisieMot (GtkWidget* entry, gpointer param_partie)
{
	Partie* partie=(Partie*)param_partie;
	int i,gagne;
	int ligne;
	GdkColor OK,mauvaisePosition,defaut;
	
	OK.red=COULEUR_OK_RED;
	OK.green=COULEUR_OK_GREEN;
	OK.blue=COULEUR_OK_BLUE;
	mauvaisePosition.red=COULEUR_MAUVAISE_POS_RED;
	mauvaisePosition.green=COULEUR_MAUVAISE_POS_GREEN;
	mauvaisePosition.blue=COULEUR_MAUVAISE_POS_BLUE;
	defaut.red=COULEUR_DEFAUT_RED;
	defaut.green=COULEUR_DEFAUT_GREEN;
	defaut.blue=COULEUR_DEFAUT_BLUE;
	
	--partie->motCourant.essaisRestants;
	ligne=partie->options.nbEssais - partie->motCourant.essaisRestants - 1;
	
	strcpy(partie->motCourant.motsSaisis[ligne],(char*)gtk_entry_get_text(GTK_ENTRY(entry)));
	gagne=jeu_corrigerMot(&(partie->motCourant),ligne,partie->options.lettresParMot);
	
	for (i=0;i<partie->options.lettresParMot;++i) {
		char str[2];
		GdkRectangle rect;
		
		str[0]=partie->motCourant.motsSaisis[ligne][i];
		str[1]='\0';
		gtk_label_set_label(GTK_LABEL(partie->widgets.caseslabels[ligne][i]),str);
		switch (partie->motCourant.corrections[ligne][i]) {
			case CORRECTION_BONNE_PLACE:
				gtk_widget_modify_bg(partie->widgets.casesevents[ligne][i],GTK_STATE_NORMAL,&OK);
				break;
			case CORRECTION_MAUVAISE_PLACE:
				gtk_widget_modify_bg(partie->widgets.casesevents[ligne][i],GTK_STATE_NORMAL,&mauvaisePosition);
				break;
		}
		g_usleep(G_USEC_PER_SEC / 8);
		rect.x=0;
		rect.y=0;
		gtk_window_get_default_size(GTK_WINDOW(partie->widgets.fenetre),&rect.width,&rect.height);
		/* Il faudrait trouver une autre méthode pour forcer le rafraichissement de la fenêtre : cette fonction est obsolète depuis GTK 1.2 */
		gtk_widget_draw(partie->widgets.table,&rect);
		g_usleep(G_USEC_PER_SEC / 8);
	}
	
	if (gagne) {
		GtkWidget* dialogue;
		
		dialogue=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,
				"Bravo, vous avez gagné !");
		gtk_dialog_run(GTK_DIALOG(dialogue));
		gtk_widget_destroy(dialogue);
		affichage_terminer(NULL,partie);
		return;
	}
	
	if (!partie->motCourant.essaisRestants) {
		/* Il n'y a plus d'essai disponible. */
		GtkWidget* dialogue;
		char str[45];
		
		sprintf (str,"Vous avez perdu.\nLe mot était : %s",partie->motCourant.mot);
		dialogue=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,str);
		gtk_dialog_run(GTK_DIALOG(dialogue));
		gtk_widget_destroy(dialogue);
		affichage_terminer(NULL,partie);
		return;
	}
	
	/* Sinon, on affiche les indications pour le mot suivant. */
	affichage_indications(partie, ligne+1);
	
	gtk_entry_set_text(GTK_ENTRY(partie->widgets.entree),"");
}

void affichage_terminer (GtkWidget* appelant, gpointer param_partie)
{
	Partie* partie=(Partie*)param_partie;
	int i;
	
	if (partie->widgets.table) {
		for (i=0;i<partie->options.nbEssais;++i) {
			free(partie->widgets.casesevents[i]);
			free(partie->widgets.caseslabels[i]);
		}
		free(partie->widgets.casesevents);
		free(partie->widgets.caseslabels);
	}
	
	jeu_terminer(partie);
	gtk_main_quit();
}
