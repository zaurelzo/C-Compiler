#include "interpreteur.h"

int PC; //global pour le compteur courant d'instruction

 //int PC =0 ;
int nb_Instruction_Programme = 0 ;

int debutMain=0 ; // TODO mettre 0 ici , puis fixer lors de la lecture du fichier 
int ebp =0; 
int esp =0;
int savEbp=0;

int indTabDebugInfo=-1; 


int traiter_ligne(char * chaine , char ** tab_retour)
{
	int cpt = 0;
	char * token ; 
	token =strtok(chaine," ");
	 //strtok permet de decouper la chaine en tokens séparés par le delimitateur espace
	while(token!=NULL)
	{
		tab_retour[cpt]=malloc(sizeof(char)*TAILLE);
		strcpy(tab_retour[cpt],token);
		//printf("token %s\n",token );
		token =strtok(NULL," ");
		cpt++;
	}
	return cpt ; 
}

int extractNumber(char * stringTOanalyse,int * absOrRelat)
{
	char nb[30];
	if (stringTOanalyse[0]=='@' && stringTOanalyse[1]=='@')
	{
		*absOrRelat=0 ;//relative 
		strcpy(nb,&(stringTOanalyse[2]));		
	}	
	else
	{
		*absOrRelat=1 ;//absolue 
		strcpy(nb,&(stringTOanalyse[1]));	
	} 
	
	return atoi(nb);
}

void charger_Programme_dans_Ram(const char * name_fichier)
{
	FILE* fichier = NULL;
 	int codeOP, resultat , retour , op1,op2; 
    fichier = fopen(name_fichier, "r");
    char line[40];
    char * tableau_retour[5];
    int abs_or_rel ; 
 	if (fichier!=NULL)
 	{
 		//printf("fopen ===\n");
 		while(fgets(line,40,fichier)!=NULL)
 		{
 			//printf("%s",line);
 			retour = traiter_ligne(line ,(char **)tableau_retour);
 			//printf("---------------------- %d \n",retour);
 			Tableau_Ram[nb_Instruction_Programme].format_instruction=retour;
 			if (retour==4)
 			{

 				strcpy(Tableau_Ram[nb_Instruction_Programme].code_operation,tableau_retour[0]) ;
 				
 				Tableau_Ram[nb_Instruction_Programme].result= extractNumber(tableau_retour[1],&abs_or_rel);
 				Tableau_Ram[nb_Instruction_Programme].abs_relResult=abs_or_rel;

 				Tableau_Ram[nb_Instruction_Programme].operande1= extractNumber(tableau_retour[2],&abs_or_rel);
 				Tableau_Ram[nb_Instruction_Programme].abs_relOP1=abs_or_rel;
 				
 				Tableau_Ram[nb_Instruction_Programme].operande2=extractNumber(tableau_retour[3],& abs_or_rel);
 				Tableau_Ram[nb_Instruction_Programme].abs_relOP2=abs_or_rel;
 			}else if (retour==3)
 			{
 				strcpy(Tableau_Ram[nb_Instruction_Programme].code_operation,tableau_retour[0]);
 				
 				Tableau_Ram[nb_Instruction_Programme].result= extractNumber(tableau_retour[1],&abs_or_rel);
 				Tableau_Ram[nb_Instruction_Programme].abs_relResult=abs_or_rel;

 				if (strcmp(tableau_retour[0],"AFC")==0 || strcmp(tableau_retour[0],"JMF")==0 ) //AFC @RES NB
 				{
 					tableau_retour[2][strlen(tableau_retour[2])-1]='\0';
 					Tableau_Ram[nb_Instruction_Programme].operande1= atoi(tableau_retour[2]);
 				}else //COP @NB1 @NB2
 				{
 					Tableau_Ram[nb_Instruction_Programme].operande1= extractNumber(tableau_retour[2],&abs_or_rel);
 					Tableau_Ram[nb_Instruction_Programme].abs_relOP1=abs_or_rel;
 				}
 				
 			}else if (retour==2)
 			{
 				strcpy(Tableau_Ram[nb_Instruction_Programme].code_operation,tableau_retour[0]) ;

 				if (strcmp(tableau_retour[0],"PUSH")==0 || strcmp(tableau_retour[0],"PRI")==0) //PUSH @NB
 				{
 					Tableau_Ram[nb_Instruction_Programme].result= extractNumber(tableau_retour[1],&abs_or_rel);
 					Tableau_Ram[nb_Instruction_Programme].abs_relResult=abs_or_rel;
 				}else
 				{
 					tableau_retour[1][strlen(tableau_retour[1])-1]='\0';
 					Tableau_Ram[nb_Instruction_Programme].result=atoi(tableau_retour[1]);
 				}

 			}else if (retour==1)
 			{
				if(strcmp(tableau_retour[0], "MAIN\n")==0)
				{
					debutMain=nb_Instruction_Programme;
					nb_Instruction_Programme--;
				}
				else
				{
 					strcpy(Tableau_Ram[nb_Instruction_Programme].code_operation,tableau_retour[0]) ;
				}
 			}
 			nb_Instruction_Programme++;
 			/* code */
 		} 
 	}
}

