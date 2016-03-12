#include "deuxieme_passe.h"

/*int  traiter_ligne(char * line, char * * l )
{
	
	char * token ;
	int cp   
	//printf("Valuer de token 1 :"); 			
	token =strtok(line," ");
	//printf("Valuer de token :  %s", token); 			
	while(token!=NULL)			
	{
		strcpy(l.nomLabel, token);
		token=strtok(NULL," ");			
		l.num_instruction=atoi(token);
	} 
	
}*/


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

void remplacer_label(char * nom_fichier_ass, Noeud_label * tab_label, int nbelement_tabLabel)
{
	int i, num,retour,k;	
	FILE * fichier=NULL;
	FILE * fichier_retour=NULL;
	FILE * fichier_retourbis=NULL;
	
	char line[40];
	char elementcourant[40];
	char  element_ligne[TAILLE];
	char * tabToken[5];

	fichier=fopen(nom_fichier_ass, "r");
	fichier_retour=fopen("fichier_ass_correct", "w+");
	fichier_retourbis = fopen("fichier_asm_binaire", "w+");

	if(nom_fichier_ass!=NULL)
	{
		while(fgets(line, 40, fichier)!=NULL)
		{
				strcpy(elementcourant,line) ; 
				retour = traiter_ligne_du_fichier(line,tabToken);

				/************************ecriture du fichier binaire *********************/

				if( strcmp(tabToken [0],"ADD") ==0) fprintf(fichier_retourbis,"1 %s %s %s",tabToken[1],tabToken[2],tabToken[3]);
								
					else if( strcmp(tabToken [0],"MUL")==0) fprintf(fichier_retourbis,"2 %s %s %s",tabToken[1],tabToken[2],tabToken[3]);
								

					else if( strcmp(tabToken [0],"SOU")==0) fprintf(fichier_retourbis,"3 %s %s %s",tabToken[1],tabToken[2],tabToken[3]);
								
					else if( strcmp(tabToken [0],"DIV")==0) fprintf(fichier_retourbis,"4 %s %s %s",tabToken[1],tabToken[2],tabToken[3]);
								
					else if( strcmp(tabToken [0],"COP")==0) fprintf(fichier_retourbis,"5 %s %s\n",tabToken[1],tabToken[2]);
								 
					else if( strcmp(tabToken [0],"AFC")==0)fprintf(fichier_retourbis,"6 %s %s",tabToken[1],tabToken[2]);
								

					
				else 	if( strcmp(tabToken [0],"INF")==0) fprintf(fichier_retourbis,"9 %s %s %s",tabToken[1],tabToken[2],tabToken[3]);

				else	if( strcmp(tabToken [0],"SUP")==0) fprintf(fichier_retourbis,"10 %s %s %s",tabToken[1],tabToken[2],tabToken[3]);
				else if( strcmp(tabToken [0],"EQU")==0) fprintf(fichier_retourbis,"11 %s %s %s",tabToken[1],tabToken[2],tabToken[3]);

				else if( strcmp(tabToken [0],"PRI")==0) fprintf(fichier_retourbis,"12 %s",tabToken[1]);

				else	if( strcmp(tabToken [0],"OR")==0) fprintf(fichier_retourbis,"13 %s %s %s",tabToken[1],tabToken[2],tabToken[3]);
				else if( strcmp(tabToken [0],"AND")==0) fprintf(fichier_retourbis,"14 %s %s %s",tabToken[1],tabToken[2],tabToken[3]);
								
				

				/************************************************************************/


				//printf("////////////////////////////%s : %d \n", line,retour );
				//Dans le cas où la ligne commence par un JMF ou JMP				
				if(strcmp(tabToken[0], "JMP")==0 )
				{
					/*for (k = 0; k < retour; ++k)
										{
											printf("+++++++++++++++%s",tabToken[k]);
										}*/					
					fputs(tabToken[0], fichier_retour);	
										
					num= getNumeroInstruction (tabToken[1] , tab_label, nbelement_tabLabel);
					sprintf(element_ligne, " %d\n",num);
						
					fputs(element_ligne, fichier_retour);

					fprintf(fichier_retourbis,"7 %s",element_ligne);
				
				}
				else if ( strcmp(tabToken[0], "JMF")==0)
				{
					/*for (k = 0; k < retour; ++k)
										{
											printf("+++++++++++++++%s",tabToken[k]);
										}*/
										
					fputs(tabToken[0], fichier_retour);	
					fputs(" ",fichier_retour); 

										
					fputs(tabToken[1], fichier_retour);	
										
					num=getNumeroInstruction(tabToken[2] ,tab_label, nbelement_tabLabel);
					sprintf(element_ligne, " %d\n",num);
					
				
					fputs(element_ligne, fichier_retour);
					fprintf(fichier_retourbis,"8 %s %s",tabToken[1],element_ligne);
				
				}
				else
				{
					
					fprintf(fichier_retour,"%s" ,elementcourant);
					//fputs("\n", fichier_retour);
				}
			
		}
	}

}
				
			
