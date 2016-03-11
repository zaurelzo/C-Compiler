#include <stdio.h>
#include <stdlib.h>
#include "tab_symb.h"
#include "tab_label.h"
#include "deuxieme_passe.h"

int main()
{
	Noeud_label tab_label[50] ; 

	 int nb_element_tab_label = recreer_tableLabel("fichier_table_des_labels", tab_label);
	 //printf("-------------------%d\n", nb_element_tab_label);
	 remplacer_label("fichier_asm",tab_label,nb_element_tab_label);
	return 0 ; 
}
 
