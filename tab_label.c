#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tab_label.h"


 int indice = 0; 
 int pc=0;
 int indiceLabel= 0 ; 

char *  ajouter_label()
{	
	char num[10];
	sprintf(num, "%d", indice);
	
	char  nom_label[10]="label";
	strcat(nom_label,num);
	 strcpy(TABLEAU[indice].nomLabel,nom_label);
	 TABLEAU[indice].num_instruction=-1;
	 indice++;
	 return TABLEAU[indice-1].nomLabel ;
}
int modifierNum_instruction(char * nom_label,int val_pc)
{
	int i ; 
	for ( i = 0; i < indice; ++i)
	{
		if (strcmp(TABLEAU[i].nomLabel,nom_label)==0)
			{
				TABLEAU[i].num_instruction=val_pc;
				return 0;  
			}
	}
	return -1 ;
}

void afficherTab()
{
	int i ; 
	for ( i = 0; i < indice; ++i)
	{
		printf("nom : %s | val : %d \n",TABLEAU[i].nomLabel,TABLEAU[i].num_instruction);
	}
}

void incrementerPC()
{
	pc++;
}

void empilerLabel(char * label)
{
	strcpy(TAB_FILE_DES_LABELS[indiceLabel].nomLabel,label);
	indiceLabel++;
}

char * depilerLabel()
{
	int i ; 
	char  retour[20];
	if (indiceLabel>0)
	{
		 strcpy(retour ,TAB_FILE_DES_LABELS[0].nomLabel);
		indiceLabel--;
		for (i = 1; i <= indiceLabel; ++i)
		{
			strcpy( TAB_FILE_DES_LABELS[i-1].nomLabel,TAB_FILE_DES_LABELS[i].nomLabel);
			TAB_FILE_DES_LABELS[i-1].num_instruction= TAB_FILE_DES_LABELS[i].num_instruction;
		}
	}
	return retour;
}

void generer_fichier_tab_label()
{
	FILE* fichier = NULL;
 	int i ;
    fichier = fopen("fichier_table_des_symbols", "w+");
 
    if (fichier != NULL)
    {
    	//printf("******indice : %d"indiceLabel)
    	for ( i = 0; i < indice; ++i)
    	{
    		fprintf(fichier,"%s %d \n",TABLEAU[i].nomLabel,TABLEAU[i].num_instruction );
    	}
    	 fclose(fichier);
	}
       
}
 	