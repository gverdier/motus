#include "Affichage.h"

void lancer_motus (int* argc, char*** argv)
{
	Partie partie;
	GtkWidget* image;
	
	gtk_init(argc,argv);
	srand(time(NULL));
	
	partie.widgets.splash=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	if (partie.widgets.splash==NULL) {
		affichage_erreur("Impossible d'ouvrir l'écran de démarrage.\n");
	} else {
		image=gtk_image_new_from_file("Motus.jpg");
		if (image==NULL) {
			affichage_erreur("Impossible de charger l'écran de démarrage.\n");
		} else {
			gtk_window_set_decorated(GTK_WINDOW(partie.widgets.splash),FALSE);
			gtk_window_set_position(GTK_WINDOW(partie.widgets.splash),GTK_WIN_POS_CENTER);
			gtk_container_add(GTK_CONTAINER(partie.widgets.splash),image);
			gtk_widget_show_all(partie.widgets.splash);
			g_timeout_add(1000,(GSourceFunc)affichage_splashDestroy,&partie);
		}
	}
	gtk_main();
}

void affichage_initialiser (Partie* partie)
{
	GtkWidget* image; /* Pour charger les images de fond */
	jeu_initialiser(partie);

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
	
	partie->widgets.scores=gtk_label_new("");
	VERIFIER_ALLOCATION(partie->widgets.scores,"Impossible de créer le label d'affichage des scores.\n",partie)
	partie->widgets.affichageTimer=gtk_progress_bar_new();
	VERIFIER_ALLOCATION(partie->widgets.affichageTimer,"Impossible de créer la barre de progression d'affichage du timer.\n",partie)
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(partie->widgets.affichageTimer),"Temps restant");
	gtk_progress_bar_set_orientation(GTK_PROGRESS_BAR(partie->widgets.affichageTimer),GTK_PROGRESS_RIGHT_TO_LEFT);
	
	partie->widgets.timer=g_timer_new();
	VERIFIER_ALLOCATION(partie->widgets.timer,"Impossible de créer le timer.\n",partie)
	
	/* La grille de motus et les cases ne seront crées qu'au début de la partie (pas au début du jeu). */
	partie->widgets.layout=NULL;
	
	/* Connexion des signaux aux fonctions de rappel */
	g_signal_connect(G_OBJECT(partie->widgets.fenetre),"delete-event",G_CALLBACK(affichage_terminer),partie);
	
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

void affichage_rafraichirFond (Partie* partie)
{
	if (partie->widgets.layout) { /* Le rafraîchissement ne se fait que si une partie a été commencée. */
		int i,j;
		
		for (i=0;i<partie->options.nbEssais-partie->motCourant.essaisRestants;++i) {
			for(j=0;j<partie->options.lettresParMot;++j) {
				switch (partie->motCourant.corrections[i][j]) {
					case CORRECTION_BONNE_PLACE:
						gtk_image_set_from_pixbuf(GTK_IMAGE(partie->widgets.casesimages[i][j]),partie->widgets.imgOK);
						break;
					case CORRECTION_MAUVAISE_PLACE:
						gtk_image_set_from_pixbuf(GTK_IMAGE(partie->widgets.casesimages[i][j]),partie->widgets.imgMauvaisePos);
						break;
					case CORRECTION_NON_PRESENT:
						gtk_image_set_from_pixbuf(GTK_IMAGE(partie->widgets.casesimages[i][j]),partie->widgets.imgDefaut);
						break;
				}
			}
		}
		
		for (;i<partie->options.nbEssais;++i) {
			for(j=0;j<partie->options.lettresParMot;++j) {
				gtk_image_set_from_pixbuf(GTK_IMAGE(partie->widgets.casesimages[i][j]),partie->widgets.imgDefaut);
			}
		}
	}
}

void affichage_erreur (const char* message)
{
	g_printerr(message);
}