void afficher_ram()
{
	int i ;
	//printf("\n\n");
	for (i = 0; i < nb_Instruction_Programme; ++i)
	{
		//printf("*****************************%d\n",nb_Instruction_Programme );
		if(Tableau_Ram[i].format_instruction==4)
		{
			printf("%s %d %d %d\n",Tableau_Ram[i].code_operation,Tableau_Ram[i].result,Tableau_Ram[i].operande1,Tableau_Ram[i].operande2);
			//printf("Result : %d | OP1 : %d | OP2 : %d\n",Tableau_Ram[i].abs_relResult,Tableau_Ram[i].abs_relOP1 ,Tableau_Ram[i].abs_relOP2);
		}else if(Tableau_Ram[i].format_instruction==3)
		{
			printf("%s %d %d\n",Tableau_Ram[i].code_operation,Tableau_Ram[i].result,Tableau_Ram[i].operande1);
			//printf("Result : %d | OP1 : %d\n",Tableau_Ram[i].abs_relResult,Tableau_Ram[i].abs_relOP1 );
		}else if(Tableau_Ram[i].format_instruction==2)
		{
			printf("%s %d\n",Tableau_Ram[i].code_operation,Tableau_Ram[i].result);
			//printf("Result : %d\n",Tableau_Ram[i].abs_relResult);
		}else if (Tableau_Ram[i].format_instruction==1)
		{
			printf("%s",Tableau_Ram[i].code_operation);
		}
	}
}


void chargerDebugInformation()
{
	FILE* fichier = NULL;
 	
    char line[40];
    char * tab_retour[5];

    int ilocal;
    int retour ;

    fichier = fopen("../compilateur/file_debug_information","r");
    
    if (fichier!=NULL)
    {
    	while(fgets(line,40,fichier)!=NULL)
 		{
 			//printf("%s",line);
 			retour = traiter_ligne(line ,(char **)tab_retour);
 			//printf("----retour ; %d\n",retour);
 			if (retour==3)
 			{
 				indTabDebugInfo++; 
 				ilocal=0;
 				strcpy(tab_DebugInfo[indTabDebugInfo].nom_fonction, tab_retour[0]);
 				tab_DebugInfo[indTabDebugInfo].nbLocales=atoi(tab_retour[2]);
 			}else if (retour==2)
 			{
 				strcpy(tab_DebugInfo[indTabDebugInfo].tab_nom_var[ilocal],tab_retour[0]);
 				tab_DebugInfo[indTabDebugInfo].adresse[ilocal]=atoi(tab_retour[1]);
 				ilocal++;
 			}
 		}
    }

    fclose(fichier);
}

void printTabDugInfo()
{
	int i,j ; 
	for ( i = 0; i <=indTabDebugInfo ; ++i)
	{
		printf("%s\n",tab_DebugInfo[i].nom_fonction);
		
		for (j = 0; j < tab_DebugInfo[i].nbLocales; ++j)
		{
			printf("%s %d\n",tab_DebugInfo[i].tab_nom_var[j],tab_DebugInfo[i].adresse[j]);
		}
		
	}
}

int  printVarLocal(int contexte,char *  name ) 
{
	int i ;
	name[strlen(name)-1]='\0';
	for (i = 0 ;i<tab_DebugInfo[contexte].nbLocales;i++)
	{
		if (strcmp(tab_DebugInfo[contexte].tab_nom_var[i],name)==0)
		{
			printf("%s %d\n",name,PILE_D_EXECUTION[calculIndice(i)].valeur); //on affiche la variable locale 
			return 0 ;
		} 
	}

	printf("ERREUR : %s n'existe pas \n",name);

	return -1  ; // si erreur 
}


