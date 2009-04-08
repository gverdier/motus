#include "Affichage_Bingo.h"

void affichage_bingo_lancer (Joueur* joueur, GtkWindow* parent)
{
	int i,j;
	BingoWidgets* widgets=&joueur->bingo.widgets; /* Pour simplifier l'écriture. */
	
	/* Création des widgets. */
	widgets->terminer=NULL;
	widgets->fenetre=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	BINGO_VERIFIER_ALLOCATION(widgets->fenetre,"Erreur à l'allocation de la fenêtre de bingo.\n",return;)
	widgets->box=gtk_vbox_new(FALSE,0);
	BINGO_VERIFIER_ALLOCATION(widgets->box,"Erreur à l'allocation de la box principale de bingo.\n",return;)
	widgets->boxMotus=gtk_hbox_new(FALSE,0);
	BINGO_VERIFIER_ALLOCATION(widgets->boxMotus,"Erreur à l'allocation de la box pour le mot \"MOTUS\".\n",return;)
	widgets->nom=gtk_label_new(joueur->nom);
	BINGO_VERIFIER_ALLOCATION(widgets->boxMotus,"Erreur à l'allocation du label pour le nom du joueur.\n",return;)
	widgets->table=gtk_table_new(5,5,FALSE);
	BINGO_VERIFIER_ALLOCATION(widgets->table,"Erreur à l'allocation de la table pour la grille de mouts.\n",return;)
	for (i=0;i<5;++i)
		for(j=0;j<5;++j) {
			widgets->events[i][j]=gtk_event_box_new();
			BINGO_VERIFIER_ALLOCATION(widgets->events[i][j],"Erreur à l'allocation d'une \"Event Box\".\n",return;)
			widgets->layouts[i][j]=gtk_layout_new(NULL,NULL);
			BINGO_VERIFIER_ALLOCATION(widgets->layouts[i][j],"Erreur à l'allocation d'un layout.\n",return;)
			widgets->images[i][j]=gtk_image_new_from_file("FondBingo.png");
			BINGO_VERIFIER_ALLOCATION(widgets->images[i][j],"Erreur à la création d'une image.\n",return;)
			widgets->lettres[i][j]=gtk_label_new(" ");
			BINGO_VERIFIER_ALLOCATION(widgets->lettres[i][j],"Erreur à la création d'un label.\n",return;)
			
			gtk_layout_put(GTK_LAYOUT(widgets->layouts[i][j]),widgets->images[i][j],0,0);
			gtk_layout_put(GTK_LAYOUT(widgets->layouts[i][j]),widgets->lettres[i][j],10,8);
			gtk_widget_set_size_request(widgets->layouts[i][j],TAILLE_IMAGE,TAILLE_IMAGE);
			gtk_container_add(GTK_CONTAINER(widgets->events[i][j]),widgets->layouts[i][j]);
			gtk_table_attach(GTK_TABLE(widgets->table),widgets->events[i][j],i,i+1,j,j+1,GTK_EXPAND|GTK_FILL,GTK_EXPAND|GTK_FILL,0,0);
			
			g_signal_connect(G_OBJECT(widgets->events[i][j]),"button-press-event",G_CALLBACK(affichage_bingo_clicCase),joueur);
		}
	for (j=0;j<5;++j) {
		widgets->layouts[i][j]=gtk_layout_new(NULL,NULL);
		BINGO_VERIFIER_ALLOCATION(widgets->layouts[i][j],"Erreur à l'allocation d'un layout.\n",return;)
		widgets->images[i][j]=gtk_image_new_from_file("FondBingo.png");
		BINGO_VERIFIER_ALLOCATION(widgets->images[i][j],"Erreur à la création d'une image.\n",return;)
		widgets->lettres[i][j]=gtk_label_new(" ");
		BINGO_VERIFIER_ALLOCATION(widgets->lettres[i][j],"Erreur à la création d'un label.\n",return;)
		gtk_layout_put(GTK_LAYOUT(widgets->layouts[i][j]),widgets->images[i][j],0,0);
		gtk_layout_put(GTK_LAYOUT(widgets->layouts[i][j]),widgets->lettres[i][j],10,8);
		gtk_widget_set_size_request(widgets->layouts[i][j],TAILLE_IMAGE,TAILLE_IMAGE);
		gtk_box_pack_start(GTK_BOX(widgets->boxMotus),widgets->layouts[i][j],TRUE,TRUE,0);
	}
	gtk_box_pack_start(GTK_BOX(widgets->box),widgets->nom,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(widgets->box),widgets->table,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(widgets->box),widgets->boxMotus,FALSE,FALSE,10);
	gtk_container_add(GTK_CONTAINER(widgets->fenetre),widgets->box);
	
	affichage_bingo_rafraichirLettres(joueur);
	
	gtk_window_set_transient_for(GTK_WINDOW(widgets->fenetre),parent);
	gtk_window_set_modal(GTK_WINDOW(widgets->fenetre),TRUE);
	gtk_window_set_position(GTK_WINDOW(widgets->fenetre),GTK_WIN_POS_CENTER);
	gtk_window_resize(GTK_WINDOW(widgets->fenetre),32*5,32*5+20);
	gtk_widget_show_all(widgets->fenetre);
	
	g_signal_connect(G_OBJECT(widgets->fenetre),"delete-event",G_CALLBACK(affichage_bingo_terminerFenetre),joueur);
}

