#include "Affichage.h"

void lancer_motus (int* argc, char*** argv)
{
	Partie* partie;
	GtkWidget* image;
	
	partie=malloc(sizeof(Partie));
	if (!partie) {
		affichage_erreur("Impossible d'allouer la partie !\n");
		return;
	}
	
	gtk_init(argc,argv);
	srand(time(NULL));
	
	partie->widgets.splash=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	if (partie->widgets.splash==NULL) {
		affichage_erreur("Impossible d'ouvrir l'écran de démarrage.\n");
	} else {
		image=gtk_image_new_from_file("Motus.jpg");
		if (image==NULL) {
			affichage_erreur("Impossible de charger l'écran de démarrage.\n");
		} else {
			gtk_window_set_decorated(GTK_WINDOW(partie->widgets.splash),FALSE);
			gtk_window_set_position(GTK_WINDOW(partie->widgets.splash),GTK_WIN_POS_CENTER);
			gtk_container_add(GTK_CONTAINER(partie->widgets.splash),image);
			gtk_widget_show_all(partie->widgets.splash);
			g_timeout_add(1000,(GSourceFunc)affichage_splashDestroy,partie);
		}
	}
	gtk_main();
}

void affichage_initialiser (Partie* partie)
{
	GtkWidget* image; /* Pour charger les images de fond */

	/* Chargement des images de base */
	image=gtk_image_new_from_file ("FondDefaut.png");
	VERIFIER_ALLOCATION(image,"Impossible de charger l'image \"FondDefaut.png\".",partie)
	partie->widgets.imgDefaut=gtk_image_get_pixbuf(GTK_IMAGE(image));
	image=gtk_image_new_from_file ("FondOK.png");
	VERIFIER_ALLOCATION(image,"Impossible de charger l'image \"FondOK.png\".",partie)
	partie->widgets.imgOK=gtk_image_get_pixbuf(GTK_IMAGE(image));
	image=gtk_image_new_from_file ("FondMauvaisePos.png");
	VERIFIER_ALLOCATION(image,"Impossible de charger l'image \"FondMauvaisePos.png\".",partie)
	partie->widgets.imgMauvaisePos=gtk_image_get_pixbuf(GTK_IMAGE(image));
	
	/* Création des widgets */
	partie->widgets.fenetre=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	VERIFIER_ALLOCATION(partie->widgets.fenetre,"Impossible de créer la fenêtre.\n",partie)
	gtk_window_set_position(GTK_WINDOW(partie->widgets.fenetre),GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(partie->widgets.fenetre),200,150);
	partie->widgets.boxprincipale=gtk_vbox_new(FALSE,0);
	VERIFIER_ALLOCATION(partie->widgets.boxprincipale,"Impossible de créer la box principale.\n",partie)
	
	affichage_creerMenu(partie);
	
	partie->widgets.joueur=gtk_label_new("");
	VERIFIER_ALLOCATION(partie->widgets.joueur,"Impossible de créer le label d'affichage du nom du joueur.\n",partie)
	partie->widgets.scores=gtk_label_new("");
	VERIFIER_ALLOCATION(partie->widgets.scores,"Impossible de créer le label d'affichage des scores.\n",partie)
	partie->widgets.affichageTimer=gtk_progress_bar_new();
	VERIFIER_ALLOCATION(partie->widgets.affichageTimer,"Impossible de créer la barre de progression d'affichage du timer.\n",partie)
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(partie->widgets.affichageTimer),"Temps restant");
	gtk_progress_bar_set_orientation(GTK_PROGRESS_BAR(partie->widgets.affichageTimer),GTK_PROGRESS_RIGHT_TO_LEFT);
	
	partie->widgets.timer=g_timer_new();
	VERIFIER_ALLOCATION(partie->widgets.timer,"Impossible de créer le timer.\n",partie)
	
	/* La grille de motus et les cases ne seront crées qu'au début de la partie (pas au début du jeu). */
	partie->widgets.layout=NULL; /* Sert à indiquer qu'aucune partie n'est commencée. */
	
	/* Connexion des signaux aux fonctions de rappel */
	g_signal_connect(G_OBJECT(partie->widgets.fenetre),"destroy",G_CALLBACK(affichage_terminer),partie);
	
	gtk_box_pack_start(GTK_BOX(partie->widgets.boxprincipale),partie->widgets.joueur,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(partie->widgets.boxprincipale),partie->widgets.scores,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(partie->widgets.boxprincipale),partie->widgets.affichageTimer,FALSE,FALSE,0);
	gtk_container_add(GTK_CONTAINER(partie->widgets.fenetre),partie->widgets.boxprincipale);
	/* Il ne faut pas tout afficher (la barre de progression pour le timer, notamment, doit rester cachée). */
	gtk_widget_show(partie->widgets.boxprincipale);
	gtk_widget_show(partie->widgets.fenetre);
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
	
	eltmenu=gtk_menu_item_new_with_label("Arrêter partie");
	VERIFIER_ALLOCATION(eltmenu,"Impossible de créer l'élément de menu \"Arrêter partie\".\n",partie)
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),eltmenu);
	g_signal_connect(G_OBJECT(eltmenu),"activate",G_CALLBACK(affichage_menuTerminerPartie),partie);
	
	eltmenu=gtk_menu_item_new_with_label("Sauvegarder");
	VERIFIER_ALLOCATION(eltmenu,"Impossible de créer l'élément de menu \"Sauvegarder\".\n",partie)
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),eltmenu);
	g_signal_connect(G_OBJECT(eltmenu),"activate",G_CALLBACK(affichage_sauvegarder),partie);
	
	eltmenu=gtk_menu_item_new_with_label("Charger");
	VERIFIER_ALLOCATION(eltmenu,"Impossible de créer l'élément de menu \"Charger\".\n",partie)
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),eltmenu);
	g_signal_connect(G_OBJECT(eltmenu),"activate",G_CALLBACK(affichage_charger),partie);
	
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
	
	eltmenu=gtk_menu_item_new_with_label("Meilleurs scores");
	VERIFIER_ALLOCATION(eltmenu,"Impossible de créer l'élément de menu \"Meilleurs scores\".\n",partie)
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),eltmenu);
	g_signal_connect(G_OBJECT(eltmenu),"activate",G_CALLBACK(affichage_meilleursScores),partie->widgets.fenetre);
	
	eltmenu=gtk_menu_item_new_with_label("Règles du jeu");
	VERIFIER_ALLOCATION(eltmenu,"Impossible de créer l'élément de menu \"Règles du jeu\".\n",partie)
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),eltmenu);
	g_signal_connect(G_OBJECT(eltmenu),"activate",G_CALLBACK(affichage_reglesJeu),partie);
	
	eltmenu=gtk_menu_item_new_with_label("À propos");
	VERIFIER_ALLOCATION(eltmenu,"Impossible de créer l'élément de menu \"À propos\".\n",partie)
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),eltmenu);
	g_signal_connect(G_OBJECT(eltmenu),"activate",G_CALLBACK(affichage_aPropos),partie->widgets.fenetre);
	
	eltmenu=gtk_menu_item_new_with_label("Aide");
	VERIFIER_ALLOCATION(eltmenu,"Impossible de créer le menu \"Aider\".\n",partie);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(eltmenu),menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(barremenu),eltmenu);
	
	gtk_box_pack_start(GTK_BOX(partie->widgets.boxprincipale),barremenu,FALSE,FALSE,0);
	gtk_widget_show_all(barremenu);
}

