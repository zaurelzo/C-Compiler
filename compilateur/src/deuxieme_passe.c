#include "../head/deuxieme_passe.h"



int recreer_tableLabel(char * fichier_table_label, Noeud_label * tab_label)
{	
	int i=0;	
	FILE * fichier;
	char line[40];
	//Noeud_label l ;
	char *  tabToken[TAILLE_TAB_LABEL] ; 
	
	fichier=fopen(fichier_table_label, "r");
	if(fichier!=NULL)
	{		
		while(fgets(line, 40, fichier)!=NULL)
		{ 		
			//printf("Valuer de line :  %s", line); 	
			traiter_ligne_du_fichier(line , tabToken);
			//printf("///////////////\n");		
			tab_label[i].num_instruction=atoi(tabToken[1]);
			strcpy(tab_label[i].nomLabel,tabToken[0]); 
			i++;		
		}
	}
	return i--;
}


int recreerTableFonctions(char * fichier_table_des_fonctions,Noeud_Fonctions * tab_fonctions)
{	
	int i=0;	
	FILE * fichier;
	char line[40];
	//Noeud_label l ;
	char *  tabToken[100] ; 
	
	fichier=fopen(fichier_table_des_fonctions, "r");
	if(fichier!=NULL)
	{		
		while(fgets(line, 40, fichier)!=NULL)
		{ 		
			traiter_ligne_du_fichier(line , tabToken);		
			tab_fonctions[i].adresse_implementation=atoi(tabToken[0]);
			//printf("======%d\n", tab_fonctions[i].adresse_implementation);
			i++;		
		}
	}
	return i--;
}

int getIndiceIntableFonctions(char * labelFonctions ) 
{
	char nb[100];
	strcpy(nb,&(labelFonctions[15]));
	nb[strlen(nb-1)]='\0';
	return atoi(nb);
}

int  traiter_ligne_du_fichier(char * line, char ** tabToken)
{
	Noeud_label Noeud_courant;
	char * token ; 
	int cpt = 0 ;
	//printf("Valuer de token 1 :"); 			
	token =strtok(line," ");
	//printf("Valuer de token :  %s\n", token); 			
	while(token!=NULL)			
	{
		tabToken[cpt]=malloc(sizeof(char)*TAILLE);
		strcpy(tabToken[cpt], token);
		cpt++;
		token=strtok(NULL," ");	
		//strcpy(tabToken[cpt], token);
		//printf("Valuer de token :  %s\n", token); 		
	} 
	return cpt;
}

