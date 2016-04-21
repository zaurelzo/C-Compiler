#ifndef __GESTION_DES_FONCTIONS__
#define __GESTION_DES_FONCTIONS__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define NOMBRES_PARAMATRES 20 
#define NOMBRES_MAX_DE_FONCTIONS_A_GERER 50
#define TAILLE_NOM_FONCTION 150  


/*==============================VARIABLES *===========================*/ 

extern int indiceNombreDefonctionDeclare ;
struct parametres
{
	int type_du_parametre; //1 pour un entier ;
	int profondeur; 
} ;

typedef struct parametres parametres ;

struct Noeud_Fonctions 
{
	int adresse_prototype ; 
	int adresse_implementation ;
	char nom_fonction[TAILLE_NOM_FONCTION] ;  
	int nombres_paramatres ; 
	parametres Tab_parametres[NOMBRES_PARAMATRES]; 
	int retour ; //0 void , 1 : int   
};

typedef struct Noeud_Fonctions Noeud_Fonctions ; 

Noeud_Fonctions Tableau_des_fonctions[NOMBRES_MAX_DE_FONCTIONS_A_GERER] ;

/*construction des parametres*/
extern int indTableDesParametres;
parametres Tab_parametres[NOMBRES_PARAMATRES];  


// nom de la fonction 
char nom_fonction[TAILLE_NOM_FONCTION] ;
//type retour ; 
int type_retour;
/*====================================END VARIABLES ================================*/




/*====================================FONCTIONS ========================================*/
//-1 si le prototype existe dejà
int ajouter_Prototype(char * nom_fonction , int type_retour, parametres * p , int nombres_paramatres) ;

//-1: si une fonction de même nom, et même parametres a déjà été déclarée
int ajouter_implementations(char * nom_fonction , int type_retour, parametres *  p ,int  adresse_implementation , int nombres_paramatres); 

/*-1 si la fonction n'est existe pas dans la table des fonctions */
int  fonction_ou_prototype_Existe(char * nom_fonction , parametres * p , int nombres_paramatres , int  type_retour ,int * prototype_ou_fonction);


/*GESTION PARAMETRES */
void ajouter_parametre(int type_du_parametre , int profondeur) ;

//renvoie un tableau avec  le nombre de parametre de la fonction et reinitialise ce tableau
void  getTab_parametres(parametres * t ) ;

void print_TABLE_DES_FONCTION() ;

void printTableParametres() ;
int getNombredeParametres() ;
void initNombreDeParametres() ;



void setIDprototype(char * name ); 
char * getIDprototype() ;

void setTypeRetour(int type_retour); 
int getTypeRetour(); 
/*===============================END FONCTIONS================================================*/

#endif