void affichage_passagePartieSuperPartie (Partie* partie)
{
	GtkWidget* dialogue;
	GtkWidget* hbox;
	GtkWidget* label;
	GtkWidget* tempsReponse;
	
	dialogue=gtk_dialog_new_with_buttons("Passage à la super-partie",GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,
			GTK_STOCK_OK,GTK_RESPONSE_OK,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,NULL);
	VERIFIER_ALLOCATION(dialogue,"Impossible de créer la boîte de dialogue de passage à la super-partie.\n",partie);
	
	label=gtk_label_new("Les options que vous avez choisi pour la partie seront conservées.\n"
			"Le temps de réponse n'est cependant pas le même dans la partie et la super-partie, veuillez le régler.");
	VERIFIER_ALLOCATION(label,"Impossible de créer le label explicatif.\n",partie);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialogue)->vbox),label,FALSE,FALSE,0);
	
	/* Temps de réponse */
	hbox=gtk_hbox_new(FALSE,10);
	VERIFIER_ALLOCATION(hbox,"Impossible de créer la boîte horizontale.\n",partie);
	label=gtk_label_new("Temps de réponse :");
	VERIFIER_ALLOCATION(label,"Impossible de créer le label \"Temps de réponse :\"\n",partie);
	gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,0);
	tempsReponse=gtk_spin_button_new_with_range(5,10,1);
	VERIFIER_ALLOCATION(tempsReponse,"Impossible de créer le « spin button ».\n",partie);
	gtk_widget_set_tooltip_text(tempsReponse,"Le nombre de minutes dont vous disposez pour trouver 10 mots");
	gtk_box_pack_start(GTK_BOX(hbox),tempsReponse,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialogue)->vbox),hbox,FALSE,FALSE,0);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(tempsReponse),partie->options.tempsReponse);

	gtk_widget_show_all(GTK_DIALOG(dialogue)->vbox);
	gtk_dialog_run(GTK_DIALOG(dialogue));
	partie->options.tempsReponse=60*gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(tempsReponse));
	gtk_widget_destroy(dialogue);
	
	partie->tempsRestant=partie->options.tempsReponse;
	
	partie->superPartie=1;
	if (partie->options.nbJoueurs==2)
		partie->joueurCourant=partie->joueur1.score>partie->joueur2.score;
}

void affichage_miseAJourScoreNom (const Partie* partie)
{
	char scoresLabel[15];
	
	gtk_label_set_label(GTK_LABEL(partie->widgets.joueur),(gchar*)(partie->joueurCourant?partie->joueur1.nom:partie->joueur2.nom));
	if (partie->superPartie)
		sprintf(scoresLabel,"Score : %d/10",(partie->joueurCourant?partie->joueur1.score:partie->joueur2.score));
	else
		sprintf(scoresLabel,"Score : %d",(partie->joueurCourant?partie->joueur1.score:partie->joueur2.score));
	gtk_label_set_label(GTK_LABEL(partie->widgets.scores),scoresLabel);
}

void affichage_nouvellePartie (GtkWidget* appelant, gpointer param_partie)
{
	Partie* partie=(Partie*)param_partie;
	int i,j;
	
	if (partie->widgets.layout) { /* Il y a déjà une partie en cours */
		GtkWidget* dialogue;
		
		dialogue=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,
				"Commencez par terminer la partie en cours !");
		gtk_dialog_run(GTK_DIALOG(dialogue));
		gtk_widget_destroy(dialogue);
		return;
	}
		
	partie->superPartie=0;
	if (!affichage_saisieOptions (partie))
		return;
	
	/* Allocations dynamiques des tableaux nécessaires au jeu. */
	if (jeu_initialiserNouvellePartie(partie))
		affichage_erreur("Erreur à l'allocation dynamique des champs de la structure Mot.\n");
	partie->widgets.casesimages=malloc(partie->options.nbEssais*sizeof(GtkWidget**));
	VERIFIER_ALLOCATION(partie->widgets.casesimages,"Impossible d'allouer le tableau de images.\n",partie);
	partie->widgets.caseslabels=malloc(partie->options.nbEssais*sizeof(GtkWidget**));
	VERIFIER_ALLOCATION(partie->widgets.caseslabels,"Impossible d'allouer le tableau de GtkEventBox.\n",partie);
	for(i=0;i<partie->options.nbEssais;++i) {
		partie->widgets.casesimages[i]=malloc(partie->options.lettresParMot*sizeof(GtkWidget*));
		VERIFIER_ALLOCATION(partie->widgets.casesimages[i],"Impossible d'allouer le tableau d'images.\n",partie);
		partie->widgets.caseslabels[i]=malloc(partie->options.lettresParMot*sizeof(GtkWidget*));
		VERIFIER_ALLOCATION(partie->widgets.caseslabels[i],"Impossible d'allouer le tableau de labels.\n",partie);
	}
	
	affichage_nouveauMot(partie);
	
	affichage_miseAJourScoreNom(partie);
	partie->widgets.layout=gtk_layout_new(NULL,NULL);
	VERIFIER_ALLOCATION(partie->widgets.layout,"Impossible d'allouer le layout.\n",partie);
	for (i=0;i<partie->options.nbEssais;++i) {
		for(j=0;j<partie->options.lettresParMot;++j) {
			partie->widgets.casesimages[i][j]=gtk_image_new_from_pixbuf(partie->widgets.imgDefaut);
			VERIFIER_ALLOCATION(partie->widgets.casesimages[i][j],"Impossible de créer une image.\n",partie);
			partie->widgets.caseslabels[i][j]=gtk_label_new(" ");
			VERIFIER_ALLOCATION(partie->widgets.caseslabels[i][j],"Impossible de créer un label.\n",partie);
			
			gtk_layout_put(GTK_LAYOUT(partie->widgets.layout),partie->widgets.casesimages[i][j],TAILLE_IMAGE*j,TAILLE_IMAGE*i);
			gtk_layout_put(GTK_LAYOUT(partie->widgets.layout),partie->widgets.caseslabels[i][j],10+TAILLE_IMAGE*j,3+TAILLE_IMAGE*i);
		}
	}
	gtk_box_pack_start(GTK_BOX(partie->widgets.boxprincipale),partie->widgets.layout,TRUE,TRUE,0);
	
	gtk_window_resize(GTK_WINDOW(partie->widgets.fenetre),partie->options.lettresParMot*32,150+partie->options.nbEssais*32);
	
	partie->widgets.entree=gtk_entry_new();
	VERIFIER_ALLOCATION(partie->widgets.entree,"Impossible de créer la zone de saisie.\n",partie)
	gtk_widget_set_tooltip_text(partie->widgets.entree,"Saisissez votre mot ici.");
	g_signal_connect(G_OBJECT(partie->widgets.entree),"activate",G_CALLBACK(affichage_saisieMot),partie);
	gtk_box_pack_start(GTK_BOX(partie->widgets.boxprincipale),partie->widgets.entree,FALSE,FALSE,0);
	
	partie->widgets.suivant=gtk_button_new_with_label("Mot suivant");
	VERIFIER_ALLOCATION(partie->widgets.entree,"Impossible de créer le bouton \"Mot suivant\".\n",partie)
	g_signal_connect(G_OBJECT(partie->widgets.suivant),"clicked",G_CALLBACK(affichage_motSuivant),partie);
	gtk_box_pack_start(GTK_BOX(partie->widgets.boxprincipale),partie->widgets.suivant,FALSE,FALSE,0);
	
	gtk_widget_show_all(partie->widgets.fenetre);
	gtk_widget_hide(partie->widgets.suivant);
	
	affichage_saisieMot (NULL, partie); /* Sert à lancer la parte (initialise ce qui doit l'être) */
}