void affichage_nouvellePartie (GtkWidget* appelant, gpointer param_partie)
{
	Partie* partie=(Partie*)param_partie;
	char scoresLabel[12];
	int i,j;
	
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
	
	sprintf(scoresLabel,"Score : %d",partie->joueur1.score);
	gtk_label_set_label(GTK_LABEL(partie->widgets.scores),scoresLabel);
	
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
	
	gtk_window_resize(GTK_WINDOW(partie->widgets.fenetre),partie->options.lettresParMot*32,120+partie->options.nbEssais*32);
	
	partie->widgets.entree=gtk_entry_new();
	VERIFIER_ALLOCATION(partie->widgets.entree,"Impossible de créer la zone de saisie.\n",partie)
	gtk_widget_set_tooltip_text(partie->widgets.entree,"Saisissez votre mot ici.");
	g_signal_connect(G_OBJECT(partie->widgets.entree),"activate",G_CALLBACK(affichage_saisieMot),partie);
	gtk_box_pack_start(GTK_BOX(partie->widgets.boxprincipale),partie->widgets.entree,FALSE,FALSE,0);
	
	gtk_widget_show_all(partie->widgets.fenetre);
	
	affichage_saisieMot (NULL, partie); /* Sert à lancer la parte (initialise ce qui doit l'être) */
}

void affichage_nouveauMot (Partie* partie)
{
	if (jeu_tirerMot(partie->motCourant.mot,partie->options.lettresParMot,partie->options.modeDiabolique))
		affichage_terminer(NULL,partie);
	
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
	gtk_widget_set_tooltip_text(tempsReponse,"Le nombre de secondes dont vous disposez pour répondre");
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
	bingo=gtk_check_button_new_with_label("Bingo ?");
	VERIFIER_ALLOCATION(bingo,"Impossible de créer la case à cocher \"Bingo ?\".\n",partie);
	gtk_widget_set_tooltip_text(bingo,"Indique si vous souhaitez que les bingos soient présents après avoir trouvé un mot.");
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

gboolean affichage_splashDestroy (gpointer param_partie)
{
	Partie* partie=(Partie*)param_partie;
	
	gtk_widget_destroy(partie->widgets.splash);
	affichage_initialiser(partie);
	
	return FALSE;
}

void affichage_nouvelleSuperPartie (GtkWidget* appelant, gpointer param_partie)
{
	GtkWidget* dialogue;
	
	dialogue=gtk_message_dialog_new(GTK_WINDOW(((Partie*)(param_partie))->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,
			"Le mode super partie n'est pas encore disponible.");
	VERIFIER_ALLOCATION(dialogue,"Impossible de créer la boîte de message.\n",(Partie*)param_partie)
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
	
	if (entry!=NULL) {
		int i;
		
		if(timeoutnum!=-1)
			g_source_remove(timeoutnum);
		--partie->motCourant.essaisRestants;
		ligne=partie->options.nbEssais - partie->motCourant.essaisRestants - 1;
		
		strcpy(partie->motCourant.motsSaisis[ligne],(char*)gtk_entry_get_text(GTK_ENTRY(entry)));
		for (i=(int)strlen(partie->motCourant.motsSaisis[ligne]);i<partie->options.lettresParMot;++i)
			partie->motCourant.motsSaisis[ligne][i]='\0';
		gagne=jeu_corrigerMot(&(partie->motCourant),ligne,partie->options.lettresParMot);
		
		g_timeout_add(500,(GSourceFunc)affichage_tableLettres,partie);
	} else {
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
			
			dialogue=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,
					"Vous avez perdu.\nLe mot était : %s.",partie->motCourant.mot);
			gtk_dialog_run(GTK_DIALOG(dialogue));
			gtk_widget_destroy(dialogue);
			affichage_terminer(NULL,partie);
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

gboolean affichage_rafraichissementTimer (gpointer param_partie)
{
	Partie* partie=(Partie*)param_partie;
	double tempsRestant;
	/*char str[20];*/
	
	tempsRestant=(double)partie->options.tempsReponse-g_timer_elapsed(partie->widgets.timer,NULL);
	if (tempsRestant<=0) { /* Temps écoulé */
		GtkWidget* dialogue;
		
		dialogue=gtk_message_dialog_new(GTK_WINDOW(partie->widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,
				"Vous n'avez pas répondu dans les délais.");
		gtk_dialog_run(GTK_DIALOG(dialogue));
		gtk_widget_destroy(dialogue);
		--partie->motCourant.essaisRestants;
		affichage_saisieMot(NULL,partie);
		return FALSE;
	}
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(partie->widgets.affichageTimer),tempsRestant/(gdouble)(partie->options.tempsReponse));
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
	gtk_main_quit();
}
