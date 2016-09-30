#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../head/tab_label.h"


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

int getPcValue()
{
	return pc;
}

void empilerLabelIF(char * label1 )
{
	strcpy(TAB_FILE_DES_LABELS[indiceLabel].type,"if");	
	strcpy(TAB_FILE_DES_LABELS[indiceLabel].label1,label1);	
	indiceLabel++;
}

void empilerPremierLabelWhile(char * label1 )
{
	strcpy(TAB_FILE_DES_LABELS[indiceLabel].type,"while");	
	strcpy(TAB_FILE_DES_LABELS[indiceLabel].label1,label1);	
	//indiceLabel++;
}



void empilerDeuxiemeLabelWhile(char * label2)
{
	//strcpy(TAB_FILE_DES_LABELS[indiceLabel].type,"if");	
	strcpy(TAB_FILE_DES_LABELS[indiceLabel].label2,label2);	
	indiceLabel++;
}

void empilerPremierLabelIF(char * label1 )
{
	strcpy(TAB_FILE_DES_LABELS[indiceLabel].type,"ifElse");	
	strcpy(TAB_FILE_DES_LABELS[indiceLabel].label1,label1);	
}
void empilerDeuxiemeLabelIF(char * label2) 
{
	//strcpy(TAB_FILE_DES_LABELS[indiceLabel].type,"if");	
	strcpy(TAB_FILE_DES_LABELS[indiceLabel].label2,label2);	
	indiceLabel++;
}

/*void  depilerLabelIF(char  * retour) 
{
	
	if (indiceLabel>0)
	{

		
		 if (strcmp(TAB_FILE_DES_LABELS[indiceLabel-1].type,"if")==0)
		{
			indiceLabel--;
			strcpy(retour ,TAB_FILE_DES_LABELS[indiceLabel].label1);
		}
	}
	//return retour;
}*/

void  depilerPremierLabelWhile(char  * retour) 
{
	if (indiceLabel>0)
	{
		if (strcmp(TAB_FILE_DES_LABELS[indiceLabel-1].type,"while")==0)
		{
			strcpy(retour ,TAB_FILE_DES_LABELS[indiceLabel-1].label1);
		}
	}
		
	//return retour;
}

void  depilerDeuxiemeLabelWhile(char  * retour) 
{
	//char  retour[40]="";
	if (indiceLabel>0)
	{
		if (strcmp(TAB_FILE_DES_LABELS[indiceLabel-1].type,"while")==0)
		{
			strcpy(retour ,TAB_FILE_DES_LABELS[indiceLabel-1].label2);
			indiceLabel--;
		}
	}
}


void  depilerPremierLabelIF(char  * retour)  
{

	if (indiceLabel>0)
	{
		if (strcmp(TAB_FILE_DES_LABELS[indiceLabel-1].type,"ifElse")==0)
		{
			strcpy(retour ,TAB_FILE_DES_LABELS[indiceLabel-1].label1);
		}
	}
}
void  depilerDeuxiemeLabelIF(char  * retour) 
{
	//char  retour[40]="";
	if (indiceLabel>0)
	{
		if (strcmp(TAB_FILE_DES_LABELS[indiceLabel-1].type,"ifElse")==0)
		{
			strcpy(retour ,TAB_FILE_DES_LABELS[indiceLabel-1].label2);
			indiceLabel--;
		}
	}
}


void generer_fichier_tab_label()
{
	FILE* fichier = NULL;
 	int i ;
    fichier = fopen("fichier_table_des_labels", "w+");
 
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
 	

void  enleverConnard(char * mot, char *  retour  )
{
	int i ; 
	//char retour [TAILLE]; 
	strcpy(retour,mot); 
	retour[strlen(mot)-1]='\0';
	///return retour; 
}	


int  getNumeroInstruction(char * label , Noeud_label * tab, int j ) 
{
	int i ; 
	char retou [TAILLE] ;
	enleverConnard(label,retou) ;  
	//printf("---------%s\n",retou);
	for(i=0; i<j; i++)
	{

		if(strcmp( retou ,tab[i].nomLabel)==0)
		{
			return tab[i].num_instruction ;
			//sprintf(mot_courant, " %d\n", tab_label[i].num_instruction);

		}
	}

	return -1 ; 
}