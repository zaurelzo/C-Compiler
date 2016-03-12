#include "gestion_des_fonctions.h"

 int indiceNombreDefonctionDeclare =0 ; 


void ajouter_Prototype(char * nom_fonction , int type_retour, parametres * p , int nombres_paramatres) 
{
	strcpy(Tableau_des_fonctions[indiceNombreDefonctionDeclare].nom_fonction , nom_fonction);

	Tableau_des_fonctions[indiceNombreDefonctionDeclare].retour = type_retour ;

	
	//Tableau_des_fonctions[indiceNombreDefonctionDeclare].adresse_prototype=  adresse_prototype
	Tableau_des_fonctions[indiceNombreDefonctionDeclare].adresse_implementation = -1 ;//on ne connait pas encore l'@
	Tableau_des_fonctions[indiceNombreDefonctionDeclare].nombres_paramatres = nombres_paramatres ;

	int i ; 
	for ( i = 0; i < nombres_paramatres; ++i)
	{
		Tableau_des_fonctions[indiceNombreDefonctionDeclare].Tab_parametres[i].type_du_parametre = p[i].type_du_parametre ; 
	}

	indiceNombreDefonctionDeclare++; 

}

int  fonctionExiste(char * nom_fonction , parametres * p , int nombres_paramatres , int  type_retour )
{
	int i ,j  ;
	int retour = 1 ; 
	for ( i = 0; i < indiceNombreDefonctionDeclare; ++i)
	 {
	 	if (strcmp(nom_fonction,Tableau_des_fonctions[i].nom_fonction)==0 && (type_retour== Tableau_des_fonctions[i].retour) && Tableau_des_fonctions[i].nombres_paramatres==nombres_paramatres)
	 	{
	 		for (j = 0; j < nombres_paramatres; ++j)
	 		{
	 			if (Tableau_des_fonctions[i].Tab_parametres[i].type_du_parametre !=  p[j].type_du_parametre)
	 				retour=-1 ; 
	 		}

	 		if (retour !=-1)
	 		{
	 			retour= i; 
	 		}
	 	} 
	 		//retour= i ; 
	 } 

	 return retour; 
}

void ajouter_implementations(char * nom_fonction , int type_retour, parametres *  p ,int  adresse_implementation , int nombres_paramatres)
{
	int retour =fonctionExiste(nom_fonction,p,nombres_paramatres,type_retour);
	if(retour==-1)
	{
		ajouter_Prototype(nom_fonction,type_retour,p,nombres_paramatres) ; 
		Tableau_des_fonctions[indiceNombreDefonctionDeclare-1].adresse_implementation = adresse_implementation ;
	}	else 
	{
		Tableau_des_fonctions[retour].adresse_implementation = adresse_implementation ;
	}
}