void affichage_nouveauMot (Partie* partie)
{
	if (jeu_tirerMot(partie->motCourant.mot,partie->options.lettresParMot,partie->options.modeDiabolique))
		affichage_terminer(NULL,partie);
	
	jeu_reinitialiserMot (&partie->motCourant,partie->options.nbEssais,partie->options.lettresParMot,partie->superPartie);
}

int affichage_saisieOptions (Partie* partie)
{
	/* ATTENTION : pour l'instant, seul le mode UN JOUEUR est disponible. */
	/* Boîte de dialogue demandant au joueur ses options. */
	GtkWidget* dialogue;
	GtkWidget* hbox;
	GtkWidget* label;
	GtkWidget *nom1,*nom2;
	GtkWidget *radio1,*radio2,*radio3;
	GSList* nbjoueurs;
	GSList* nblettres;
	GSList* nbessais;
	GtkWidget* diabolique;
	GtkWidget* tempsReponse;
	GtkWidget* bingo;
	int i;
	
	jeu_initialiser(partie);
	
	dialogue=gtk_dialog_new_with_buttons("Choix des options",GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,
			GTK_STOCK_OK,GTK_RESPONSE_OK,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,NULL);
	VERIFIER_ALLOCATION(dialogue,"Impossible de créer la boîte de dialogue de saisie des options.\n",partie);
	
	/* Les pseudos */
	hbox=gtk_hbox_new(FALSE,10);
	VERIFIER_ALLOCATION(hbox,"Impossible de créer la boîte horizontale.\n",partie);
	if (!partie->superPartie)
		label=gtk_label_new("Nom du joueur 1 :");
	else
		label=gtk_label_new("Nom du joueur :");
	VERIFIER_ALLOCATION(label,"Impossible de créer le label \"Nom du joueur 1 :\"\n",partie);
	gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,0);
	nom1=gtk_entry_new();
	VERIFIER_ALLOCATION(nom1,"Impossible de créer la zone de saisie du nom du joueur 1.\n",partie);
	gtk_box_pack_start(GTK_BOX(hbox),nom1,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialogue)->vbox),hbox,FALSE,FALSE,0);
	
	if (!partie->superPartie) {
		hbox=gtk_hbox_new(FALSE,10);
		VERIFIER_ALLOCATION(hbox,"Impossible de créer la boîte horizontale.\n",partie);
		label=gtk_label_new("Nom du joueur 2 :");
		VERIFIER_ALLOCATION(label,"Impossible de créer le label \"Nom du joueur 2 :\"\n",partie);
		gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,0);
		nom2=gtk_entry_new();
		VERIFIER_ALLOCATION(nom2,"Impossible de créer la zone de saisie du nom du joueur 2.\n",partie);
		gtk_box_pack_start(GTK_BOX(hbox),nom2,FALSE,FALSE,0);
		gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialogue)->vbox),hbox,FALSE,FALSE,0);
	}
	
	/* Le nombre de joueurs */
	if (!partie->superPartie) {
		hbox=gtk_hbox_new(FALSE,10);
		VERIFIER_ALLOCATION(hbox,"Impossible de créer la boîte horizontale.\n",partie);
		label=gtk_label_new("Nombre de joueurs :");
		VERIFIER_ALLOCATION(label,"Impossible de créer le label \"Nombre de joueurs :\"\n",partie);
		gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,0);
		radio1=gtk_radio_button_new_with_label(NULL,"1");
		VERIFIER_ALLOCATION(radio1,"Impossible de créer le bouton radio \"1\".\n",partie);
		gtk_box_pack_start(GTK_BOX(hbox),radio1,FALSE,FALSE,0);
		radio2=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio1),"2");
		VERIFIER_ALLOCATION(radio2,"Impossible de créer le bouton radio \"2\".\n",partie);
		gtk_box_pack_start(GTK_BOX(hbox),radio2,FALSE,FALSE,0);
		gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialogue)->vbox),hbox,FALSE,FALSE,0);
		nbjoueurs=gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio1));
	}
	
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
	if (partie->superPartie) {
		tempsReponse=gtk_spin_button_new_with_range(5,10,1);
		VERIFIER_ALLOCATION(tempsReponse,"Impossible de créer le « spin button ».\n",partie);
		gtk_widget_set_tooltip_text(tempsReponse,"Le nombre de minutes dont vous disposez pour trouver 10 mots");
	} else {
		tempsReponse=gtk_spin_button_new_with_range(5,20,1);
		VERIFIER_ALLOCATION(tempsReponse,"Impossible de créer le « spin button ».\n",partie);
		gtk_widget_set_tooltip_text(tempsReponse,"Le nombre de secondes dont vous disposez pour répondre");
	}
	gtk_box_pack_start(GTK_BOX(hbox),tempsReponse,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialogue)->vbox),hbox,FALSE,FALSE,0);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(tempsReponse),partie->options.tempsReponse);
	
	/* Diabolique */
	diabolique=gtk_check_button_new_with_label("Mode diabolique ?");
	VERIFIER_ALLOCATION(diabolique,"Impossible de créer la case à cocher \"Mode diabolique ?\".\n",partie);
	gtk_widget_set_tooltip_text(diabolique,"Dans le mode diabolique, les mots seront plus difficiles à trouver (mots rares, à l'orthographe particulière, ...)");
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialogue)->vbox),diabolique,FALSE,FALSE,0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(diabolique),(partie->options.modeDiabolique ? TRUE : FALSE));
	
	/* Bingo */
	if (!partie->superPartie) {
		bingo=gtk_check_button_new_with_label("Bingo ?");
		VERIFIER_ALLOCATION(bingo,"Impossible de créer la case à cocher \"Bingo ?\".\n",partie);
		gtk_widget_set_tooltip_text(bingo,"Indique si vous souhaitez que les bingos soient présents après avoir trouvé un mot.");
		gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialogue)->vbox),bingo,FALSE,FALSE,0);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bingo),(partie->options.bingo ? TRUE : FALSE));
	}
	
	gtk_widget_show_all(GTK_DIALOG(dialogue)->vbox);
	if (gtk_dialog_run(GTK_DIALOG(dialogue))==GTK_RESPONSE_OK) {
		strcpy(partie->joueur1.nom,(char*)gtk_entry_get_text(GTK_ENTRY(nom1)));
		if (!partie->superPartie)
			strcpy(partie->joueur2.nom,(char*)gtk_entry_get_text(GTK_ENTRY(nom2)));
		if (!partie->superPartie) {
			for (i=2;nbjoueurs;--i,nbjoueurs=g_slist_next(nbjoueurs)) {
				if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(nbjoueurs->data))) {
					partie->options.nbJoueurs=i;
					break;
				}
			}
		}
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
		if (partie->superPartie)
			partie->options.tempsReponse=60*gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(tempsReponse));
		else
			partie->options.tempsReponse=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(tempsReponse));
		partie->options.modeDiabolique=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(diabolique));
		if (!partie->superPartie)
			partie->options.bingo=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(bingo));
		gtk_widget_destroy(dialogue);
		return 1;
	}
	gtk_widget_destroy(dialogue);
	return 0;
}

