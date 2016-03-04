#ifndef __GESTION_DES_FONCTIONS__
#define __GESTION_DES_FONCTIONS__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOMBRES_PARAMATRES 20 
#define NOMBRES_MAX_DE_FONCTIONS_A_GERER 50
#define TAILLE_NOM_FONCTION 150  

extern int indiceNombreDefonctionDeclare ;


union parametres
{
	int entier; 
} ;

typedef union parametres parametres ;


union type_retour 
{
	int entier ;
} ;

typedef union type_retour type_retour ; 

struct Noeud_Fonctions 
{
	int adresse_prototype ; 
	int adresse_implementation ;
	char nom_fonction[TAILLE_NOM_FONCTION] ;  
	int nombres_paramatres ; 
	parametres Tab_parametres[NOMBRES_PARAMATRES]; 
	type_retour retour ; 
};

typedef struct Noeud_Fonctions Noeud_Fonctions ; 

Noeud_Fonctions Tableau_gestion_des_fonctions[NOMBRES_MAX_DE_FONCTIONS_A_GERER] ; 

void ajouter_Prototyper(char * nom_fonction , int type_retour, parametres p ,int  adresse_prototype) ; 

void ajouter_implementations(char * nom_fonction , int type_retour, parametres p ,int  adresse_implementation) ; 




#endif