int  printVarGlobale(char * name ) 
{
	int i, j ;
	//printf(" funnction call \n");
	name[strlen(name)-1]='\0';
	for (j = 0 ;j<indTabDebugInfo;j++)
	{
		if (strcmp(tab_DebugInfo[j].nom_fonction,"global")==0)
		{
			//printf("FIND \n");
			//printf("========nom correcte and nb locale %d and name %s \n",tab_DebugInfo[0].nbLocales,name);
			for (i = 0; i< tab_DebugInfo[j].nbLocales;i++)
			{
				//printf("%s",name);
				//printf("-----------length de tab : %d and name %d \n",strlen(tab_DebugInfo[j].tab_nom_var[i]),strlen(name));
				if (strcmp(tab_DebugInfo[j].tab_nom_var[i],name)==0)
				{
					printf("GLOBAL %s : %d\n",name,Tab_Mem_data[0].valeur);
					return 0 ;
				}/* else 
				{
					printf ("NULL");
				}*/
			}
		}
	}

	printf("ERREUR : la var globale %s n'existe pas \n",name);
	return -1  ; // si erreur 
}


void clean(const char *buffer, FILE *fp)
{
    char *p = strchr(buffer,'\n');
    if (p != NULL)
        *p = 0;
    else
    {
        int c;
        while ((c = fgetc(fp)) != '\n' && c != EOF);
    }
}

void printInstructionCourante(int numInst)
{
	//printf("INSTRUCTION COURANTE : %d et nb instruction :%d \n",numInst, nb_Instruction_Programme);
	if(Tableau_Ram[numInst].format_instruction==4)
		{
			printf("%s %d %d %d\n",Tableau_Ram[numInst].code_operation,Tableau_Ram[numInst].result,Tableau_Ram[numInst].operande1,Tableau_Ram[numInst].operande2);
			//printf("Result : %d | OP1 : %d | OP2 : %d\n",Tableau_Ram[i].abs_relResult,Tableau_Ram[i].abs_relOP1 ,Tableau_Ram[i].abs_relOP2);
		}else if(Tableau_Ram[numInst].format_instruction==3)
		{
			printf("%s %d %d\n",Tableau_Ram[numInst].code_operation,Tableau_Ram[numInst].result,Tableau_Ram[numInst].operande1);
			//printf("Result : %d | OP1 : %d\n",Tableau_Ram[i].abs_relResult,Tableau_Ram[i].abs_relOP1 );
		}else if(Tableau_Ram[numInst].format_instruction==2)
		{
			//printf("HERE============\n");
			printf("%s %d\n",Tableau_Ram[numInst].code_operation,Tableau_Ram[numInst].result);
			//printf("Result : %d\n",Tableau_Ram[i].abs_relResult);
		}else if (Tableau_Ram[numInst].format_instruction==1)
		{
			printf("%s\n",Tableau_Ram[numInst].code_operation);
		}
}


int calculIndice(int adresse)
{
	//printf("pc %d | debutMain%d\n",PC ,debutMain );
	if (debutMain>PC)
	{
		//printf("============ save ebp : %d ebp %d\n",savEbp,ebp);
		int ecart =savEbp-ebp; 
		//if (PC==0) printf("==============adresse %d et ecart %d\n",adresse,ecart );
		int newaddr=ebp + adresse ;	
		
			if (adresse>=ecart)
				newaddr=ebp + adresse+2;

		//if (PC==0) printf(" @@@@@@@@@@@@@@@@@ newaddr : %d\n",newaddr );
		return newaddr;
	} else 
	{
		return adresse;
	}
}

void InitiPileExecution()
{
	int i; 
	for ( i = 0; i < TAILLE_PILE_D_EXECUTION; ++i)
	{
		PILE_D_EXECUTION[i].addrRetour=0 ;
		PILE_D_EXECUTION[i].ebp=0;
	}
}

void debug(int *pc,int pc_value_if_debug,int contextCourant)
{	
	char * tab_retour[5];
	char inputUser[40] ;//= "d";
	char  copieInputUser[40];
	int fini = 0 ;
	strcpy(inputUser,"d");
	

	while(fini==0)
	{
		fgets(inputUser,sizeof(inputUser), stdin);
		strcpy(copieInputUser,inputUser);
		clean(inputUser,stdin);
		if (strcmp(copieInputUser,"\n")!=0)
		{
			traiter_ligne(copieInputUser,tab_retour);
			
			
			if (strcmp(tab_retour[0],"printvar")==0)
			{
				 printVarLocal(contextCourant,tab_retour[1]);
			}else if (strcmp(tab_retour[0],"printgl")==0)
			{
				//printf("VARIABLE GLOBAL À CHERCHER %s",tab_retour[1]);
				printVarGlobale(tab_retour[1]);
			} //on passera automatique à l'instruction suivante si on a pas ce format à l'entrée
		}else 
		{
			fini=1;
		}	
	}	
	*pc=pc_value_if_debug;

	//printf("leave loop =======\n");
}


