#include "asm.h"
#include "tab_symb.h"
#include "tab_label.h"


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

int declaration_affectation_asm( char * nom_var,int constante ,int dollar_relative_ou_absolue , int dollar_addr)
{
	if (ajouter_Var(nom_var,1,1,constante )==-1)
	{
		return -1 ; //pas reussi 
	}else 
	{
		int abs_rel ;
		int var = recherchet(nom_var,&abs_rel); 
		
		/*if (strcmp(getMode(),"fonction")==0) 
		{
			//printf("------------%d\n",getTailleTypeRetourFonction());	
			var += getTailleTypeRetourFonction() ; //si on un type retour
			
		}*/

		if (dollar_relative_ou_absolue==0 && abs_rel==0)
		{	
			/*if (strcmp(getMode(),"fonction")==0)
			{
				dollar_addr +=getTailleTypeRetourFonction() ;
			}*/
			printf( "\x1B[37mCOP @@%d @@%d \n",var,dollar_addr);

		} else if  (dollar_relative_ou_absolue==0 && abs_rel==1)
		{
			/*if (strcmp(getMode(),"fonction")==0)
			{
				dollar_addr +=getTailleTypeRetourFonction() ;
			}*/
			printf( "\x1B[37mCOP @@%d @%d \n",var,dollar_addr);

		} else if  (dollar_relative_ou_absolue==1 && abs_rel==0)
		{
			
			printf( "\x1B[37mCOP @%d @@%d \n",var,dollar_addr);
		} else if  (dollar_relative_ou_absolue==1 && abs_rel==1)
		{
			printf( "\x1B[37mCOP @%d @%d \n",var,dollar_addr);
		}
		incrementerPC();
		viderPile();
		modifierChampInitialiserVariable(nom_var);

	return 0 ;
	}
}

int affection_asm( char * nom_var ,int dollar_relative_ou_absolue , int dollar_addr) 
{

	int var ;
	int abs_rel ; 
																									
	if ((  var =recherchet(nom_var,&abs_rel))==-1)
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
			printf( "\x1B[37mCOP @@%d @@%d \n",var,dollar_addr);
		}else if  (dollar_relative_ou_absolue==0 && abs_rel==1)
		{	
			/*if (strcmp(getMode(),"fonction")==0)
			{
				dollar_addr +=getTailleTypeRetourFonction() ;
			}*/
			printf( "\x1B[37mCOP @%d @@%d \n",var,dollar_addr);
		}else if  (dollar_relative_ou_absolue==1 && abs_rel==0)
		{
			printf( "\x1B[37mCOP @@%d @%d \n",var,dollar_addr);
		}else if  (dollar_relative_ou_absolue==1 && abs_rel==1)
		{
			printf( "\x1B[37mCOP @%d @%d \n",var,dollar_addr);
		}																											
		incrementerPC();
		viderPile();
		modifierChampInitialiserVariable(nom_var);									
	}
	return 0 ;
}

int print_asm(int addr )
{
	if(addr==0)
		printf("\x1B[37mPRI @@%d\n",addr);
	else 
		printf("\x1B[37mPRI @%d\n",addr);
																							
	incrementerPC();
	viderPile();
}

