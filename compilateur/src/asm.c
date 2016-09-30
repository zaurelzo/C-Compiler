#include "../head/asm.h"
#include "../head/tab_symb.h"
#include "../head/tab_label.h"



int declaration_asm(char * nom_var,int type_de_la_var) 
{
	
	if ( ajouter_Var(nom_var,type_de_la_var,0,0)==-1)  
	{
		return -1 ;
	} else 
	{
		/*if(strcmp(getMode(),"fonction")==0)
		{
			IncrementeNbVariableLocalDeClare() ;
		}*/
		return 0 ;
	}
}

int declaration_affectation_asm( char * nom_var,int constante ,int dollar_relative_ou_absolue , int dollar_addr,int type_de_la_var)
{
	//printf("==========adresse %d %d \n", dollar_addr,dollar_relative_ou_absolue);
	if (ajouter_Var(nom_var,type_de_la_var,0,constante )==-1)
	{
		return -1 ; //pas reussi 
	}else 
	{
		int abs_rel ;
		int typage_var;
		int var = recherchet(nom_var,&abs_rel,&typage_var); 
		int decGlob= getDeclaGlobale();
		/*if (strcmp(getMode(),"fonction")==0) 
		{
			//printf("------------%d\n",getTailleTypeRetourFonction());	
			var += getTailleTypeRetourFonction() ; //si on un type retour
			
		}*/
		if(decGlob==0){
			//printf("++++++++++++++++++\n");
			if (dollar_relative_ou_absolue==0 && abs_rel==0)
			{	
				/*if (strcmp(getMode(),"fonction")==0)
				{
					dollar_addr +=getTailleTypeRetourFonction() ;
				}*/
				printf( "COP @@%d @@%d\n",var,dollar_addr);

			} else if  (dollar_relative_ou_absolue==0 && abs_rel==1)
			{
				/*if (strcmp(getMode(),"fonction")==0)
				{
					dollar_addr +=getTailleTypeRetourFonction() ;
				}*/
				printf( "COP @%d @@%d\n",var,dollar_addr);

			} else if  (dollar_relative_ou_absolue==1 && abs_rel==0)
			{
			
				printf( "COP @@%d @%d\n",var,dollar_addr);
			} else if  (dollar_relative_ou_absolue==1 && abs_rel==1)
			{
				//printf("==================\n");
				printf( "COP @%d @%d\n",var,dollar_addr);
			}
		}else
		{
			//printf("dfdfdfdeff\n");
			printf( "COP @%d @%d\n",var,dollar_addr);
			//reinitDeclaGlobale();
		}
		incrementerPC();
		viderPile();
		if(dollar_addr!=NULL_2)
		{
			modifierChampInitialiserVariable(nom_var);	
		}

	return 0 ;
	}
}

int affection_asm( char * nom_var ,int dollar_relative_ou_absolue , int dollar_addr) 
{

	int var ;
	int abs_rel ; 
	int typage_var;												
	if ((  var =recherchet(nom_var,&abs_rel,&typage_var))==-1)
	{
		return -1 ;
	}else 
	{
		/*if(strcmp(getMode(),"fonction")==0)
		{
			var += getTailleTypeRetourFonction() ; //si on un type retour
		}*/

		if (dollar_relative_ou_absolue==0 && abs_rel==0)
		{
			/*if (strcmp(getMode(),"fonction")==0)
			{
				dollar_addr +=getTailleTypeRetourFonction() ;
			}*/
			printf( "COP @@%d @@%d\n",var,dollar_addr);
		}else if  (dollar_relative_ou_absolue==0 && abs_rel==1)
		{	
			/*if (strcmp(getMode(),"fonction")==0)
			{
				dollar_addr +=getTailleTypeRetourFonction() ;
			}*/
			printf( "COP @%d @@%d\n",var,dollar_addr);
		}else if  (dollar_relative_ou_absolue==1 && abs_rel==0)
		{
			printf( "COP @@%d @%d\n",var,dollar_addr);
		}else if  (dollar_relative_ou_absolue==1 && abs_rel==1)
		{
			printf( "COP @%d @%d\n",var,dollar_addr);
		}																											
		incrementerPC();
		viderPile();
		if(dollar_addr!=NULL_2)
		{
			modifierChampInitialiserVariable(nom_var);	
		}								
	}
	return 0 ;
}

