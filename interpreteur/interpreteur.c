#include "interpreteur.h"


 int PC =0 ;
int nb_Instruction_Programme = 0 ;

int traiter_ligne(char * chaine , int *tab_retour)
{
	int cpt = 0;
	char * token ; 
	token =strtok(chaine," ");
	 //strtok permet de decouper la chaine en tokens séparés par le delimitateur espace
	while(token!=NULL)
	{
		tab_retour[cpt]=atoi(token);
		//printf("token %s\n",token );
		token =strtok(NULL," ");
		cpt++;
	}
	return cpt ; 
}

void charger_Programme_dans_Ram(char * name_fichier)
{
	FILE* fichier = NULL;
 	int codeOP, resultat , retour , op1,op2; 
    fichier = fopen(name_fichier, "r");
    char line[10];
    int tableau_retour[4];

 	if (fichier!=NULL)
 	{
 	
 		while(fgets(line,10,fichier)!=NULL)
 		{
 			retour = traiter_ligne(line ,tableau_retour);
 				//printf("---------- retour %d | %d| %d| %d |%d-------------\n",retour,codeOP,resultat,op1,op2);
 			Tableau_Ram[nb_Instruction_Programme].format_instruction=retour;
 			if (retour==4)
 			{
 				Tableau_Ram[nb_Instruction_Programme].code_operation=tableau_retour[0] ;
 				Tableau_Ram[nb_Instruction_Programme].result=tableau_retour[1];
 				Tableau_Ram[nb_Instruction_Programme].operande1= tableau_retour[2];
 				Tableau_Ram[nb_Instruction_Programme].operande2=tableau_retour[3];
 			}else if (retour==3)
 			{
 				Tableau_Ram[nb_Instruction_Programme].code_operation=tableau_retour[0] ;
 				Tableau_Ram[nb_Instruction_Programme].result=tableau_retour[1];
 				Tableau_Ram[nb_Instruction_Programme].operande1=tableau_retour[2];

 			}else if (retour==2)
 			{
 				Tableau_Ram[nb_Instruction_Programme].code_operation=tableau_retour[0] ;
 				Tableau_Ram[nb_Instruction_Programme].result=tableau_retour[1];
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
			printf("%d %d %d %d\n",Tableau_Ram[i].code_operation,Tableau_Ram[i].result,Tableau_Ram[i].operande1,Tableau_Ram[i].operande2);
		}else if(Tableau_Ram[i].format_instruction==3)
		{
			printf("%d %d %d\n",Tableau_Ram[i].code_operation,Tableau_Ram[i].result,Tableau_Ram[i].operande1);
		}else if(Tableau_Ram[i].format_instruction==2)
		{
			printf("%d %d\n",Tableau_Ram[i].code_operation,Tableau_Ram[i].result);
		}
	}
}

void interpreter()
{
	
	while(PC < nb_Instruction_Programme)
	{	
		if(Tableau_Ram[PC].code_operation==1)//ADD
		{
			Tableau_Memoire_des_donnes[Tableau_Ram[PC].result].valeur= Tableau_Ram[PC].operande1+ Tableau_Ram[PC].operande2;
			PC++;
		} else if(Tableau_Ram[PC].code_operation==2)//MUL
		{
			Tableau_Memoire_des_donnes[Tableau_Ram[PC].result].valeur= Tableau_Ram[PC].operande1* Tableau_Ram[PC].operande2;
			PC++;
		} else if(Tableau_Ram[PC].code_operation==3)//SOU
		{
			Tableau_Memoire_des_donnes[Tableau_Ram[PC].result].valeur= Tableau_Ram[PC].operande1- Tableau_Ram[PC].operande2;
			PC++;
		} else if(Tableau_Ram[PC].code_operation==4)//DIV
		{
			Tableau_Memoire_des_donnes[Tableau_Ram[PC].result].valeur= Tableau_Ram[PC].operande1/ Tableau_Ram[PC].operande2;
			PC++;
		} else if(Tableau_Ram[PC].code_operation==5)//COPIE
		{
			Tableau_Memoire_des_donnes[Tableau_Ram[PC].result].valeur=Tableau_Memoire_des_donnes[Tableau_Ram[PC].operande1].valeur;
			PC++;
		} else if(Tableau_Ram[PC].code_operation==6)//AFC
		{
			Tableau_Memoire_des_donnes[Tableau_Ram[PC].result].valeur=Tableau_Ram[PC].operande1;
			PC++;
		} else if(Tableau_Ram[PC].code_operation==7)//SAUT INCONDITIONNEL 
		{
			PC=Tableau_Ram[PC].operande1;
		} else if(Tableau_Ram[PC].code_operation==8)//saut si la condition est fausse  
		{
			if (Tableau_Memoire_des_donnes[getAdresseResultatComparaison()].valeur==0)
				PC =Tableau_Ram[PC].operande1;
		} else if(Tableau_Ram[PC].code_operation==9)//Inferieur  
		{
			if (Tableau_Memoire_des_donnes[getAdressePremierOperandeCondition()].valeur<Tableau_Memoire_des_donnes[getAdresseDeuxiemeOperandeCondition()].valeur)
				Tableau_Memoire_des_donnes[getAdresseResultatComparaison()].valeur==1;
			else 
				Tableau_Memoire_des_donnes[getAdresseResultatComparaison()].valeur==0 ;
			PC++;
		} else if(Tableau_Ram[PC].code_operation==10)//superieur  
		{
			if (Tableau_Memoire_des_donnes[getAdressePremierOperandeCondition()].valeur>Tableau_Memoire_des_donnes[getAdresseDeuxiemeOperandeCondition()].valeur)
				Tableau_Memoire_des_donnes[getAdresseResultatComparaison()].valeur==1;
			else 
				Tableau_Memoire_des_donnes[getAdresseResultatComparaison()].valeur==0 ;
			PC++;
		} else if(Tableau_Ram[PC].code_operation==11)//superieur  
		{
			if (Tableau_Memoire_des_donnes[getAdressePremierOperandeCondition()].valeur==Tableau_Memoire_des_donnes[getAdresseDeuxiemeOperandeCondition()].valeur)
				Tableau_Memoire_des_donnes[getAdresseResultatComparaison()].valeur==1;
			else 
				Tableau_Memoire_des_donnes[getAdresseResultatComparaison()].valeur==0 ;
			PC++;
		}else if(Tableau_Ram[PC].code_operation==12)//PRINTER 
		{
			printf("%d\n",Tableau_Memoire_des_donnes[Tableau_Ram[PC].result].valeur);
			PC++;
		} 


	}
}