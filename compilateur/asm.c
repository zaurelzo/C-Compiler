#include "asm.h"
#include "tab_symb.h"
#include "tab_label.h"

int declaration_affectation_asm( char * nom_var,int constante ,int dollar_relative_ou_absolue , int dollar_addr)
{
	if (ajouter_Var(nom_var,1,1,constante )==-1)
	{
		return -1 ; //pas reussi 
	}else 
	{
		int abs_rel ; 
		int var = recherchet(nom_var,&abs_rel);
		if (dollar_relative_ou_absolue==0 && abs_rel==0)
			printf( "\x1B[37mCOP @@%d @@%d \n",var,dollar_addr);
		else if  (dollar_relative_ou_absolue==0 && abs_rel==1)
			printf( "\x1B[37mCOP @@%d @%d \n",var,dollar_addr);
		else if  (dollar_relative_ou_absolue==1 && abs_rel==0)
			printf( "\x1B[37mCOP @%d @@%d \n",var,dollar_addr);
		else if  (dollar_relative_ou_absolue==1 && abs_rel==1)
		printf( "\x1B[37mCOP @%d @%d \n",var,dollar_addr);

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
		if (dollar_relative_ou_absolue==0 && abs_rel==0)
			printf( "\x1B[37mCOP @@%d @@%d \n",var,dollar_addr);
		else if  (dollar_relative_ou_absolue==0 && abs_rel==1)
			printf( "\x1B[37mCOP @%d @@%d \n",var,dollar_addr);
		else if  (dollar_relative_ou_absolue==1 && abs_rel==0)
			printf( "\x1B[37mCOP @@%d @%d \n",var,dollar_addr);
		else if  (dollar_relative_ou_absolue==1 && abs_rel==1)
			printf( "\x1B[37mCOP @%d @%d \n",var,dollar_addr);
																													
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
				printf("\x1B[37m%s @%d @@%d @@%d\n",operation,getAdressePile(),valeurOp2,valeurOp1);
			else if (abs_rel1==0 && abs_rel2==1)
				printf("\x1B[37m%s @%d @%d @@%d\n",operation,getAdressePile(),valeurOp2,valeurOp1); 	
			else if (abs_rel1==1 && abs_rel2==0)
				printf("\x1B[37m%s @%d @@%d @%d\n",operation,getAdressePile(),valeurOp2,valeurOp1); 	
			else if (abs_rel1==1 && abs_rel2==1)
				printf("\x1B[37m%s @%d @%d @%d\n",operation,getAdressePile(),valeurOp2,valeurOp1); 	
			
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