void print_asm( int dollar_addr , int dollar_relative_ou_absolue )
{
	if(dollar_relative_ou_absolue==0)
		printf("PRI @@%d\n", dollar_addr );
	else 
		printf("PRI @%d\n", dollar_addr );
																							
	incrementerPC();
	viderPile();
}

int operation_arithmetique_asm(char * operation , int * dollar_addr, int * dollar_relative_ou_absolue,int *  dollar_typage_result )
{
	int typeOp1,typeOp2; 
	int  abs_rel1,abs_rel2;
	int typage_Op1 , typage_Op2;

	int valeurOp2 = depilert(&typeOp2,&abs_rel2,&typage_Op2);
	int valeurOp1= depilert(&typeOp1,&abs_rel1,&typage_Op1);
	//printf("======typage_Op1 : %d | typage_Op2 : %d\n",typage_Op1,typage_Op2);

	
		if (typage_Op2 != typage_Op1) //si les types des opérandes sont différents
		{
			return -2;
		}
		if(typage_Op1>1 && typage_Op2>1)
		{
			return -3;
		}
		if (valeurOp2==-1 || valeurOp1==-1 )
		{
			//printf("ERREUR indicePile :%d\n",indPile );
			return -1 ; // erreur depilement des opérandes 
		}else 
		{
			*dollar_addr= getAdressePile();
			incrementerPC();
			*dollar_relative_ou_absolue=1;
			*dollar_typage_result = typage_Op1; 
		
			if (typeOp1==1 && typeOp2==1)//si 1er et 2iem opérande est une variable 
			{

				if (abs_rel1==0 && abs_rel2==0)
				{
					printf("%s @%d @@%d @@%d\n",operation,getAdressePile(),valeurOp2,valeurOp1);
				}else if (abs_rel1==0 && abs_rel2==1)
				{
					printf("%s @%d @%d @@%d\n",operation,getAdressePile(),valeurOp2,valeurOp1); 	
				}else if (abs_rel1==1 && abs_rel2==0)
				{
					printf("%s @%d @@%d @%d\n",operation,getAdressePile(),valeurOp2,valeurOp1); 	
				}else if (abs_rel1==1 && abs_rel2==1)
				{
					printf("%s @%d @%d @%d\n",operation,getAdressePile(),valeurOp2,valeurOp1); 	
				}

				empilert(getAdressePile(),1,1,typage_Op1) ;
			
			}else if( typeOp1==1 && typeOp2==0) //si 1er est une var et 2iem est une constante
			{
				//printf("++++++++++++++++++++++++ %d %d \n",abs_rel1,abs_rel2);
				int NouvAdrOp2= obtenirAdressDeuxiemeOperande();
				if (abs_rel1==0 && abs_rel2==0)
					printf("%s @%d @@%d @@%d\n",operation,getAdressePile(),valeurOp1,NouvAdrOp2);
				else if (abs_rel1==0 && abs_rel2==1)
					printf("%s @%d @@%d @%d\n",operation,getAdressePile(),valeurOp1,NouvAdrOp2);
				else if (abs_rel1==1 && abs_rel2==0)
					printf("%s @%d @%d @@%d\n",operation,getAdressePile(),valeurOp1,NouvAdrOp2);
				else if (abs_rel1==1 && abs_rel2==1)
					printf("%s @%d @%d @%d\n",operation,getAdressePile(),valeurOp1,NouvAdrOp2);
		
				empilert(getAdressePile(),1,1,typage_Op1) ;
			}else if( typeOp1==0 && typeOp2==1) //si 1er est une constante et 2iem est une var
			{
			
				int NouvAdrOp1=obtenirAdressePremierOperande(); 
			
				if (abs_rel1==0 && abs_rel2==0)
					printf("%s @%d @@%d @@%d\n",operation,getAdressePile(),NouvAdrOp1,valeurOp2);
				if (abs_rel1==0 && abs_rel2==1)
					printf("%s @%d @@%d @%d\n",operation,getAdressePile(),NouvAdrOp1,valeurOp2);
				if (abs_rel1==1 && abs_rel2==0)
					printf("%s @%d @%d @@%d\n",operation,getAdressePile(),NouvAdrOp1,valeurOp2);
				if (abs_rel1==1 && abs_rel2==1)
					printf("%s @%d @%d @%d\n",operation,getAdressePile(),NouvAdrOp1,valeurOp2);
				
				empilert(getAdressePile(),1,1,typage_Op1) ;
			
			}else if( typeOp1==0 && typeOp2==0) //si 1er est une constante et 2iem est une constante
			{
				//printf("++++++++++++++++++++++++ %d %d \n",abs_rel1,abs_rel2);
				int NouvAdrOp1=obtenirAdressePremierOperande(); 	
				int NouvAdrOp2 = obtenirAdressDeuxiemeOperande();
			
				if (abs_rel1==0 && abs_rel2==0)
					printf("%s @%d @@%d @@%d\n",operation,getAdressePile(),NouvAdrOp1,NouvAdrOp2);
				else if (abs_rel1==0 && abs_rel2==1)
					 printf("%s @%d @@%d @%d\n",operation,getAdressePile(),NouvAdrOp1,NouvAdrOp2);
				else if (abs_rel1==1 && abs_rel2==0)
					 printf("%s @%d @%d @@%d\n",operation,getAdressePile(),NouvAdrOp1,NouvAdrOp2);
				else if (abs_rel1==1 && abs_rel2==1)
					 printf("%s @%d @%d @%d\n",operation,getAdressePile(),NouvAdrOp1,NouvAdrOp2);
		
				empilert(getAdressePile(),1,1,typage_Op1) ;
			}
			return 0 ;
		}

}

