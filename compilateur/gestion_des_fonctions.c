#include "gestion_des_fonctions.h"

 int indiceNombreDefonctionDeclare =0 ; 
int indTableDesParametres = 0 ;

 int numeroParametresEncoursDAnalyse = -1;

//@return 
int ajouter_Prototype(char * nom_fonction , int type_retour, parametres * p , int nombres_paramatres) 
{
	int prot_or_implementation,retour;
	if ((retour = fonction_ou_prototype_Existe(nom_fonction,p,nombres_paramatres,type_retour, &prot_or_implementation)) !=-1)
	{
		//printf("mmmmmmmmmmmmmmmm %d\n",prot_or_implementation);
		return -1; // c'est sur certain que c'est le prototype qui existe, pas l'implémentation
	}else 
	{
		//printf("=======DEBUG :%d\n",retour) ;
		strcpy(Tableau_des_fonctions[indiceNombreDefonctionDeclare].nom_fonction , nom_fonction);

		Tableau_des_fonctions[indiceNombreDefonctionDeclare].retour = type_retour ;


		Tableau_des_fonctions[indiceNombreDefonctionDeclare].adresse_prototype= 1 ;//on s'en fou de ce que l'on met dedans , l'important que ça soit diffenret de -1
		Tableau_des_fonctions[indiceNombreDefonctionDeclare].adresse_implementation = -1 ;//on ne connait pas encore l'@
		Tableau_des_fonctions[indiceNombreDefonctionDeclare].nombres_paramatres = nombres_paramatres ;

		int i ; 
		for ( i = 0; i < nombres_paramatres; ++i)
		{
			//prendre en compte plus tard la profondeur pour les pointeurs sur fonctions
			Tableau_des_fonctions[indiceNombreDefonctionDeclare].Tab_parametres[i].type_du_parametre = p[i].type_du_parametre ; 
		}

		indiceNombreDefonctionDeclare++;
		return 0 ; 
	}
}

//si la fonction existe ou le prototype , retourne son indice dans la table des fonctions sinon retourn -1
//param int * prototype_ou_fonction : 1 prototype , 2 : implementation_fonction , 3 : prototupe et implementation fonction present 
int  fonction_ou_prototype_Existe(char * nom_fonction , parametres * p , int nombres_paramatres , int  type_retour ,int * prototype_ou_fonction)
{
	int i ,j  ;
	//int retour = -1 ; 
	int indice=-1;
	for ( i = 0; i < indiceNombreDefonctionDeclare; ++i)
	 {
	 	if (strcmp(nom_fonction,Tableau_des_fonctions[i].nom_fonction)==0 && (type_retour== Tableau_des_fonctions[i].retour) && Tableau_des_fonctions[i].nombres_paramatres==nombres_paramatres)
	 	{
	 		//verifier plus si quand le type est un pointeur, sa profoncdeur 
	 		for (j = 0; j < nombres_paramatres; ++j)
	 		{
	 			//si un para est different, les machins sont différents
	 			if (Tableau_des_fonctions[i].Tab_parametres[j].type_du_parametre !=  p[j].type_du_parametre)
	 			{
	 				//printf("IN FONCTIO : %d / %d\n",Tableau_des_fonctions[i].adresse_prototype,Tableau_des_fonctions[i].adresse_implementation )
	 				return -1 ;
	 			} 
	 		}

	 		if(Tableau_des_fonctions[i].adresse_prototype!=-1 && Tableau_des_fonctions[i].adresse_implementation==-1 )
	 					*prototype_ou_fonction =1 ;  
	 		else if ((Tableau_des_fonctions[i].adresse_prototype==-1 && Tableau_des_fonctions[i].adresse_implementation!=-1 ))
	 			*prototype_ou_fonction= 2 ;

	 		indice = i;
	 	}  
	 } 

	 return indice; 
}

int ajouter_implementations(char * nom_fonction , int type_retour, parametres *  p ,int  adresse_implementation , int nombres_paramatres)
{
	int prot_or_implementation=-99;
	//fonction existe et est déjà implémenté
	int indice ; 
	if ( ( (indice=fonction_ou_prototype_Existe(nom_fonction,p,nombres_paramatres,type_retour, &prot_or_implementation))!=-1) && (prot_or_implementation ==2) )  
	{	
		return -1; //fonction deja implémenté
	}else
	{
		//printf("cool \n");
		//printf("=========prot_or_implementation %d : \n", prot_or_implementation);
		if (prot_or_implementation!=1)
		{
			indice = indiceNombreDefonctionDeclare ;
			indiceNombreDefonctionDeclare++;
		}
		strcpy(Tableau_des_fonctions[indice].nom_fonction , nom_fonction);
		Tableau_des_fonctions[indice].retour = type_retour ;
		Tableau_des_fonctions[indice].adresse_implementation = adresse_implementation ;
		Tableau_des_fonctions[indice].nombres_paramatres = nombres_paramatres ;
		Tableau_des_fonctions[indice].adresse_prototype= -1 ;
		//if ()

		int i ; 
		for ( i = 0; i < nombres_paramatres; ++i)
		{
			//prendre en compte plus tard la profondeur pour les pointeurs sur fonctions
			Tableau_des_fonctions[indice].Tab_parametres[i].type_du_parametre = p[i].type_du_parametre ; 
		}
		
		return 0 ; 
	}
}