void affichage_bingo_clicCase (GtkWidget* appelant, GdkEventButton* bouton, gpointer param_joueur)
{
	Joueur* joueur=(Joueur*)param_joueur;
	GtkWidget* dialogue;
	int i,j;
	int flag=0;
	
	if (joueur->bingo.widgets.terminer) {
		dialogue=gtk_message_dialog_new(GTK_WINDOW(joueur->bingo.widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_WARNING,
				GTK_BUTTONS_OK,"Vous avez déjà joué !");
		gtk_dialog_run(GTK_DIALOG(dialogue));
		gtk_widget_destroy(dialogue);
		return;
	}
	
	/* Cherche sur quelle case l'utilisateur a cliqué. */
	for (i=0;i<5&&!flag;++i)
		for (j=0;j<5&&!flag;++j)
			if (appelant==joueur->bingo.widgets.events[j][i])
				flag=1;
	/* Les variables i et j ont été incrémentées une fois de trop. */
	--i;
	--j;
	if (!flag) {
		affichage_erreur("Impossible de trouver sur quelle case vous avez cliqué. Fin du bingo.\n");
		return;
	}
	switch (jeu_bingo_gratter(&joueur->bingo,5*i+j+1)) {
		case 1:
			dialogue=gtk_message_dialog_new(GTK_WINDOW(joueur->bingo.widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_WARNING,
					GTK_BUTTONS_OK,"Vous avez déjà cliqué sur cette case !");
			gtk_dialog_run(GTK_DIALOG(dialogue));
			gtk_widget_destroy(dialogue);
			break;
		case 2:
			if (*joueur->nbJoueurs==2) {
				dialogue=gtk_message_dialog_new(GTK_WINDOW(joueur->bingo.widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,
						GTK_BUTTONS_OK,"Vous avez tiré une boule noire.\nLa main passe.");
				gtk_dialog_run(GTK_DIALOG(dialogue));
				*(joueur->joueurCourant)=!*(joueur->joueurCourant);
			} else {
				dialogue=gtk_message_dialog_new(GTK_WINDOW(joueur->bingo.widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,
						GTK_BUTTONS_OK,"Vous avez tiré une boule noire.\nVous perdez 50 points.");
				gtk_dialog_run(GTK_DIALOG(dialogue));
				joueur->score-=50;
			}
			gtk_widget_destroy(dialogue);
			affichage_bingo_aJoue(joueur);
			break;
		case 3:
			dialogue=gtk_message_dialog_new(GTK_WINDOW(joueur->bingo.widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,
					GTK_BUTTONS_OK,"Vous avez tiré un joker : vous pouvez rejouer !");
			gtk_dialog_run(GTK_DIALOG(dialogue));
			gtk_widget_destroy(dialogue);
			affichage_bingo_rafraichirLettres(joueur);
			gtk_widget_show_all(joueur->bingo.widgets.fenetre);
			break;
		default:
		{
			if (!memcmp(joueur->bingo.motusBingo,"MOTUS",5)) {
				dialogue=gtk_message_dialog_new(GTK_WINDOW(joueur->bingo.widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,
						GTK_BUTTONS_OK,"MOTUS ! Vous gagnez 100 points !\nLa (les) grille(s) est (sont) réinitialisée(s).");
				gtk_dialog_run(GTK_DIALOG(dialogue));
				gtk_widget_destroy(dialogue);
				joueur->score+=100;
				jeu_bingo_initialiser(&joueur->bingo);
				jeu_bingo_initialiser(joueur->bingoAutreJoueur);
			}
			affichage_bingo_aJoue(joueur);
		}
	}
}

void affichage_bingo_aJoue (Joueur* joueur)
{
	affichage_bingo_rafraichirLettres(joueur);
	
	joueur->bingo.widgets.terminer=gtk_button_new_with_label("Terminer");
	BINGO_VERIFIER_ALLOCATION(joueur->bingo.widgets.terminer,"Erreur à l'allocation du bouton \"Terminer\".\n",
			gtk_widget_destroy(joueur->bingo.widgets.fenetre);)
	gtk_box_pack_start(GTK_BOX(joueur->bingo.widgets.box),joueur->bingo.widgets.terminer,FALSE,FALSE,0);
	g_signal_connect(G_OBJECT(joueur->bingo.widgets.terminer),"clicked",G_CALLBACK(affichage_bingo_terminer),joueur);
	gtk_widget_show_all(joueur->bingo.widgets.box);
}

void affichage_bingo_rafraichirLettres (Joueur* joueur)
{
	int i,j;
	char str[2];
	
	str[1]='\0';
	for (i=0;i<5;++i)
		for (j=0;j<5;++j)
			if (joueur->bingo.grilleBingo[i][j].gratte) {
				str[0]=joueur->bingo.grilleBingo[i][j].lettre;
				gtk_label_set_label(GTK_LABEL(joueur->bingo.widgets.lettres[i][j]),str);
			}
	for (i=0;i<5;++i)
		if (joueur->bingo.motusBingo[i]!='*') {
			str[0]=joueur->bingo.motusBingo[i];
			gtk_label_set_label(GTK_LABEL(joueur->bingo.widgets.lettres[5][i]),str);
		}
}

void affichage_bingo_terminer (GtkWidget* appelant, gpointer joueur) {
	gtk_widget_destroy(((Joueur*)joueur)->bingo.widgets.fenetre);
}

gboolean affichage_bingo_terminerFenetre (GtkWidget* appelant, gpointer param_joueur) {
	GtkWidget* dialogue;
	Joueur* joueur=(Joueur*)param_joueur;
	
	if (!joueur->bingo.widgets.terminer) {
		dialogue=gtk_message_dialog_new(GTK_WINDOW(joueur->bingo.widgets.fenetre),GTK_DIALOG_MODAL,GTK_MESSAGE_WARNING,
				GTK_BUTTONS_OK,"Vous devez jouer le bingo.");
		gtk_dialog_run(GTK_DIALOG(dialogue));
		gtk_widget_destroy(dialogue);
		return TRUE;
	}
	return FALSE;
}

void affichage_erreur (const char* message)
{
	g_printerr(message);
}