int operation_arithmetique_asm(char * operation , int * dollar_addr, int * dollar_relative_ou_absolue )
{
	int typeOp1,typeOp2; 
	int  abs_rel1,abs_rel2;
	int valeurOp2 = depilert(&typeOp2,&abs_rel2);
	int valeurOp1= depilert(&typeOp1,&abs_rel1);
	//printf("type 1 : %d | type 2 : %d\n",typeOp1,typeOp2);
	if (valeurOp2==-1 || valeurOp1==-1 )
	{
		//printf("ERREUR indicePile :%d\n",indPile );
		return -1 ; // erreur depilement des opérandes 
	}else 
	{
		*dollar_addr= getAdressePile();
		incrementerPC();
		* dollar_relative_ou_absolue=1;
		
		if (typeOp1==1 && typeOp2==1)//si 1er et 2iem opérande est une variable 
		{

			if (abs_rel1==0 && abs_rel2==0)
			{
				printf("\x1B[37m%s @%d @@%d @@%d\n",operation,getAdressePile(),valeurOp2,valeurOp1);
			}else if (abs_rel1==0 && abs_rel2==1)
			{
				printf("\x1B[37m%s @%d @%d @@%d\n",operation,getAdressePile(),valeurOp2,valeurOp1); 	
			}else if (abs_rel1==1 && abs_rel2==0)
			{
				printf("\x1B[37m%s @%d @@%d @%d\n",operation,getAdressePile(),valeurOp2,valeurOp1); 	
			}else if (abs_rel1==1 && abs_rel2==1)
			{
				printf("\x1B[37m%s @%d @%d @%d\n",operation,getAdressePile(),valeurOp2,valeurOp1); 	
			}

			empilert(getAdressePile(),1,1) ;
			
		}else if( typeOp1==1 && typeOp2==0) //si 1er est une var et 2iem est une constante
		{
			//printf("++++++++++++++++++++++++ %d %d \n",abs_rel1,abs_rel2);
			int NouvAdrOp2= obtenirAdressDeuxiemeOperande();
			if (abs_rel1==0 && abs_rel2==0)
				printf("\x1B[37m%s @%d @@%d @@%d\n",operation,getAdressePile(),valeurOp1,NouvAdrOp2);
			else if (abs_rel1==0 && abs_rel2==1)
				printf("\x1B[37m%s @%d @@%d @%d\n",operation,getAdressePile(),valeurOp1,NouvAdrOp2);
			else if (abs_rel1==1 && abs_rel2==0)
				printf("\x1B[37m%s @%d @%d @@%d\n",operation,getAdressePile(),valeurOp1,NouvAdrOp2);
			else if (abs_rel1==1 && abs_rel2==1)
				printf("\x1B[37m%s @%d @%d @%d\n",operation,getAdressePile(),valeurOp1,NouvAdrOp2);
		
			empilert(getAdressePile(),1,1) ;
		}else if( typeOp1==0 && typeOp2==1) //si 1er est une constante et 2iem est une var
		{
			
			int NouvAdrOp1=obtenirAdressePremierOperande(); 
			
			if (abs_rel1==0 && abs_rel2==0)
				printf("\x1B[37m%s @%d @@%d @@%d\n",operation,getAdressePile(),NouvAdrOp1,valeurOp2);
			if (abs_rel1==0 && abs_rel2==1)
				printf("\x1B[37m%s @%d @@%d @%d\n",operation,getAdressePile(),NouvAdrOp1,valeurOp2);
			if (abs_rel1==1 && abs_rel2==0)
				printf("\x1B[37m%s @%d @%d @@%d\n",operation,getAdressePile(),NouvAdrOp1,valeurOp2);
			if (abs_rel1==1 && abs_rel2==1)
				printf("\x1B[37m%s @%d @%d @%d\n",operation,getAdressePile(),NouvAdrOp1,valeurOp2);
				
			empilert(getAdressePile(),1,1) ;
			
		}else if( typeOp1==0 && typeOp2==0) //si 1er est une constante et 2iem est une constante
		{
			//printf("++++++++++++++++++++++++ %d %d \n",abs_rel1,abs_rel2);
			int NouvAdrOp1=obtenirAdressePremierOperande(); 	
			int NouvAdrOp2 = obtenirAdressDeuxiemeOperande();
			
			if (abs_rel1==0 && abs_rel2==0)
				printf("\x1B[37m%s @%d @@%d @@%d\n",operation,getAdressePile(),NouvAdrOp1,NouvAdrOp2);
			else if (abs_rel1==0 && abs_rel2==1)
				 printf("\x1B[37m%s @%d @@%d @%d\n",operation,getAdressePile(),NouvAdrOp1,NouvAdrOp2);
			else if (abs_rel1==1 && abs_rel2==0)
				 printf("\x1B[37m%s @%d @%d @@%d\n",operation,getAdressePile(),NouvAdrOp1,NouvAdrOp2);
			else if (abs_rel1==1 && abs_rel2==1)
				 printf("\x1B[37m%s @%d @%d @%d\n",operation,getAdressePile(),NouvAdrOp1,NouvAdrOp2);
		
			empilert(getAdressePile(),1,1) ;
		}
	return 0 ;
	}
}

