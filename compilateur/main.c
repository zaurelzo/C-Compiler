#include <stdio.h>
#include <stdlib.h>
//#include "tab_symb.h"
//#include "tab_label.h"
//#include "deuxieme_passe.h"
#include "gestion_des_fonctions.h"
//#include "asm.h"
int main()
{	parametres  p1 = {1,0};
	parametres  tab_param[2] = {p1,p1};

	//printf("===%d\n",ajouter_Prototype("fonction_test", 0, tab_param, 2) );
	//printf("===%d\n",ajouter_Prototype("fonction_test", 0, tab_param, 2) );
	if (ajouter_Prototype("fonction_test", 1, tab_param, 2)==0)
	{
		printf("YES :) \n") ;
	}else 
	{
		printf("==PAS YES :( \n");
	}

	ajouter_parametre(1 , 0) ;
	ajouter_parametre(1 , 0) ;
	parametres  t[getNombredeParametres()] ;
	
	getTab_parametres(t) ;

	printf("%d\n",getNombredeParametres() );
	if (ajouter_Prototype("fonction_tes", 1, t, 2)==0)
	{
		printf("YES :) \n") ;
	}else 
	{
		printf("==PAS YES :( \n");
	}

	
	
	if (ajouter_implementations("fonction_tes" , 1, t ,44, getNombredeParametres())==0)
	{
		printf("IMPLEMENTATION FAITE  and NOMBRE DE PARAMETRES %d \n",getNombredeParametres());
		print_TABLE_DES_FONCTION();
	} else
	{
		printf("IMPLEMENTATION PAS FAITE \n");
	}

	/*if (ajouter_implementations("fonction_test" , 1, tab_param,4, 2)==0)
	{
		printf("IMPLEMENTATION FAITE \n");
	} else
	{
		printf("IMPLEMENTATION PAS FAITE \n");
	}*/

	return 0;
}
 
