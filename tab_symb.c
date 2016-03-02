#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tab_symb.h"


 int ind = 0; 

 int indPile = ADRESSE_DEBUTPILE;

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
 
void printTabVar()
{
	
	int i =0 ;
	//printf("indice %d  ",ind);
	for (i=0 ; i<=ind;i++)
	{
		printf("%s \n", Array[i].nom_var );
	}
}

int  empiler(int value ,int type )
{
	char buf[32];
	sprintf(buf,"%d",value);
	strcpy(Array[indPile].nom_var,buf);
	Array[indPile].type=type;
	indPile--;  
	return indPile+1 ; 
} 


int depiler(int *  type_val_depile )
{
	 //* adresse = indPile; 
	if(indPile<=ADRESSE_DEBUTPILE) 
	{
			indPile++;
			*type_val_depile= Array[indPile].type; 
			//printf("adr ou l'on cherche type:%d\n",indPile);
	return atoi(Array[indPile].nom_var);
	}
	
	return -1 ; 	
} 

int obtenirAdressePremierOperande()
{
	return indPile;
}

int obtenirAdressDeuxiemeOperande()
{
	return indPile-1;
}



void modifierChampInitialiserVariable(char * nom_var)
{
	int i =0 ;

	
		for (i=0 ; i<ind;i++)
		{
			if ( strcmp(Array[i].nom_var,nom_var)==0)
				Array[i].initialiser=1;
		}
}

int getAdressePile()
{
	return indPile;
}
 


void viderPile()
{
	indPile=ADRESSE_DEBUTPILE;
}

int  getAdressePremierOperandeCondition()
{
	return ADRESSE_DEBUTPILE+1;
}
int  getAdresseDeuxiemeOperandeCondition()
{
	return ADRESSE_DEBUTPILE+2;
}

int getAdresseResultatComparaison()
{
	return ADRESSE_DEBUTPILE+3;
}