void affichage_indications (Partie* partie, int ligne)
{
	int i;
	
	for (i=0;i<partie->options.lettresParMot;++i) {
		if (partie->motCourant.motTrouve[i]) {
			char str[40];
			
			sprintf(str,"<span size=\"x-large\">%c</span>",partie->motCourant.mot[i]);
			gtk_label_set_markup(GTK_LABEL(partie->widgets.caseslabels[ligne][i]),str);
		}
	}
}

void affichage_terminerPartie (Partie* partie)
{
	int i,j;
	
	jeu_terminer(partie);
	
	for (i=0;i<partie->options.nbEssais;++i) {
		for(j=0;j<partie->options.lettresParMot;++j) {
			gtk_widget_destroy(partie->widgets.casesimages[i][j]);
			gtk_widget_destroy(partie->widgets.caseslabels[i][j]);
		}
		free(partie->widgets.casesimages[i]);
		free(partie->widgets.caseslabels[i]);
	}
	free(partie->widgets.casesimages);
	free(partie->widgets.caseslabels);
	gtk_widget_destroy(partie->widgets.layout);
	partie->widgets.layout=NULL; /* On s'en sert pour savoir si une partie est commencée ou non. */
	gtk_widget_hide(partie->widgets.joueur);
	gtk_widget_hide(partie->widgets.scores);
	gtk_widget_hide(partie->widgets.affichageTimer);
	gtk_widget_destroy(partie->widgets.entree);
	gtk_widget_destroy(partie->widgets.suivant);
}

gboolean affichage_splashDestroy (gpointer param_partie)
{
	Partie* partie=(Partie*)param_partie;
	
	gtk_widget_destroy(partie->widgets.splash);
	affichage_initialiser(partie);
	
	return FALSE;
}