void nombre_negatif_asm(int * dollar_addr,int * dollar_relative_ou_absolue,int * dollar_typage_result)
{
	int typeOp;
	int abs_rel;
	int typage_var;
	int valeurOp = depilert(&typeOp,&abs_rel,&typage_var);
	if (typeOp==0)//si constante
	{
		if (abs_rel==0)
			printf("AFC @@%d -%d\n",empilert(-valeurOp,0,abs_rel,typage_var),valeurOp);
		else 
			printf("AFC @%d -%d\n",empilert(-valeurOp,0,abs_rel,typage_var),valeurOp);
		incrementerPC();
	}/*else  //si variable (TODO)
	{
		int new_addr = empilert(valeurOp,0,abs_rel);
		printf("AFC @%d -%d\n",empilert(valeurOp,0,abs_rel,),1);
	}*/

	*dollar_addr=getAdressePile()+1;
	*dollar_relative_ou_absolue=abs_rel;
	*dollar_typage_result=typage_var;
	//empiler(-valeurOp,0);
}

void JMF_IF_ASM(int addr_condition)
{
	viderPile();
	char * label =  ajouter_label();
	printf("JMF @%d %s\n",addr_condition,label); //condition se trouve toujours à une adresse au format fixe 
	incrementerPC();
	empilerPremierLabelIF(label); 
}

void JMP_IF_ASM()
{
	char * label =  ajouter_label();
	printf("JMP %s\n",label);
	incrementerPC();
	empilerDeuxiemeLabelIF(label);
}

void ELSE_FIRST_LABEL_ASM()
{
	char label[TAILLE];
	depilerPremierLabelIF(label) ;
	modifierNum_instruction(label,pc);
}

void ELSE_SECOND_LABEL_ASM()
{
	char label[TAILLE];
	depilerDeuxiemeLabelIF(label);
	modifierNum_instruction(label,pc);
}

void IF_WITHOUT_ELSE_ASM()
{
	char label[TAILLE];
	depilerPremierLabelIF(label) ;
	char label2[TAILLE];
	depilerDeuxiemeLabelIF(label2);
	modifierNum_instruction(label,pc);
	//printf("------------------------%d\n",pc);
	modifierNum_instruction(label2,pc);
	//printf("------------------------%d\n",pc);
	//viderPile();
}

//avec les conditions, tout est en adressage fixe car on les empile 