void nombre_negatif_asm(int * dollar_addr,int * dollar_relative_ou_absolue)
{
	int typeOp;
	int abs_rel;
	int valeurOp = depilert(&typeOp,&abs_rel);
	if (typeOp==0)//si constante
	{
		if (abs_rel==0)
			printf("\x1B[37mAFC @@%d -%d\n",empilert(-valeurOp,0,abs_rel),valeurOp);
		else 
			printf("\x1B[37mAFC @%d -%d\n",empilert(-valeurOp,0,abs_rel),valeurOp);
		incrementerPC();
	}/*else  //si variable (TODO)
	{
		int new_addr = empilert(valeurOp,0,abs_rel);
		printf("\x1B[37mAFC @%d -%d\n",empilert(valeurOp,0,abs_rel,),1);
	}*/

	*dollar_addr=getAdressePile()+1;
	*dollar_relative_ou_absolue=abs_rel;
	//empiler(-valeurOp,0);
}

void JMF_IF_ASM(int addr_condition)
{
	viderPile();
	char * label =  ajouter_label();
	printf("\x1B[37mJMF @%d %s\n",addr_condition,label); //condition se trouve toujours à une adresse au format fixe 
	incrementerPC();
	empilerPremierLabelIF(label); 
}

void JMP_IF_ASM()
{
	char * label =  ajouter_label();
	printf("\x1B[37mJMP %s\n",label);
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

void CONDITION_ASM(char * comparateur, int * dollar_addr , int * dollar_relative_ou_absolue, int addr_exp1,int addr_exp2)
{
	int retour =empilert(-1,1,1);
	*dollar_addr=retour;
	*dollar_relative_ou_absolue=1;
	if(strcmp(comparateur,"==")==0)
	{
		printf("\x1B[37mEQU @%d @%d @%d\n", retour,addr_exp1,addr_exp2);
		incrementerPC();
	}else if(strcmp(comparateur,"<")==0)
	{
		printf("\x1B[37mINF @%d @%d @%d\n", retour,addr_exp1,addr_exp2);
		incrementerPC();
	}else if(strcmp(comparateur,">")==0)
	{
		printf("\x1B[37mSUP @%d @%d @%d\n", retour,addr_exp1,addr_exp2);
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
	printf("\x1B[37mJMF @%d %s\n",dollar_addr,label);
	incrementerPC();
	empilerDeuxiemeLabelWhile(label); 	
}

void JMP_WHILE_ASM()
{
	char  label[TAILLE] ; 
	depilerPremierLabelWhile(label);
	printf("\x1B[37mJMP %s\n", label);
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

void PUSH_ADDR_RETOUR_AND_PC_ASM(int taille_element_retour)
{
	printf("\x1B[37mPUSH PC\n");
	incrementerPC();
	printf("\x1B[37mPUSH %d\n",getAdresseRetour(taille_element_retour));
	incrementerPC();
}

void CALL_PARAMETERS_ASM(int dollar_addr , int dollar_relative_ou_absolue)
{
	
	if (dollar_relative_ou_absolue==0) //relatif 
	{	
		printf("\x1B[37mPUSHA @@%d\n",dollar_addr);	
	}else //absolue
	{
		printf("\x1B[37mPUSHA @%d\n",dollar_addr);
	}
	incrementerPC();
}

/*void CALL_ASM()
{

	printf("\x1B[37mCALL \n");
}*/