void affichage_nouvelleSuperPartie (GtkWidget* appelant, gpointer param_partie)
{
	Partie* partie=(Partie*)param_partie;
	int i,j;
	
	if (partie->widgets.layout) { /* Il y a déjà une partie en cours */
		GtkWidget* dialogue;
		
		dialogue=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,
				"Commencez par terminer la partie en cours !");
		gtk_dialog_run(GTK_DIALOG(dialogue));
		gtk_widget_destroy(dialogue);
		return;
	}
	
	partie->superPartie=1;
	partie->options.nbJoueurs=1;
	
	if (!affichage_saisieOptions (partie))
		return;
	
	/* Allocations dynamiques des tableaux nécessaires au jeu. */
	if (jeu_initialiserNouvellePartie(partie))
		affichage_erreur("Erreur à l'allocation dynamique des champs de la structure Mot.\n");
	partie->joueur1.score=0;
	
	partie->widgets.casesimages=malloc(partie->options.nbEssais*sizeof(GtkWidget**));
	VERIFIER_ALLOCATION(partie->widgets.casesimages,"Impossible d'allouer le tableau de images.\n",partie);
	partie->widgets.caseslabels=malloc(partie->options.nbEssais*sizeof(GtkWidget**));
	VERIFIER_ALLOCATION(partie->widgets.caseslabels,"Impossible d'allouer le tableau de GtkEventBox.\n",partie);
	for(i=0;i<partie->options.nbEssais;++i) {
		partie->widgets.casesimages[i]=malloc(partie->options.lettresParMot*sizeof(GtkWidget*));
		VERIFIER_ALLOCATION(partie->widgets.casesimages[i],"Impossible d'allouer le tableau d'images.\n",partie);
		partie->widgets.caseslabels[i]=malloc(partie->options.lettresParMot*sizeof(GtkWidget*));
		VERIFIER_ALLOCATION(partie->widgets.caseslabels[i],"Impossible d'allouer le tableau de labels.\n",partie);
	}
	
	affichage_nouveauMot(partie);
	
	affichage_miseAJourScoreNom(partie);
	partie->widgets.layout=gtk_layout_new(NULL,NULL);
	VERIFIER_ALLOCATION(partie->widgets.layout,"Impossible d'allouer le layout.\n",partie);
	for (i=0;i<partie->options.nbEssais;++i) {
		for(j=0;j<partie->options.lettresParMot;++j) {
			partie->widgets.casesimages[i][j]=gtk_image_new_from_pixbuf(partie->widgets.imgDefaut);
			VERIFIER_ALLOCATION(partie->widgets.casesimages[i][j],"Impossible de créer une image.\n",partie);
			partie->widgets.caseslabels[i][j]=gtk_label_new(" ");
			VERIFIER_ALLOCATION(partie->widgets.caseslabels[i][j],"Impossible de créer un label.\n",partie);
			
			gtk_layout_put(GTK_LAYOUT(partie->widgets.layout),partie->widgets.casesimages[i][j],TAILLE_IMAGE*j,TAILLE_IMAGE*i);
			gtk_layout_put(GTK_LAYOUT(partie->widgets.layout),partie->widgets.caseslabels[i][j],10+TAILLE_IMAGE*j,3+TAILLE_IMAGE*i);
		}
	}
	gtk_box_pack_start(GTK_BOX(partie->widgets.boxprincipale),partie->widgets.layout,TRUE,TRUE,0);
	
	gtk_window_resize(GTK_WINDOW(partie->widgets.fenetre),partie->options.lettresParMot*32,150+partie->options.nbEssais*32);
	
	partie->widgets.entree=gtk_entry_new();
	VERIFIER_ALLOCATION(partie->widgets.entree,"Impossible de créer la zone de saisie.\n",partie)
	gtk_widget_set_tooltip_text(partie->widgets.entree,"Saisissez votre mot ici.");
	g_signal_connect(G_OBJECT(partie->widgets.entree),"activate",G_CALLBACK(affichage_saisieMot),partie);
	gtk_box_pack_start(GTK_BOX(partie->widgets.boxprincipale),partie->widgets.entree,FALSE,FALSE,0);
	
	partie->widgets.suivant=gtk_button_new_with_label("Mot suivant");
	VERIFIER_ALLOCATION(partie->widgets.entree,"Impossible de créer le bouton \"Mot suivant\".\n",partie)
	g_signal_connect(G_OBJECT(partie->widgets.suivant),"clicked",G_CALLBACK(affichage_motSuivant),partie);
	gtk_box_pack_start(GTK_BOX(partie->widgets.boxprincipale),partie->widgets.suivant,FALSE,FALSE,0);
	
	gtk_widget_show_all(partie->widgets.fenetre);
	gtk_widget_hide(partie->widgets.suivant);
	
	/*g_timer_start(partie->widgets.timer);*/ /* Lance le timer (qui ne sera plus modifié jusqu'à la fin de la super-partie) */
	partie->tempsRestant=partie->options.tempsReponse;
	affichage_saisieMot (NULL, partie); /* Sert à lancer la partie (initialise ce qui doit l'être) */
}

void affichage_menuTerminerPartie (GtkWidget* appelant, gpointer param_partie)
{
	Partie* partie=(Partie*)param_partie;
	GtkWidget* dialogue;
	
	if (!partie->widgets.layout) {
		dialogue=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,
				"Vous ne pouvez pas arrêter la partie : vous n'en avez pas commencé !");
		gtk_dialog_run(GTK_DIALOG(dialogue));
		gtk_widget_destroy(dialogue);
	} else {
		dialogue=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,
				"Voulez-vous vraiment arrêter la partie en cours ?");
		if (gtk_dialog_run(GTK_DIALOG(dialogue))==GTK_RESPONSE_YES) {
			affichage_terminerPartie (partie);
			affichage_saisieMot (NULL,partie); /* Pour arrêter les timers */
		}
		gtk_widget_destroy(dialogue);
	}
}

void affichage_meilleursScores (GtkWidget* appelant, gpointer fenetre)
{
	GtkWidget* dialogue;
	GtkWidget *labelpartie;
	char* texte;
	char noms[TAILLE_HISTORIQUE][TAILLE_PSEUDO];
	int scores[TAILLE_HISTORIQUE];
	int nbenregistres,i;
	
	dialogue=gtk_dialog_new_with_buttons("Meilleurs scores",GTK_WINDOW(fenetre),GTK_DIALOG_MODAL,GTK_STOCK_OK,GTK_RESPONSE_OK,NULL);
	/* Il est inutile de terminer le jeu si ceci échoue. */
	BINGO_VERIFIER_ALLOCATION(dialogue,"Impossible de créer la boîte de dialogue des meilleurs scores.\n",return;);
	
	nbenregistres=jeu_historique_donnees(noms,scores);
	texte=malloc(nbenregistres*(TAILLE_PSEUDO+3)+200); /*Pour avoir assez de place pour tout écrire */
	if (!texte)
		return;
	sprintf(texte,"\t<big>Meilleurs scores :</big>\t\n");
	for (i=0;i<nbenregistres;++i) {
		char temp[TAILLE_PSEUDO+40];
		if (!i)
			sprintf(temp,"1<sup>er</sup> : %s  %d points\n",noms[i],scores[i]);
		else if(i==1&&nbenregistres==2)
			sprintf(temp,"2<sup>nd</sup> : %s %d points\n",noms[i],scores[i]);
		else
			sprintf(temp,"%d<sup>ème</sup> : %s %d points\n",i+1,noms[i],scores[i]);
		texte=strcat(texte,temp);
	}
	labelpartie=gtk_label_new("");
	gtk_label_set_markup(GTK_LABEL(labelpartie),texte);
	free(texte);
	
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialogue)->vbox),labelpartie,FALSE,FALSE,10);
	gtk_widget_show_all(GTK_DIALOG(dialogue)->vbox);
	gtk_dialog_run(GTK_DIALOG(dialogue));
	gtk_widget_destroy(dialogue);
}