void remplacer_label(const char * fichier_avec_labels, const char * fichier_sans_labels, Noeud_label * tab_label, Noeud_Fonctions * tab_fonctions , int nbelement_tabLabel)
{
	int i, num,retour,k;	
	FILE * fichier=NULL;
	FILE * fichier_retour=NULL;
	FILE * fichier_retourbis=NULL;
	
	char line[40];
	char elementcourant[40];
	char  element_ligne[TAILLE];
	char * tabToken[5];

	fichier=fopen(fichier_avec_labels, "r");
	fichier_retourbis=fopen(fichier_sans_labels, "w+");

	//debug 
	//printf("with labels : %s | without labels : %s \n",fichier_avec_labels , fichier_sans_labels) ;
	
	if (fichier_retourbis==NULL && fichier ==NULL)
	{
		printf("ERROR WHEN OPEN FILES \n");
		exit(1);
	}

	while(fgets(line, 40, fichier)!=NULL)
	{
			strcpy(elementcourant,line) ; //printf("Ligne %s\n", line);
			retour = traiter_ligne_du_fichier(line,tabToken);
			//printf("valeur de retour %d \n", retour);
			/*if( strcmp(tabToken [0],"RET")==0)
			{
				printf("RETOUR :%d and line :%s",retour,elementcourant);
			}*/

			//************************ecriture dans le fichier de sorti *********************

			if( strcmp(tabToken [0],"ADD") ==0) fprintf(fichier_retourbis,"%s %s %s %s",tabToken[0],tabToken[1],tabToken[2],tabToken[3]);
							
			else if( strcmp(tabToken [0],"MUL")==0) fprintf(fichier_retourbis,"%s %s %s %s",tabToken[0],tabToken[1],tabToken[2],tabToken[3]);
							

			else if( strcmp(tabToken [0],"SOU")==0) fprintf(fichier_retourbis,"%s %s %s %s",tabToken[0],tabToken[1],tabToken[2],tabToken[3]);
							
			else if( strcmp(tabToken [0],"DIV")==0) fprintf(fichier_retourbis,"%s %s %s %s",tabToken[0],tabToken[1],tabToken[2],tabToken[3]);
							
			else if( strcmp(tabToken [0],"COP")==0) fprintf(fichier_retourbis,"%s %s %s",tabToken[0],tabToken[1],tabToken[2]);


			else if( strcmp(tabToken [0],"COPA")==0) fprintf(fichier_retourbis,"%s %s %s",tabToken[0],tabToken[1],tabToken[2]);


			else if( strcmp(tabToken [0],"COPB")==0) fprintf(fichier_retourbis,"%s %s %s",tabToken[0],tabToken[1],tabToken[2]);
							 
			else if( strcmp(tabToken [0],"AFC")==0)fprintf(fichier_retourbis,"%s %s %s",tabToken[0],tabToken[1],tabToken[2]);
										
			else if( strcmp(tabToken [0],"INF")==0) fprintf(fichier_retourbis,"%s %s %s %s",tabToken[0],tabToken[1],tabToken[2],tabToken[3]);

			else	if( strcmp(tabToken [0],"SUP")==0) fprintf(fichier_retourbis,"%s %s %s %s",tabToken[0],tabToken[1],tabToken[2],tabToken[3]);
			
			else if( strcmp(tabToken [0],"EQU")==0) fprintf(fichier_retourbis,"%s %s %s %s",tabToken[0],tabToken[1],tabToken[2],tabToken[3]);

			else if( strcmp(tabToken [0],"PRI")==0) fprintf(fichier_retourbis,"%s %s",tabToken[0],tabToken[1]);

			else	if( strcmp(tabToken [0],"OR")==0) fprintf(fichier_retourbis,"%s %s %s %s",tabToken[0],tabToken[1],tabToken[2],tabToken[3]);
			
			else if( strcmp(tabToken [0],"AND")==0) fprintf(fichier_retourbis,"%s %s %s %s",tabToken[0],tabToken[1],tabToken[2],tabToken[3]);

			else if( strcmp(tabToken [0],"RET\n")==0) fprintf(fichier_retourbis,"%s",tabToken[0]);

			else if( strcmp(tabToken [0],"MAIN\n")==0) fprintf(fichier_retourbis,"%s",tabToken[0]);

			else if( strcmp(tabToken [0],"PUSH")==0) fprintf(fichier_retourbis,"%s %s",tabToken[0],tabToken[1]);

			else if( strcmp(tabToken [0],"APB")==0) fprintf(fichier_retourbis,"%s %s",tabToken[0],tabToken[1]);


										

			//************************************************************************
			if(strcmp(tabToken[0], "JMP")==0 )
			{			
				fputs(tabToken[0], fichier_retourbis);						
				num= getNumeroInstruction (tabToken[1] , tab_label, nbelement_tabLabel);
				sprintf(element_ligne, " %d\n",num);	
				fputs(element_ligne, fichier_retourbis);
			
			} else if ( strcmp(tabToken[0], "JMF")==0)
			{						
				fputs(tabToken[0], fichier_retourbis);	
				fputs(" ",fichier_retourbis); 					
				fputs(tabToken[1], fichier_retourbis);	
									
				num=getNumeroInstruction(tabToken[2] ,tab_label, nbelement_tabLabel);
				sprintf(element_ligne, " %d\n",num);			
				fputs(element_ligne, fichier_retourbis);
			
			}else if ( strcmp(tabToken[0], "CALL")==0) //à finir 
			{
				fputs(tabToken[0], fichier_retourbis);
				fputs(" ",fichier_retourbis);
				
				if (strlen(tabToken[1])>14)
				{
					
					int inds =getIndiceIntableFonctions(tabToken[1] );
					sprintf(element_ligne, " %d\n",tab_fonctions[inds].adresse_implementation);			
					fputs(element_ligne, fichier_retourbis);
					
				}else
				{
					fputs(tabToken[1], fichier_retourbis);
				}
			}
			/*else
			{
				
				fprintf(fichier_retour,"%s" ,elementcourant);
				
			}*/
		
	}

}
				
			
