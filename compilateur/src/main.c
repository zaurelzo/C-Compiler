#include <stdio.h>
#include <stdlib.h>
#include "../head/tab_symb.h"
//<<<<<<< HEAD
#include "../head/tab_label.h"
#include "../head/deuxieme_passe.h"
//=======
//#include "tab_label.h"
//#include "deuxieme_passe.h"
//>>>>>>> 3c65610403ddc4ee5af857ed36986294dc93881a
#include "../head/gestion_des_fonctions.h"

int main(int argc, char const *argv[])
{
	
	Noeud_label tab_label[TAILLE_TAB_LABEL] ; 
	Noeud_Fonctions tab_fonctions[NOMBRES_MAX_DE_FONCTIONS_A_GERER];

	 int nb_element_tab_label = recreer_tableLabel("fichier_table_des_labels", tab_label);
	 int nb_elt_tab_fonctions =  recreerTableFonctions("fichier_table_des_fonctions",tab_fonctions);
	//printf("%d\n",atoi("15\n"));
	remplacer_label(argv[1],argv[2],tab_label,tab_fonctions,nb_element_tab_label);
	
	return 0 ; 
}
 