void affichage_reglesJeu (GtkWidget* appelant, gpointer param_partie)
{
	GtkWidget* dialogue;
	
	dialogue=gtk_message_dialog_new(GTK_WINDOW(((Partie*)(param_partie))->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,
			"Non disponible");
	VERIFIER_ALLOCATION(dialogue,"Impossible de créer la boîte de message.\n",(Partie*)param_partie)
	gtk_dialog_run(GTK_DIALOG(dialogue));
	gtk_widget_destroy(dialogue);
}

void affichage_aPropos (GtkWidget* appelant, gpointer fenetre)
{
	GtkWidget* dialogue;
	
	dialogue=gtk_message_dialog_new(GTK_WINDOW(fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,
			"Jeu programmé par Guillaume Verdier et Thomas Fock-Chow-Tho.\nCe programme utilise la bibliothèque GTK+ version 2.");
	gtk_dialog_run(GTK_DIALOG(dialogue));
	gtk_widget_destroy(dialogue);
}

void affichage_saisieMot (GtkWidget* entry, gpointer param_partie)
{
	Partie* partie=(Partie*)param_partie;
	static int gagne=0;
	int ligne;
	static int timeoutnum=-1;
	
	if (!partie->widgets.layout||GTK_WIDGET_VISIBLE(partie->widgets.suivant)) {
		if(timeoutnum!=-1) {
			g_source_remove(timeoutnum);
			timeoutnum=-1;
		}
		return;
	}
	if (entry!=NULL) {
		char* mot;
		if(timeoutnum!=-1) {
			g_source_remove(timeoutnum);
			timeoutnum=-1;
		}
		--partie->motCourant.essaisRestants;
		ligne=partie->options.nbEssais - partie->motCourant.essaisRestants - 1;
		
		/*strcpy(partie->motCourant.motsSaisis[ligne],(char*)gtk_entry_get_text(GTK_ENTRY(entry)));*/
		mot=(char*)gtk_entry_get_text(GTK_ENTRY(entry));
		/* Si la saisie n'est pas valide */
		if (mot[0]!=partie->motCourant.mot[0]||
				(int)strlen(mot)!=partie->options.lettresParMot||
				!jeu_motPresent(mot,partie->options.lettresParMot,0)||
				(partie->options.modeDiabolique&&!jeu_motPresent(mot,partie->options.lettresParMot,1))) {
			partie->motCourant.motsSaisis[ligne][0]='\0';
			affichage_saisieMot(NULL,partie); /* On passe au mot suivant */
			if (!partie->superPartie&&partie->options.nbJoueurs==2) {
				partie->joueurCourant=!partie->joueurCourant;
				jeu_ajouterLettre(&partie->motCourant);
				affichage_miseAJourScoreNom(partie);
			}
			return;
		}
		strcpy(partie->motCourant.motsSaisis[ligne],mot);
		
		gagne=jeu_corrigerMot(&(partie->motCourant),ligne,partie->options.lettresParMot);
		
		g_timeout_add(500,(GSourceFunc)affichage_tableLettres,partie);
	} else {
		if (!partie->superPartie)
			partie->tempsRestant=partie->options.tempsReponse;
		if (gagne) {
			gagne=0;
			if (partie->superPartie) {
				GtkWidget* dialogue;
				
				dialogue=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,
						"Bravo, vous avez trouvé un mot supplémentaire !");
				gtk_dialog_run(GTK_DIALOG(dialogue));
				gtk_widget_destroy(dialogue);
				if (partie->joueurCourant)
					++partie->joueur1.score;
				else
					++partie->joueur2.score;
			} else {
				GtkWidget* dialogue;
				
				dialogue=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,
						"Bravo, vous avez gagné 50 points !");
				gtk_dialog_run(GTK_DIALOG(dialogue));
				gtk_widget_destroy(dialogue);
				if (partie->joueurCourant)
					partie->joueur1.score+=50;
				else
					partie->joueur2.score+=50;
				if (partie->options.bingo)
					affichage_bingo_lancer(partie->joueurCourant?&partie->joueur1:&partie->joueur2,GTK_WINDOW(partie->widgets.fenetre));
			}
			gtk_widget_show(partie->widgets.suivant);
			return;
		}
		
		if (!partie->motCourant.essaisRestants) {
			/* Il n'y a plus d'essai disponible. */
			GtkWidget* dialogue;
			
			dialogue=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,
					"Vous avez perdu.\nLe mot était : %s.",partie->motCourant.mot);
			gtk_dialog_run(GTK_DIALOG(dialogue));
			gtk_widget_destroy(dialogue);
			if (!partie->superPartie&&partie->options.bingo)
				affichage_bingo_lancer(partie->joueurCourant?&partie->joueur1:&partie->joueur2,GTK_WINDOW(partie->widgets.fenetre));
			gtk_widget_show(partie->widgets.suivant);
			return;
		}
		
		/* Sinon, on affiche les indications pour le mot suivant et on relance les timers. */
		ligne=partie->options.nbEssais - partie->motCourant.essaisRestants - 1;
		affichage_indications(partie, ligne+1);
		g_timer_start(partie->widgets.timer);
		timeoutnum=g_timeout_add(50,(GSourceFunc)affichage_rafraichissementTimer,partie);
		
		gtk_entry_set_text(GTK_ENTRY(partie->widgets.entree),"");
	}
}

