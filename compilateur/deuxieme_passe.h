#ifndef __DEUXIEMEPASSE__
#define __DEUXIEMEPASSE__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tab_label.h"
#include "gestion_des_fonctions.h"

int recreer_tableLabel(char * fichier_table_label, Noeud_label * tab_label);
int recreerTableFonctions(char * fichier_table_des_fonctions,Noeud_Fonctions * tab_fonctions);
int  traiter_ligne_du_fichier(char * line, char ** tabToken);
void remplacer_label(const char * fichier_avec_labels, const char * fichier_sans_labels, Noeud_label * tab_label, Noeud_Fonctions * tab_fonctions , int nbelement_tabLabel);
int getIndiceIntableFonctions(char * labelFonctions ) ;
#endif
