#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tab_symb.h"


 int ind=0;

int  ajouter_Var(char *nom_var,int type ,int initialiser ,int constante ) 
{

	int i =0 ;

	if (ind!=0)
	{
		for (i=0 ; i<=ind-1;i++)
		{
			if ( strcmp(Array[i].nom_var,nom_var)==0)
				return -1 ;
		}
	}
	
	strcpy(Array[ind].nom_var,nom_var); 
	Array[ind].type = type ; 
	Array[ind].initialiser=initialiser; 
	Array[ind].constante = constante ; 
	ind++;

	return 0 ; 
}

int recherche(char * nom_var )
{
	int i =0 ;

	for (i=0 ; i<=ind;i++)
	{
		if (strcmp(Array[i].nom_var,nom_var)==0)
			return i ;
	}

	return -1 ; 

}

void printTab()
{
	
	int i =0 ;
	//printf("indice %d  ",ind);
	for (i=0 ; i<=ind;i++)
	{
		printf("%s \n", Array[i].nom_var );
	}
}