void affichage_motSuivant (GtkWidget* appelant, gpointer param_partie)
{
	Partie* partie=(Partie*)param_partie;
	static int nbMots=0;
	
	if (!partie->superPartie&&++nbMots==10) {
		GtkWidget* dialogue;
		if (partie->options.nbJoueurs==2)
			dialogue=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,
					"Vous avez terminé la partie.\nVos scores sont :\n\t\t%s %d\n\t\t%s %d\n",
					partie->joueur1.nom,partie->joueur1.score,partie->joueur2.nom,partie->joueur2.score);
		else
			dialogue=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,
					"Vous avez terminé la partie, votre score est de %d.\n",partie->joueur1.score);
		gtk_dialog_run(GTK_DIALOG(dialogue));
		gtk_widget_destroy(dialogue);
		if (partie->options.nbJoueurs==1) {
			if (jeu_historique_sauver (partie->joueur1.nom, partie->joueur1.score)==1) {
				dialogue=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,
						"Bravo %s, vous faîtes partie des meilleurs joueurs.",partie->joueur1.nom);
				gtk_dialog_run(GTK_DIALOG(dialogue));
				gtk_widget_destroy(dialogue);
			}
		} else {
			if (partie->joueur1.score>partie->joueur2.score) {
				if (jeu_historique_sauver (partie->joueur1.nom, partie->joueur1.score)==1) {
					dialogue=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,
							"Bravo %s, vous faîtes partie des meilleurs joueurs.",partie->joueur1.nom);
					gtk_dialog_run(GTK_DIALOG(dialogue));
					gtk_widget_destroy(dialogue);
				}
			} else {
				if (jeu_historique_sauver (partie->joueur2.nom, partie->joueur2.score)==1) {
					dialogue=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,
							"Bravo %s, vous faîtes partie des meilleurs joueurs.",partie->joueur2.nom);
					gtk_dialog_run(GTK_DIALOG(dialogue));
					gtk_widget_destroy(dialogue);
				}
			}
		}
		dialogue=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_YES_NO,
					"Voulez-vous continuer et jouer la super-partie ?");
		if (gtk_dialog_run(GTK_DIALOG(dialogue))==GTK_RESPONSE_YES) {
			affichage_passagePartieSuperPartie(partie);
			gtk_widget_destroy(dialogue);
			if (partie->joueurCourant)
				partie->joueur1.score=0;
			else
				partie->joueur2.score=0;
			nbMots=-1;
			/*affichage_motSuivant(partie);*/
			gtk_widget_show(partie->widgets.suivant);
		} else {
			affichage_terminerPartie(partie);
			partie->joueur1.score=partie->joueur2.score=0;
			gtk_widget_destroy(dialogue);
			nbMots=0;
		}
	} else if (partie->superPartie&&(partie->joueurCourant?partie->joueur1.score:partie->joueur2.score)==10) {
		GtkWidget* dialogue=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,
				"Bravo, vous avez gagné la super-partie !");
		gtk_dialog_run(GTK_DIALOG(dialogue));
		gtk_widget_destroy(dialogue);
		affichage_terminerPartie(partie);
		partie->joueur1.score=0;
		nbMots=0;
	} else{
		int i,j;
		
		affichage_miseAJourScoreNom(partie);
		
		for (i=0;i<partie->options.nbEssais;++i) {
			for(j=0;j<partie->options.lettresParMot;++j) {
				gtk_image_set_from_pixbuf(GTK_IMAGE(partie->widgets.casesimages[i][j]),partie->widgets.imgDefaut);
				gtk_label_set_label(GTK_LABEL(partie->widgets.caseslabels[i][j])," ");
			}
		}
		gtk_entry_set_text(GTK_ENTRY(partie->widgets.entree),"");
		gtk_widget_hide(partie->widgets.suivant);
		affichage_nouveauMot(partie);
		affichage_saisieMot (NULL, partie);
	}
}

gboolean affichage_rafraichissementTimer (gpointer param_partie)
{
	Partie* partie=(Partie*)param_partie;
	
	if (!partie->widgets.layout) /* La partie a été arrêtée entre temps */
		return FALSE;
	partie->tempsRestant-=g_timer_elapsed(partie->widgets.timer,NULL);
	g_timer_start(partie->widgets.timer);
	
	if (partie->tempsRestant<=0) { /* Temps écoulé */
		GtkWidget* dialogue;
		
		dialogue=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,
				"Vous n'avez pas répondu dans les délais.");
		gtk_dialog_run(GTK_DIALOG(dialogue));
		gtk_widget_destroy(dialogue);
		if (partie->superPartie) {
			dialogue=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,
					"Vous n'avez pas répondu dans les délais.\nLa super partie est terminée.");
			gtk_dialog_run(GTK_DIALOG(dialogue));
			gtk_widget_destroy(dialogue);
			affichage_terminerPartie(partie);
		}
		--partie->motCourant.essaisRestants;
		if (partie->options.nbJoueurs==2) {
			partie->joueurCourant=!partie->joueurCourant;
			jeu_ajouterLettre(&partie->motCourant);
		}
		affichage_miseAJourScoreNom(partie);
		affichage_saisieMot(NULL,partie);
		return FALSE;
	}
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(partie->widgets.affichageTimer),partie->tempsRestant/(gdouble)(partie->options.tempsReponse));
	return TRUE;
}

gboolean affichage_tableLettres (gpointer param_partie)
{
	Partie* partie=(Partie*)param_partie;
	static int numLettre=0;
	int ligne;
	char str[40];
	
	ligne=partie->options.nbEssais - partie->motCourant.essaisRestants - 1;
	
	if (partie->motCourant.motsSaisis[ligne][numLettre]=='\0') {
		numLettre=0;
		affichage_saisieMot(NULL,partie);
		return FALSE;
	}
	
	sprintf(str,"<span size=\"x-large\">%c</span>",partie->motCourant.motsSaisis[ligne][numLettre]);
	gtk_label_set_markup(GTK_LABEL(partie->widgets.caseslabels[ligne][numLettre]),str);
	switch (partie->motCourant.corrections[ligne][numLettre]) {
		case CORRECTION_BONNE_PLACE:
			gtk_image_set_from_pixbuf(GTK_IMAGE(partie->widgets.casesimages[ligne][numLettre]),partie->widgets.imgOK);
			break;
		case CORRECTION_MAUVAISE_PLACE:
			gtk_image_set_from_pixbuf(GTK_IMAGE(partie->widgets.casesimages[ligne][numLettre]),partie->widgets.imgMauvaisePos);
			break;
	}
	
	if (++numLettre==partie->options.lettresParMot) {
		numLettre=0;
		affichage_saisieMot(NULL,partie);
		return FALSE;
	}
	return TRUE;
}

void affichage_sauvegarder (GtkWidget* appelant, gpointer param_partie)
{
	Partie* partie=(Partie*)param_partie;
	
	if (!partie->widgets.layout) {
		GtkWidget* dialogue;
		
		dialogue=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,
				"Vous ne pouvez pas sauvegarder la partie : vous n'en avez pas commencé !");
		gtk_dialog_run(GTK_DIALOG(dialogue));
		gtk_widget_destroy(dialogue);
	} else {
		GtkWidget* selection;
		GtkFileFilter* filter;
		
		selection=gtk_file_chooser_dialog_new("Sauvegarder",GTK_WINDOW(partie->widgets.fenetre),GTK_FILE_CHOOSER_ACTION_SAVE,
				GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,GTK_STOCK_SAVE,GTK_RESPONSE_ACCEPT,NULL);
		VERIFIER_ALLOCATION(selection,"Impossible de créer la boîte de dialogue pour sauvegarder.\n",partie)
		gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(selection),TRUE);
		gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(selection),FALSE);
		
		filter=gtk_file_filter_new();
		VERIFIER_ALLOCATION(filter,"Impossible de créer le filtre des fichiers de sauvegardes.\n",partie)
		gtk_file_filter_add_pattern(filter,"*.sav");
		
		gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(selection),filter);
		
		if (gtk_dialog_run(GTK_DIALOG(selection))==GTK_RESPONSE_ACCEPT) {
			char* nom;
			
			nom=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(selection));
			if (jeu_sauvegarder(partie,nom)) {
				GtkWidget* dialog;
				
				dialog=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,
					"Impossible de sauvegarder dans le fichier %s.",nom);
				gtk_dialog_run(GTK_DIALOG(dialog));
				gtk_widget_destroy(dialog);
			}
			
			g_free(nom);
		}
		
		gtk_widget_destroy(selection);
	}
}

