#ifndef __DEUXIEMEPASSE__
#define __DEUXIEMEPASSE__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tab_label.h"

int recreer_tableLabel(char * fichier_table_label, Noeud_label tab_label[]);

void remplacer_label(char * nom_fichier_ass, Noeud_label tab_label[], int nbelement_tabLabel);


#endif