//TODO
//void printPileContextCOurant();

void interpreter(int modeDebug)
{
	InitiPileExecution();
	int resRes,resOp1 ,resOp2;
	int contextCourant =indTabDebugInfo ; //on est dans le context du main

	int saveContextWhenCall=0;


	//printf("Context Courant %d",contextCourant);
	

	char inputUser[40] = "";

	int newEbp_for_call_context=0;
	
	 PC= debutMain;

	if (modeDebug)
	{
		printf("=================	DEBUG MODE =====================\n");
		printf("COMMANDE : printvar localVar | printgl GlobalVar | entrée\n");
	}/*else 
	{
		printf("VALEUR DU MAIN :%d | NOMBRE INSTRUCTION :%d\n",PC ,nb_Instruction_Programme );
	}*/
	while(PC < nb_Instruction_Programme)
	{	
		/*if (modeDebug)
		{
			printInstructionCourante(PC);
		}*/
		

		//TODO : RAJOUTER UNE CONDITION POUR L'OVERFLOW DE LA PILE 

		if (modeDebug)
		{
			printInstructionCourante(PC);//Instruction en cours d'éxécution 
		}


		if( strcmp (Tableau_Ram[PC].code_operation,"ADD")==0)//ADD
		{
			if (Tableau_Ram[PC].abs_relOP2==0) //l'opérande 2 est à adresse absolue
			{
				resOp2= PILE_D_EXECUTION[ calculIndice(Tableau_Ram[PC].operande2)].valeur ;
			}else //l'opérande 2 est à adresse  relatif
			{
				resOp2=Tab_Mem_data[Tableau_Ram[PC].operande2].valeur ;
			}

			if (Tableau_Ram[PC].abs_relOP1==0) //l'opérande 1 est à adresse absolue
			{
				resOp1= PILE_D_EXECUTION[ calculIndice(Tableau_Ram[PC].operande1)].valeur ;
			}else //l'opérande 1 est à adresse  relatif
			{
				resOp1=Tab_Mem_data[Tableau_Ram[PC].operande1].valeur ;
			}

			if (Tableau_Ram[PC].abs_relResult==0) //resultat est à adresse absolue
			{
				PILE_D_EXECUTION[calculIndice(Tableau_Ram[PC].result)].valeur= resOp2+resOp1 ;
			}else //lresultat est à adresse  relatif
			{
				Tab_Mem_data[Tableau_Ram[PC].result].valeur=resOp2+resOp1;
			}

			if(modeDebug==0)
			{
				++PC;
			}else 
			{
				debug(&PC,++PC,contextCourant);
			}
			//++PC;


		} else if( strcmp (Tableau_Ram[PC].code_operation,"MUL")==0)//MUL
		{
			
			if (Tableau_Ram[PC].abs_relOP2==0) //l'opérande 2 est à adresse absolue
			{
				resOp2= PILE_D_EXECUTION[ calculIndice(Tableau_Ram[PC].operande2)].valeur ;
			}else //l'opérande 2 est à adresse  relatif
			{
				resOp2=Tab_Mem_data[Tableau_Ram[PC].operande2].valeur ;
			}

			if (Tableau_Ram[PC].abs_relOP1==0) //l'opérande 1 est à adresse absolue
			{
				resOp1= PILE_D_EXECUTION[ calculIndice(Tableau_Ram[PC].operande1)].valeur ;
			}else //l'opérande 1 est à adresse  relatif
			{
				resOp1=Tab_Mem_data[Tableau_Ram[PC].operande1].valeur ;
			}

			if (Tableau_Ram[PC].abs_relResult==0) //resultat est à adresse absolue
			{
				PILE_D_EXECUTION[calculIndice(Tableau_Ram[PC].result)].valeur= resOp2+resOp1 ;
			}else //lresultat est à adresse  relatif
			{
				Tab_Mem_data[Tableau_Ram[PC].result].valeur=resOp2*resOp1;
			}



			if(modeDebug==0)
			{
				++PC;
			}else 
			{
				debug(&PC,++PC,contextCourant);
			}
			//++PC;


		} else if( strcmp (Tableau_Ram[PC].code_operation,"SOU")==0)//SOU
		{
			
			if (Tableau_Ram[PC].abs_relOP2==0) //l'opérande 2 est à adresse absolue
			{
				resOp2= PILE_D_EXECUTION[ calculIndice(Tableau_Ram[PC].operande2)].valeur ;
			}else //l'opérande 2 est à adresse  relatif
			{
				resOp2=Tab_Mem_data[Tableau_Ram[PC].operande2].valeur ;
			}

			if (Tableau_Ram[PC].abs_relOP1==0) //l'opérande 1 est à adresse absolue
			{
				resOp1= PILE_D_EXECUTION[ calculIndice(Tableau_Ram[PC].operande1)].valeur ;
			}else //l'opérande 1 est à adresse  relatif
			{
				resOp1=Tab_Mem_data[Tableau_Ram[PC].operande1].valeur ;
			}

			if (Tableau_Ram[PC].abs_relResult==0) //resultat est à adresse absolue
			{
				PILE_D_EXECUTION[calculIndice(Tableau_Ram[PC].result)].valeur= resOp2+resOp1 ;
			}else //lresultat est à adresse  relatif
			{
				Tab_Mem_data[Tableau_Ram[PC].result].valeur=resOp1- resOp2;
			}


			if(modeDebug==0)
			{
				++PC;
			}else 
			{
				debug(&PC,++PC,contextCourant);
			}
			//++PC;


		} else if( strcmp (Tableau_Ram[PC].code_operation,"DIV")==0)//DIV
		{
			
			if (Tableau_Ram[PC].abs_relOP2==0) //l'opérande 2 est à adresse absolue
			{
				resOp2= PILE_D_EXECUTION[ calculIndice(Tableau_Ram[PC].operande2)].valeur ;
			}else //l'opérande 2 est à adresse  relatif
			{
				resOp2=Tab_Mem_data[Tableau_Ram[PC].operande2].valeur ;
			}

			if (Tableau_Ram[PC].abs_relOP1==0) //l'opérande 1 est à adresse absolue
			{
				resOp1= PILE_D_EXECUTION[ calculIndice(Tableau_Ram[PC].operande1)].valeur ;
			}else //l'opérande 1 est à adresse  relatif
			{
				resOp1=Tab_Mem_data[Tableau_Ram[PC].operande1].valeur ;
			}

			if (Tableau_Ram[PC].abs_relResult==0) //resultat est à adresse absolue
			{
				PILE_D_EXECUTION[calculIndice(Tableau_Ram[PC].result)].valeur= resOp2+resOp1 ;
			}else //lresultat est à adresse  relatif
			{
				Tab_Mem_data[Tableau_Ram[PC].result].valeur=resOp1/resOp2;
			}



			if(modeDebug==0)
			{
				++PC;
			}else 
			{
				debug(&PC,++PC,contextCourant);
			}
			//++PC;


		} else if( strcmp (Tableau_Ram[PC].code_operation,"COP")==0)////COPIE
		{
			if (Tableau_Ram[PC].abs_relOP1==0) //l'opérande 1 est à adresse absolue
			{
				resOp1= PILE_D_EXECUTION[ calculIndice(Tableau_Ram[PC].operande1)].valeur ;
			}else //l'opérande 1 est à adresse  relatif
			{
				resOp1=Tab_Mem_data[Tableau_Ram[PC].operande1].valeur ;
			}

			if (Tableau_Ram[PC].abs_relResult==0) //resultat est à adresse absolue
			{
				PILE_D_EXECUTION[calculIndice(Tableau_Ram[PC].result)].valeur= resOp1 ;
			}else //lresultat est à adresse  relatif
			{
				Tab_Mem_data[Tableau_Ram[PC].result].valeur=resOp1;
			}

			if(modeDebug==0)
			{
				++PC;
			}else 
			{
				debug(&PC,++PC,contextCourant);
			}



		} else if( strcmp (Tableau_Ram[PC].code_operation,"COPA")==0)//// COPA
		{
			if (Tableau_Ram[PC].abs_relOP1==0) //l'opérande 1 est à adresse absolue
			{
				printf("ERREUR \n");
				exit(1);
				//resOp1= PILE_D_EXECUTION[ calculIndice(Tableau_Ram[PC].operande1)].valeur ;
			}else //l'opérande 1 est à adresse  relatif
			{
				//printf("here\n");
				resOp1=PILE_D_EXECUTION[ calculIndice(Tab_Mem_data[ Tableau_Ram[PC].operande1].valeur )].valeur ;
			}

			if (Tableau_Ram[PC].abs_relResult==0) //resultat est à adresse absolue
			{
				//printf("EREUUR \n");
				exit(1);
				//PILE_D_EXECUTION[calculIndice(Tableau_Ram[PC].result)].valeur= resOp1 ;
			}else //lresultat est à adresse  relatif
			{
				//printf("----------------------\n");
				Tab_Mem_data[Tableau_Ram[PC].result].valeur=resOp1;
			}

			if(modeDebug==0)
			{
				++PC;
			}else 
			{
				debug(&PC,++PC,contextCourant);
			}

		}else if( strcmp (Tableau_Ram[PC].code_operation,"COPB")==0)//// COPB
		{
			if (Tableau_Ram[PC].abs_relOP1==0) //l'opérande 1 est à adresse absolue
			{
				resOp1=PILE_D_EXECUTION[ calculIndice(Tableau_Ram[PC].operande1)].valeur ;
				//resOp1= PILE_D_EXECUTION[ calculIndice(Tableau_Ram[PC].operande1)].valeur ;
			}else //l'opérande 1 est à adresse  relatif
			{
				//printf("here\n");
				resOp1=Tab_Mem_data[Tableau_Ram[PC].operande1].valeur;
			}

			if (Tableau_Ram[PC].abs_relResult==0) //resultat est à adresse absolue
			{
				int ad = PILE_D_EXECUTION[calculIndice(Tableau_Ram[PC].result)].valeur;
				PILE_D_EXECUTION[  calculIndice(ad)].valeur= resOp1 ;
			}else //lresultat est à adresse  relatif
			{
				int ad = Tab_Mem_data[Tableau_Ram[PC].result].valeur ;
				PILE_D_EXECUTION[ calculIndice(ad)].valeur= resOp1 ;
			}

			if(modeDebug==0)
			{
				++PC;
			}else 
			{
				debug(&PC,++PC,contextCourant);
			}

			//++PC;
		} else if( strcmp (Tableau_Ram[PC].code_operation,"AFC")==0)//AFC
		{
			//printf("+++++++++++IN AFC\n");
			if (Tableau_Ram[PC].abs_relResult==0) //resultat est à adresse absolue
			{
				//printf("-----afc absolue\n");
				PILE_D_EXECUTION[calculIndice(Tableau_Ram[PC].result)].valeur= Tableau_Ram[PC].operande1 ;
			}else //lresultat est à adresse  relatif
			{
				//printf("-------afc relatif \n");
				Tab_Mem_data[Tableau_Ram[PC].result].valeur=Tableau_Ram[PC].operande1;
			}

			//Tab_Mem_data[Tableau_Ram[PC].result].valeur=Tableau_Ram[PC].operande1;


			if(modeDebug==0)
			{
				++PC;
				//printf("AFC DONE ==========\n");
			}else 
			{
				debug(&PC,++PC,contextCourant);
			}
			//++PC;


		} else if( strcmp (Tableau_Ram[PC].code_operation,"JMP")==0)//SAUT INCONDITIONNEL 
		{


			if(modeDebug==0)
			{
				PC=Tableau_Ram[PC].result;
			}else 
			{
				debug(&PC,Tableau_Ram[PC].result,contextCourant);
			}
			//PC=Tableau_Ram[PC].result;

		} else if( strcmp (Tableau_Ram[PC].code_operation,"JMF")==0)//saut si la condition est fausse  
		{
			if (Tableau_Ram[PC].abs_relResult==0) //resultat est à adresse absolue
			{
				resRes= PILE_D_EXECUTION[ calculIndice(Tableau_Ram[PC].result)].valeur;
			}else //lresultat est à adresse  relatif
			{
				resRes=   Tab_Mem_data[Tableau_Ram[PC].result].valeur;
			}

			if (resRes==0)
			{
					//printf("==========////%d\n", Tableau_Ram[PC].operande1);
				if(modeDebug==0)
				{
					//printf("===IN  JMF ;%d\n", Tableau_Ram[PC].operande1);
					PC =Tableau_Ram[PC].operande1;
				}else 
				{
					debug(&PC,Tableau_Ram[PC].operande1,contextCourant);
				}

				//PC =Tableau_Ram[PC].operande1;
			}else 
			{
				if(modeDebug==0)
				{
					++PC;
				}else 
				{
					debug(&PC,++PC,contextCourant);
				}
				//++PC;
			}

		} else if( strcmp (Tableau_Ram[PC].code_operation,"INF")==0)//Inferieur //TODO changer la fonction asm qui génére le inf pour inclure le fait que les expressions à comparer peuvent etre absolue ou relative 
		{
			if (Tableau_Ram[PC].abs_relOP2==0) //l'opérande 2 est à adresse absolue
			{
				resOp2= PILE_D_EXECUTION[ calculIndice(Tableau_Ram[PC].operande2)].valeur ;
			}else //l'opérande 2 est à adresse  relatif
			{
				resOp2=Tab_Mem_data[Tableau_Ram[PC].operande2].valeur ;
			}

			if (Tableau_Ram[PC].abs_relOP1==0) //l'opérande 1 est à adresse absolue
			{
				resOp1= PILE_D_EXECUTION[ calculIndice(Tableau_Ram[PC].operande1)].valeur ;
			}else //l'opérande 1 est à adresse  relatif
			{
				resOp1=Tab_Mem_data[Tableau_Ram[PC].operande1].valeur ;
			}


			int reslComp;

			if (resOp1<resOp2)
				reslComp=1;
			else 
				reslComp=0 ;

			if (Tableau_Ram[PC].abs_relResult==0) //resultat est à adresse absolue
			{
				PILE_D_EXECUTION[calculIndice(Tableau_Ram[PC].result)].valeur= reslComp;

			}else //lresultat est à adresse  relatif
			{
				Tab_Mem_data[Tableau_Ram[PC].result].valeur=reslComp;
				
			}


				if(modeDebug==0)
				{
					++PC;
				}else 
				{
					debug(&PC,++PC,contextCourant);
				}

			/*if (Tab_Mem_data[Tableau_Ram[PC].operande1].valeur<Tab_Mem_data[Tableau_Ram[PC].operande2].valeur)
				Tab_Mem_data[Tableau_Ram[PC].result].valeur=1;
			else 
				Tab_Mem_data[Tableau_Ram[PC].result].valeur=0 ;*/
			//++PC;
		} else if( strcmp (Tableau_Ram[PC].code_operation,"SUP")==0)//Inferieur //TODO changer la fonction asm qui génére le inf pour inclure le fait que les expressions à comparer peuvent etre absolue ou relative 
		{
			if (Tableau_Ram[PC].abs_relOP2==0) //l'opérande 2 est à adresse absolue
			{
				resOp2= PILE_D_EXECUTION[ calculIndice(Tableau_Ram[PC].operande2)].valeur ;
			}else //l'opérande 2 est à adresse  relatif
			{
				resOp2=Tab_Mem_data[Tableau_Ram[PC].operande2].valeur ;
			}

			if (Tableau_Ram[PC].abs_relOP1==0) //l'opérande 1 est à adresse absolue
			{
				resOp1= PILE_D_EXECUTION[ calculIndice(Tableau_Ram[PC].operande1)].valeur ;
			}else //l'opérande 1 est à adresse  relatif
			{
				resOp1=Tab_Mem_data[Tableau_Ram[PC].operande1].valeur ;
			}


			int reslComp;

			if (resOp1>resOp2)
				reslComp=1;
			else 
				reslComp=0 ;

			if (Tableau_Ram[PC].abs_relResult==0) //resultat est à adresse absolue
			{
				PILE_D_EXECUTION[calculIndice(Tableau_Ram[PC].result)].valeur= reslComp;

			}else //lresultat est à adresse  relatif
			{
				Tab_Mem_data[Tableau_Ram[PC].result].valeur=reslComp;
				
			}


				if(modeDebug==0)
				{
					++PC;
				}else 
				{
					debug(&PC,++PC,contextCourant);
				}

			/*if (Tab_Mem_data[Tableau_Ram[PC].operande1].valeur<Tab_Mem_data[Tableau_Ram[PC].operande2].valeur)
				Tab_Mem_data[Tableau_Ram[PC].result].valeur=1;
			else 
				Tab_Mem_data[Tableau_Ram[PC].result].valeur=0 ;*/
			//++PC;
		}else if( strcmp (Tableau_Ram[PC].code_operation,"EQU")==0)//Inferieur //TODO changer la fonction asm qui génére le inf pour inclure le fait que les expressions à comparer peuvent etre absolue ou relative 
		{
			if (Tableau_Ram[PC].abs_relOP2==0) //l'opérande 2 est à adresse absolue
			{
				resOp2= PILE_D_EXECUTION[ calculIndice(Tableau_Ram[PC].operande2)].valeur ;
			}else //l'opérande 2 est à adresse  relatif
			{
				resOp2=Tab_Mem_data[Tableau_Ram[PC].operande2].valeur ;
			}

			if (Tableau_Ram[PC].abs_relOP1==0) //l'opérande 1 est à adresse absolue
			{
				resOp1= PILE_D_EXECUTION[ calculIndice(Tableau_Ram[PC].operande1)].valeur ;
			}else //l'opérande 1 est à adresse  relatif
			{
				resOp1=Tab_Mem_data[Tableau_Ram[PC].operande1].valeur ;
			}


			int reslComp;

			if (resOp1==resOp2)
				reslComp=1;
			else 
				reslComp=0 ;

			if (Tableau_Ram[PC].abs_relResult==0) //resultat est à adresse absolue
			{
				PILE_D_EXECUTION[calculIndice(Tableau_Ram[PC].result)].valeur= reslComp;
				
			}else //lresultat est à adresse  relatif
			{
				Tab_Mem_data[Tableau_Ram[PC].result].valeur=reslComp;
				

			}


				if(modeDebug==0)
				{
					++PC;
				}else 
				{
					debug(&PC,++PC,contextCourant);
				}
			//++PC;


		}else  if( strcmp (Tableau_Ram[PC].code_operation,"PRI")==0) //Print 
		{
			if (Tableau_Ram[PC].abs_relResult==0) //resultat est à adresse RELATIVE
			{
				//printf("PC : %d relative  ========== à indice %d \n",PC,Tableau_Ram[PC].result);

				resRes=PILE_D_EXECUTION[calculIndice(Tableau_Ram[PC].result)].valeur;
				
			}else //lresultat est à adresse  relatif
			{
				//printf("PC %d and absoluee -------- à indice %d\n",PC,Tableau_Ram[PC].result);
				resRes=Tab_Mem_data[Tableau_Ram[PC].result].valeur;
			}
			printf("%d\n",resRes);

				if(modeDebug==0)
				{
					++PC;
				}else 
				{
					debug(&PC,++PC,contextCourant);
				}
			//++PC;


		}else  if( strcmp (Tableau_Ram[PC].code_operation,"APB")==0) //APB
		{
			//todo rajouter +2 quand on a appelé une nouvelle fonction
			esp  = esp + Tableau_Ram[PC].result; //avant le push
			//printf("NEW CALL CONTEXT DU MAIN %d \n",esp );
			if(PC>debutMain)
				newEbp_for_call_context=esp;
			else
					newEbp_for_call_context=esp+2;

			if(modeDebug==0)
			{
				++PC;
			}else 
			{
				debug(&PC,++PC,contextCourant);
			}



		} else  if( strcmp (Tableau_Ram[PC].code_operation,"PUSH")==0) //PUSH
		{
			//if (boolFirtPush==1)

			if (Tableau_Ram[PC].abs_relResult==0) //resultat est à adresse absolue
			{
				resRes=PILE_D_EXECUTION[calculIndice(Tableau_Ram[PC].result)].valeur;
			}else //lresultat est à adresse  relatif
			{
				resRes=Tab_Mem_data[Tableau_Ram[PC].result].valeur;
			}

			
			PILE_D_EXECUTION[esp].valeur=resRes; //on push le resultat
			
		
			esp++;

			if(modeDebug==0)
			{
				++PC;
			}else 
			{
				debug(&PC,++PC,contextCourant);
			}
			//++PC;

		} else  if( strcmp (Tableau_Ram[PC].code_operation,"CALL")==0) //TODO CHANGER DE CONTEXT POUR LE DEBUG
		{
			PILE_D_EXECUTION[esp].valeur=PC+1;//on empile la valeur retour
			PILE_D_EXECUTION[esp].addrRetour=1; 
			
			savEbp=esp; //on sauvegard l'endroit on se trouve l'adresse Retour
			esp++;
			

			PILE_D_EXECUTION[esp].valeur=ebp; //on empile ebp
			PILE_D_EXECUTION[esp].ebp=1;//on save oldEbp
			esp++;
			
			ebp=newEbp_for_call_context ;
			//PC=Tableau_Ram[PC].result;//on saute à la fonction

			saveContextWhenCall =contextCourant; 
			contextCourant = Tableau_Ram[PC].result ;
			if(modeDebug==0)
			{
				PC=Tableau_Ram[PC].result;//on saute à la fonction
			}else 
			{
				debug(&PC,Tableau_Ram[PC].result,contextCourant);
			}
			
		} else  if( strcmp (Tableau_Ram[PC].code_operation,"RET\n")==0) //on restaure le contexte 
		{
			//printf("IN RETURN \n");
			//printf("current context : ebp %d | esp %d and save ebp %d and indice save ebp %d  \n" ,ebp, esp, PILE_D_EXECUTION[savEbp+1].valeur ,savEbp+1);
			esp=ebp;
			//PC=PILE_D_EXECUTION[savEbp].valeur ;
			ebp= PILE_D_EXECUTION[savEbp+1].valeur ;
			contextCourant =saveContextWhenCall ;
			if(modeDebug==0)
			{
				PC=PILE_D_EXECUTION[savEbp].valeur ;
			}else 
			{
				debug(&PC,PILE_D_EXECUTION[savEbp].valeur ,contextCourant);
			}
		} 

	}
}