void affichage_charger (GtkWidget* appelant, gpointer param_partie)
{
	Partie* partie=(Partie*)param_partie;
	
	if (partie->widgets.layout) {
		GtkWidget* dialogue;
		
		dialogue=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,
				"Commencez par terminer la partie en cours.");
		gtk_dialog_run(GTK_DIALOG(dialogue));
		gtk_widget_destroy(dialogue);
	} else {
		GtkWidget* selection;
		GtkFileFilter* filter;
		int i,j;
		
		selection=gtk_file_chooser_dialog_new("Charger",GTK_WINDOW(partie->widgets.fenetre),GTK_FILE_CHOOSER_ACTION_OPEN,
				GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,GTK_STOCK_OPEN,GTK_RESPONSE_ACCEPT,NULL);
		VERIFIER_ALLOCATION(selection,"Impossible de créer la boîte de dialogue pour charger.\n",partie)
		gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(selection),FALSE);
		
		filter=gtk_file_filter_new();
		VERIFIER_ALLOCATION(filter,"Impossible de créer le filtre des fichiers de sauvegardes.\n",partie)
		gtk_file_filter_add_pattern(filter,"*.sav");
		
		gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(selection),filter);
		
		if (gtk_dialog_run(GTK_DIALOG(selection))==GTK_RESPONSE_ACCEPT) {
			char* nom;
			
			nom=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(selection));
			if (jeu_charger(partie,nom)) {
				GtkWidget* dialog;
				
				dialog=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,
					"Impossible de charger le fichier %s.",nom);
				gtk_dialog_run(GTK_DIALOG(dialog));
				gtk_widget_destroy(dialog);
			}
			
			g_free(nom);
		}
		
		gtk_widget_destroy(selection);
		
		/* Allocations dynamiques des tableaux nécessaires au jeu. */
		partie->widgets.casesimages=malloc(partie->options.nbEssais*sizeof(GtkWidget**));
		VERIFIER_ALLOCATION(partie->widgets.casesimages,"Impossible d'allouer le tableau de images.\n",partie);
		partie->widgets.caseslabels=malloc(partie->options.nbEssais*sizeof(GtkWidget**));
		VERIFIER_ALLOCATION(partie->widgets.caseslabels,"Impossible d'allouer le tableau de GtkEventBox.\n",partie);
		for(i=0;i<partie->options.nbEssais;++i) {
			partie->widgets.casesimages[i]=malloc(partie->options.lettresParMot*sizeof(GtkWidget*));
			VERIFIER_ALLOCATION(partie->widgets.casesimages[i],"Impossible d'allouer le tableau d'images.\n",partie);
			partie->widgets.caseslabels[i]=malloc(partie->options.lettresParMot*sizeof(GtkWidget*));
			VERIFIER_ALLOCATION(partie->widgets.caseslabels[i],"Impossible d'allouer le tableau de labels.\n",partie);
		}
		
		affichage_nouveauMot(partie);
		
		affichage_miseAJourScoreNom(partie);
		partie->widgets.layout=gtk_layout_new(NULL,NULL);
		VERIFIER_ALLOCATION(partie->widgets.layout,"Impossible d'allouer le layout.\n",partie);
		for (i=0;i<partie->options.nbEssais;++i) {
			for(j=0;j<partie->options.lettresParMot;++j) {
				partie->widgets.casesimages[i][j]=gtk_image_new_from_pixbuf(partie->widgets.imgDefaut);
				VERIFIER_ALLOCATION(partie->widgets.casesimages[i][j],"Impossible de créer une image.\n",partie);
				partie->widgets.caseslabels[i][j]=gtk_label_new(" ");
				VERIFIER_ALLOCATION(partie->widgets.caseslabels[i][j],"Impossible de créer un label.\n",partie);
				
				gtk_layout_put(GTK_LAYOUT(partie->widgets.layout),partie->widgets.casesimages[i][j],TAILLE_IMAGE*j,TAILLE_IMAGE*i);
				gtk_layout_put(GTK_LAYOUT(partie->widgets.layout),partie->widgets.caseslabels[i][j],10+TAILLE_IMAGE*j,3+TAILLE_IMAGE*i);
			}
		}
		gtk_box_pack_start(GTK_BOX(partie->widgets.boxprincipale),partie->widgets.layout,TRUE,TRUE,0);
		
		gtk_window_resize(GTK_WINDOW(partie->widgets.fenetre),partie->options.lettresParMot*32,150+partie->options.nbEssais*32);
		
		partie->widgets.entree=gtk_entry_new();
		VERIFIER_ALLOCATION(partie->widgets.entree,"Impossible de créer la zone de saisie.\n",partie)
		gtk_widget_set_tooltip_text(partie->widgets.entree,"Saisissez votre mot ici.");
		g_signal_connect(G_OBJECT(partie->widgets.entree),"activate",G_CALLBACK(affichage_saisieMot),partie);
		gtk_box_pack_start(GTK_BOX(partie->widgets.boxprincipale),partie->widgets.entree,FALSE,FALSE,0);
		
		partie->widgets.suivant=gtk_button_new_with_label("Mot suivant");
		VERIFIER_ALLOCATION(partie->widgets.entree,"Impossible de créer le bouton \"Mot suivant\".\n",partie)
		g_signal_connect(G_OBJECT(partie->widgets.suivant),"clicked",G_CALLBACK(affichage_motSuivant),partie);
		gtk_box_pack_start(GTK_BOX(partie->widgets.boxprincipale),partie->widgets.suivant,FALSE,FALSE,0);
		
		gtk_widget_show_all(partie->widgets.fenetre);
		gtk_widget_hide(partie->widgets.suivant);
		
		affichage_saisieMot(NULL,partie); /* Sert à lancer la parte (initialise ce qui doit l'être) */
		
		affichage_miseAJourScoreNom(partie);
	}
}

void affichage_terminer (GtkWidget* appelant, gpointer param_partie)
{
	Partie* partie=(Partie*)param_partie;
	int i;
	
	if (partie->widgets.layout) {
		for (i=0;i<partie->options.nbEssais;++i) {
			free(partie->widgets.casesimages[i]);
			free(partie->widgets.caseslabels[i]);
		}
		free(partie->widgets.casesimages);
		free(partie->widgets.caseslabels);
	}
	
	jeu_terminer(partie);
	
	free(partie);
	
	gtk_main_quit();
}
