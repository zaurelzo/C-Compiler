#include <stdio.h>
#include <stdlib.h>
//#include "tab_symb.h"
//#include "tab_label.h"
//#include "deuxieme_passe.h"
#include "gestion_des_fonctions.h"
//#include "asm.h"
int main()
{	parametres  p1 = {"a",1,0};
	parametres p2={"b",2,0};
	parametres  tab_param[2] = {p1,p1};
	parametres  tab_param2[2] = {p1,p2};
	
	if (ajouter_Prototype("fonction_test", 1, tab_param, 2)==0)
	{
		printf("AJOUT PROTOTYPE FONCTION TEST  \n") ;
	}else 
	{
		printf("ECHEC AJOUT PROTOTYPE FONCTION TEST \n");
	}

	if (ajouter_Prototype("fonction_test2", 1, tab_param2, 2)==0)
	{
		printf("AJOUT PROTOTYPE FONCTION TEST2  \n") ;
	}else 
	{
		printf("ECHEC AJOUT PROTOTYPE FONCTION TEST2 \n");
	}


	if (ajouter_Prototype("fonction", 1, tab_param2, 2)==0)
	{
		printf("AJOUT PROTOTYPE FONCTION \n");
	}else 
	{
		printf("ECHEC AJOUT PROTOTYPE FONCTION  \n");
	}

	if (ajouter_implementations("fonction" , 1, tab_param2, 22 , 2)==0)
	{
		printf("AJOUT IMPLEMENTATION FONCTION \n");
	} else
	{
		printf("ECHEC AJOUT IMPLEMENTATION FONCTION\n");
	}

	//preparation appel de fonction
	initNombreDeParametresAPPEL() ;
	 ajouter_parametreAPPEL(1);
	 ajouter_parametreAPPEL(2);
	 
	 parametres t[getNombredeParametresAPPEL()];
	 getTab_parametresAPPEL(t);

	 char  labelPotentiel[100];

	 //printf("nombre de parametre d'appel: %d \n",getNombredeParametresAPPEL() );
	 //printParametresDappel(  t,getNombredeParametresAPPEL()) ;

	 int retour = checkAppelFonctionParametreConforme("fonction_test2", t, getNombredeParametresAPPEL(), labelPotentiel) ;
	 if( retour ==-2)
	 {
	 	printf("LABEL :%s \n", labelPotentiel);
	 }else if (retour ==-1)
	 {
	 	printf("fonction non existante \n");
	 }else 
	 {
	 	printf("SUCCÉS LORS DE L'APPEL DE LA FONCTION , adresse implementation : %d \n",retour );
	 }
	
	return 0;
}
 