void CONDITION_ASM(char * comparateur, int * dollar_addr , int * dollar_relative_ou_absolue, int * dollar_typage_result ,int addr_exp1,int addr_exp2, int abs_rel1, int abs_rel2 )
{
	int retour =empilert(-1,1,1,1);
	*dollar_addr=retour;
	*dollar_relative_ou_absolue=1;
	 * dollar_typage_result = 1;
	if(strcmp(comparateur,"==")==0)
	{
		if (abs_rel1==0 && abs_rel2==0)
		{	
			printf("EQU @%d @@%d @@%d\n", retour,addr_exp1,addr_exp2);
		} else if  (abs_rel1==0 && abs_rel2==1)
		{
			printf("EQU @%d @@%d @%d\n", retour,addr_exp1,addr_exp2);
		} else if  (abs_rel1==1 && abs_rel2==0)
		{
			printf("EQU @%d @%d @@%d\n", retour,addr_exp1,addr_exp2);
		} else if  (abs_rel1==1 && abs_rel2==1)
		{
			printf("EQU @%d @%d @%d\n", retour,addr_exp1,addr_exp2);
		}
		incrementerPC();

	}else if(strcmp(comparateur,"<")==0)
	{
		if (abs_rel1==0 && abs_rel2==0)
		{	
			printf("INF @%d @@%d @@%d\n", retour,addr_exp1,addr_exp2);
		} else if  (abs_rel1==0 && abs_rel2==1)
		{
			printf("INF @%d @@%d @%d\n", retour,addr_exp1,addr_exp2);
		} else if  (abs_rel1==1 && abs_rel2==0)
		{
			printf("INF @%d @%d @@%d\n", retour,addr_exp1,addr_exp2);
		} else if  (abs_rel1==1 && abs_rel2==1)
		{
			printf("INF @%d @%d @%d\n", retour,addr_exp1,addr_exp2);
		}
		incrementerPC();
	}else if(strcmp(comparateur,">")==0)
	{
		if (abs_rel1==0 && abs_rel2==0)
		{	
			printf("SUP @%d @@%d @@%d\n", retour,addr_exp1,addr_exp2);
		} else if  (abs_rel1==0 && abs_rel2==1)
		{
			printf("SUP @%d @@%d @%d\n", retour,addr_exp1,addr_exp2);
		} else if  (abs_rel1==1 && abs_rel2==0)
		{
			printf("SUP @%d @%d @@%d\n", retour,addr_exp1,addr_exp2);
		} else if  (abs_rel1==1 && abs_rel2==1)
		{
			printf("SUP @%d @%d @%d\n", retour,addr_exp1,addr_exp2);
		}
		incrementerPC();
	}
} 

void WHILE_FIRST_LABEL_ASM()
{
	char * label = ajouter_label(); 
	//printf("LABEL %s\n",label);
	empilerPremierLabelWhile(label);
	modifierNum_instruction(label,pc);
	//	incrementerPC();
}

void JMF_WHILE_ASM(int dollar_addr)
{
	viderPile();
	char * label =  ajouter_label();
	printf("JMF @%d %s\n",dollar_addr,label);
	incrementerPC();
	empilerDeuxiemeLabelWhile(label); 	
}

void JMP_WHILE_ASM()
{
	char  label[TAILLE] ; 
	depilerPremierLabelWhile(label);
	printf("JMP %s\n", label);
	incrementerPC();
}


void WHILE_SECOND_LABEL_ASM()
{
	char  label[TAILLE] ;
	depilerDeuxiemeLabelWhile(label) ;
	//printf("LABEL %s\n",label);
	modifierNum_instruction(label,pc);
	//viderPile();
}

int ADD_PROTOTYPE_ASM() 
{
	parametres  t[getNombredeParametres()] ;
	getTab_parametres(t) ;

	if (ajouter_Prototype(getIDprototypeOrImplementationFunction(), getTypeRetour(), t, getNombredeParametres())!=0)
	{
		return -1 ;  
	}else 
	{
		initNombreDeParametres() ;
		return 0 ;
	}
}

