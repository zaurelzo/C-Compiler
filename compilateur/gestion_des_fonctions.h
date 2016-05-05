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
//extern int numeroParametresEncoursDAnalyse; 

struct parametres
{
	char nom[TAILLE_NOM_FONCTION ] ; // on en a besoin pour quand on fait les calculs en utilisant les parametres 
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
int type_retour; //type de la valeur retourné par la fonction appelé, par défaut, on dit que les fonctions renvoie un int 
extern int adresseDuReturn ; //s'il y'a un type return , on stocke l'adresse du resultat dans cette variable , cette adresse sera toujours absolue! 



/*construction des parametres d'appel*/
extern int indTableDesParametresDappel;
parametres Tab_parametresDappel[NOMBRES_PARAMATRES]; 
/*====================================END VARIABLES ================================*/




/*====================================FONCTIONS ========================================*/
//-1 si le prototype existe dejà
int ajouter_Prototype(char * nom_fonction , int type_retour, parametres * p , int nombres_paramatres) ;

//-1: si une fonction de même nom, et même parametres a déjà été déclarée
int ajouter_implementations(char * nom_fonction , int type_retour, parametres *  p ,int  adresse_implementation , int nombres_paramatres); 

/*-1 si la fonction n'est existe pas dans la table des fonctions */
int  fonction_ou_prototype_Existe(char * nom_fonction , parametres * p , int nombres_paramatres , int  type_retour ,int * prototype_ou_fonction);


/*GESTION PARAMETRES */
int ajouter_parametre(int type_du_parametre , int profondeur ,char * nom_du_parametre);
//renvoie un tableau avec  le nombre de parametre de la fonction et reinitialise ce tableau
void  getTab_parametres(parametres * t ) ;

void print_TABLE_DES_FONCTION() ;

void printTableParametres() ;
int getNombredeParametres() ;
void initNombreDeParametres() ;

void setIDprototypeOrImplementationFunction(char * name ); 
char * getIDprototypeOrImplementationFunction() ;


void setTypeRetour(int typeretour); 
int getTypeRetour(); 

void setAdresseDuReturn(int adresse); 
int getAdresseDuReturn (); 


//int getTypeDuReturn () ; 

/*GESTION PARAMETRES D'APPEL*/
void ajouter_parametreAPPEL(int type_du_parametre );
void  getTab_parametresAPPEL(parametres * t ) ;
int getNombredeParametresAPPEL();
void initNombreDeParametresAPPEL() ;
void printParametresDappel( parametres *  t,int nbParamAppel);


//renvoie -1 si le numero du parametre appelé n'est pas du bon type , -2 si la fonction a un prototype mais quelle n'est pas encore implémenté ,  sinon l'@ de l'implementation de la fonction
int checkAppelFonctionParametreConforme(char * nom_fonction , parametres * p , int nombreDeParametre, char * labelPotentiel) ;

void  creerLabel(int indiceDansTableDesFonctions , char * nom_label ) ;

void genererTableDesFonctions();
/*===============================END FONCTIONS================================================*/

#endif