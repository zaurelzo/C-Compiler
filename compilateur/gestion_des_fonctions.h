#ifndef __GESTION_DES_FONCTIONS__
#define __GESTION_DES_FONCTIONS__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOMBRES_PARAMATRES 20 
#define NOMBRES_MAX_DE_FONCTIONS_A_GERER 50
#define TAILLE_NOM_FONCTION 150  

extern int indiceNombreDefonctionDeclare ;


struct parametres
{
	int type_du_parametre; //0 pour un entier ;
	int entier; 
} ;

typedef struct parametres parametres ;



struct Noeud_Fonctions 
{
	int adresse_prototype ; 
	int adresse_implementation ;
	char nom_fonction[TAILLE_NOM_FONCTION] ;  
	int nombres_paramatres ; 
	parametres Tab_parametres[NOMBRES_PARAMATRES]; 
	int retour ; //0 int  
};

typedef struct Noeud_Fonctions Noeud_Fonctions ; 

Noeud_Fonctions Tableau_des_fonctions[NOMBRES_MAX_DE_FONCTIONS_A_GERER] ; 

void ajouter_Prototype(char * nom_fonction , int type_retour, parametres * p , int nombres_paramatres) ; 

void ajouter_implementations(char * nom_fonction , int type_retour, parametres *  p ,int  adresse_implementation , int nombres_paramatres); 

/*-1 si la fonction n'est existe pas dans la table des fonctions */
int  fonctionExiste(char * nom_fonction , parametres * p , int nombres_paramatres , int  type_retour ) ;



#endif