int ADD_IMPLEMENTATION_FUNCTION_ASM(int addr_implementation)
{
	parametres  t[getNombredeParametres()] ;
	getTab_parametres(t) ;

	if ( ajouter_implementations(getIDprototypeOrImplementationFunction(), getTypeRetour(),t,addr_implementation, getNombredeParametres())!=0)
	{
		return -1;
	} 
		//initNombreDeParametres() ; Il faut le faire quand on est sorti de la fonction
		return 0 ;

}

void PUSH_PC_ASM()
{
	/*int adrResult=getAdresseRetour(taille_element_retour); 
	 setAdresseDuReturn(adrResult);*/
	printf("PUSH PC\n");
	incrementerPC();
	/*printf("PUSH %d\n",adrResult);
	incrementerPC();*/
}

/*empiler les parametres d'appel */
void CALL_PARAMETERS_ASM(int dollar_addr , int dollar_relative_ou_absolue,int first_push)
{
	int type,abs_rel,typage;

	if (first_push)
	{
		printf("APB %d\n",length_table_symbole() );// pour sauvegarder les variables locales
		incrementerPC();
	}

	if (dollar_relative_ou_absolue==0) //relatif 
	{	
		printf("PUSH @@%d\n",dollar_addr);	
	}else //absolue
	{
		printf("PUSH @%d\n",dollar_addr);
	}
	incrementerPC();
	depilert(&type,&abs_rel,&typage);
}

void CALL_PARAMETERS_ASM_WITHOUT_AARGUMENT()
{
	
		printf("APB %d\n",length_table_symbole() );// pour sauvegarder les variables locales
		incrementerPC();
}

int VERIFICATION_AND_CALL_GENERATION_ASM()
{
	parametres t[getNombredeParametresAPPEL()];
	char  labelPotentiel[100];
	getTab_parametresAPPEL( t ) ;
	//printf("id de la function %s : nb parametre : %d \n",getIDprototypeOrImplementationFunction(),getNombredeParametresAPPEL());

	//printParametresDappel(  t,getNombredeParametresAPPEL());
	//print_TABLE_DES_FONCTION() ;
	//printf("----------------NOMBRE PARAME APPEL : %d\n",getNombredeParametresAPPEL());
	int retour = checkAppelFonctionParametreConforme( getIDprototypeOrImplementationFunction(), t , getNombredeParametresAPPEL(), labelPotentiel) ;

	if (retour==-2)
	{
		//empilert(0,1,0,1);//on empile l'adresse du résultat qui sera toujours 0 (@ relatif)

		//printf("APB %d\n",length_table_symbole() );// pour sauvegarder les variables locales
		printf("CALL %s\n",labelPotentiel);
		//incrementerPC();
		incrementerPC();
		return 0 ; 
	}else if (retour==-1)
	{
		return -1 ;//erreur fonction appelé non existante 
	}else 
	{
		//empilert(0,1,0,1);//on empile l'adresse du résultat qui sera toujours 0 (@ relatif)

		//printf("APB %d\n",length_table_symbole() );// pour sauvegarder les variables locales
		printf("CALL %d\n",retour);
		//incrementerPC();
		incrementerPC();
		initNombreDeParametresAPPEL() ; //on reinitialise pour l'appel suivant 
		
		return 0 ;
	}

}



void RETURN_ASM (int dollar_addr, int dollar_relative_ou_absolue,int dollar_typage_result) 
{

	if (dollar_relative_ou_absolue==0) //relatif 
	{
		printf("COP @%d @@%d\n",Registre_retour_fonctions,dollar_addr);
	}else //absolue
	{
		printf("COP @%d @%d\n",Registre_retour_fonctions,dollar_addr);
	}

	printf("RET\n");
	incrementerPC();
	incrementerPC();
}

/*void APPEL_FONCTION_IN_EXPRESSION_ASM()
{
		//on empile l'@du return , on en a besoin pour les cas ou on des choses du type return n* facto(n-1) par exemple
		printf("AFC @%d %d\n",empilert(getAdresseDuReturn(),0,1,getTypeRetour()),getAdresseDuReturn());
		incrementerPC();
}*/

void PushResultFunction()
{
	int addr =  empilert(0,0,0,1);//on empile l'adresse du résultat qui sera toujours 0 (@ relatif)	
	printf("COP @%d @%d\n",addr,Registre_retour_fonctions);
	incrementerPC();
}



