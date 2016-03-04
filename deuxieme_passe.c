#include "deuxieme_passe.h"

Noeud_label traiter_ligne(char * line)
{
	Noeud_label Noeud_courant;
	char * token ; 
	printf("Valuer de token 1 :"); 			
	token =strtok(line," ");
	printf("Valuer de token :  %s", token); 			
	while(token!=NULL)			
	{
		strcpy(Noeud_courant.nomLabel, token);
		token=strtok(NULL," ");			
		Noeud_courant.num_instruction=atoi(token);
	} 
	return Noeud_courant;
}


int recreer_tableLabel(char * fichier_table_label, Noeud_label tab_label[])
{	
	int i=0;	
	FILE * fichier;
	char line[20];

	
	fichier=fopen(fichier_table_label, "r");
	if(fichier!=NULL)
	{		
		while(fgets(line, 20, fichier)!=NULL)
		{ 		
			printf("Valuer de line :  %s", line); 			
			tab_label[i]=traiter_ligne(line);		
			i++;		
		}
	}
	return i--;
}

void remplacer_label(char * nom_fichier_ass, Noeud_label tab_label[], int nbelement_tabLabel)
{
	int i;	
	FILE * fichier=NULL;
	FILE * fichier_retour=NULL;
	char line[20];
	char * element_ligne;
	char * mot_courant=NULL;

	fichier=fopen(nom_fichier_ass, "r");
	fichier_retour=fopen("fichier_ass_correct", "w+");
	if(nom_fichier_ass!=NULL)
	{
		while(fgets(line, 20, fichier)!=NULL)
		{
			element_ligne=strtok(line," ");
			while(element_ligne!=NULL)			
			{
				strcpy(mot_courant, element_ligne);
				//Dans le cas où la ligne commence par un JMF ou JMP				
				if(strcmp(mot_courant, "JMP")==0 || strcmp(mot_courant, "JMF")==0)
				{
					//On écrit JMP ou JMF dans le fichier						
					fputs(mot_courant, fichier_retour);					
					//On lit le mot "labelx"									
					element_ligne=strtok(NULL," ");	
					strcpy(mot_courant, element_ligne);
					//On cherche le label correspondant dans la table des labels								
					for(i=0; i<nbelement_tabLabel; i++)
					{
						if(strcmp(mot_courant, tab_label[i].nomLabel)==0)
						{
							sprintf(mot_courant, " %d\n", tab_label[i].num_instruction);
							//On écrit après JMP ou JMF le numéro de PC		
							fputs(mot_courant, fichier_retour);
						}
					}
				}
				else
				{
					//Si la ligne ne commence pas par JMP ou JMF on l'écrit dans le fichier					
					fputs(line, fichier_retour);
				}
			}
		}
	}

}
				
				








