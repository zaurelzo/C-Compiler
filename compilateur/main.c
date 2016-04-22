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

	if (ajouter_parametre(1 , 0 , "a") ==-1)
	{
		printf("PARAMETRE EXISTE\n") ;
	}else 
	{
		printf("N'EXISTE PAS \n");
	}
	if (ajouter_parametre(1 , 0 , "c") ==-1)
	{
		printf("PARAMETRE EXISTE\n") ;
	}else
	{
		printf("N'EXISTE PAS \n");
	}
	/*parametres  t[getNombredeParametres()] ;
	
	getTab_parametres(t) ;

	printf("%d\n",getNombredeParametres() );
	if (ajouter_Prototype("fonction_tes", 1, t, 2)==0)
	{
		printf("YES :) \n") ;
	}else 
	{
		printf("==PAS YES :( \n");
	}*/

	
	
	return 0;
}
 