void print_TABLE_DES_FONCTION()
{
	printf("=================CONTENTS TABLE FUNCTIONS =====================");
	int i ,j;
	for ( i = 0; i < indiceNombreDefonctionDeclare; ++i)
	{
		printf("======================================\n");
		printf("nom fonction : %s\n",Tableau_des_fonctions[i].nom_fonction);
		printf("type retour : %d\n", Tableau_des_fonctions[i].retour);
		printf("adresse implementation :  %d \n",Tableau_des_fonctions[i].adresse_implementation) ;
		printf("adresse prototype : %d \n",Tableau_des_fonctions[i].adresse_prototype);
		printf("Nombre de parametres : %d\n",Tableau_des_fonctions[i].nombres_paramatres);
		for ( j = 0; j <Tableau_des_fonctions[i].nombres_paramatres ; ++j)
		{
			printf("type parametre %d : %d \n",j,Tableau_des_fonctions[i].Tab_parametres[j].type_du_parametre );
		}
	}
}



int ajouter_parametre(int type_du_parametre , int profondeur ,char * nom_du_parametre)
{
	int i ;
	for ( i = 0; i < indTableDesParametres; ++i)
	{
		// si param de meme nom et meme type existe deja, alors erreur 
		if ( strcmp(Tab_parametres[i].nom,nom_du_parametre)==0  && Tab_parametres[i].type_du_parametre == type_du_parametre )
		{
			return -1 ;
		}
	}

	Tab_parametres[indTableDesParametres].type_du_parametre =type_du_parametre;
	strcpy(Tab_parametres[i].nom,nom_du_parametre);
	//Tab_parametres[indTableDesParametres].profondeur= profondeur ; //à predre en compte plus tard 
	indTableDesParametres++;
	return 0 ;
}

int getNombredeParametres()
{
	return indTableDesParametres ;
}

void  getTab_parametres(parametres * t ) 
{
	int i ;
	for ( i = 0; i < indTableDesParametres; ++i)
	{
		t[i].type_du_parametre = Tab_parametres[i].type_du_parametre ;
		//printf("%d\n",Tab_parametres[i].type_du_parametre);
		//T[i].profondeur = Tab_parametres[i].profondeur; //à prendre en compte plus tard 
	}
	
}


void printTableParametres()
{
	int i ;
	for ( i = 0; i < indTableDesParametres; ++i)
	{
		printf("type param %d : %d \n",i,Tab_parametres[i].type_du_parametre );

		//T[i].profondeur = Tab_parametres[i].profondeur; //à prendre en compte plus tard 
	}
}

void initNombreDeParametres()
{
	indTableDesParametres=0;
}

void setIDprototypeOrImplementationFunction(char *  name )
{
	strcpy(nom_fonction,name);
}

char * getIDprototypeOrImplementationFunction()
{
	return nom_fonction ; 
}

void setTypeRetour(int type_retour)
{
	type_retour = type_retour ; 
} 
int getTypeRetour()
{
	return type_retour ;
}

int  incrementeNumeroParametresEncoursDAnalyse()  
{
 	numeroParametresEncoursDAnalyse++;
 	return  numeroParametresEncoursDAnalyse ;
}

void NumeroParametresEncoursDAnalyse()
{
	 numeroParametresEncoursDAnalyse=0;
}

int checkAppelFonctionParametreConforme(char * nom_fonction , int numParametre,int type , int profondeur) 
{
	int i ; 
	for (i = 0; i <indiceNombreDefonctionDeclare ; ++i)
	{
		if (strcmp(nom_fonction, Tableau_des_fonctions[i].nom_fonction)==0  && Tableau_des_fonctions[i].Tab_parametres[numParametre].type_du_parametre ==type)
		{	
			//s'occuper de la profondeur plus tard 
			return 0 ;
		}
	}
	return -1 ; 
}

