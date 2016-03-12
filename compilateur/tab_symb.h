#ifndef __TAB_SYMBOL__
#define __TAB_SYMBOL__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TAILLE 30

#define TAILLE_TAB_SYMBOL 1000

#define ADRESSE_DEBUTPILE TAILLE_TAB_SYMBOL-1

extern int ind ; 

extern int indPile;
extern int profondeur ; 

struct Noeud_symbol {
	char nom_var[TAILLE];
	int type;
	int initialiser;
	int constante ; 
	//int global ; // 0 global , 1 pas global 
};

typedef struct  Noeud_symbol   Noeud_symbol ; 

 Noeud_symbol  Array[TAILLE_TAB_SYMBOL] ; 


/*TABLEAU POUR GERER LA LOCALITÉ DES VARIABLES ET LES VARIABLES DES FONCTIONS*/
int TAB_PROFONDEUR[TAILLE_TAB_SYMBOL] ; 

/* type : 1 pour entier 
 initialiser : 0 pas init  1 init 
constante = 0 pas constante , 1 si contante 
@retour : 0 si ok , -1 si probleme */ 
int ajouter_Var(char *nom_var,int type ,int initialiser ,int constante ) ;


int recherche(char * nom_var ) ; 



/*lorsque l'on empile , la valeur est convertit en string , et stocké dans le champ nom_var
La base de la pile en la fin de la table des symbol

 type : 0 : la valeur empilé est une constante ,
	1: la valeur empilé est l'@ d'une varible qui est existe ! 
	@retour : l'@ ou on a empilé 
	*/ 
int empiler(int value , int type  ); 




/*par1: type  de la valeur depilée , 0 :constante , 1 : @ d'une var existante 
@retour : valeur depile , -1 si l'on ne peut pas dépiler(ie : on est début de la pile ) */
int depiler(int *  type_val_depile ) ;


void printTabVar(); 



int obtenirAdressePremierOperande();

int obtenirAdressDeuxiemeOperande();


/*permet d'indiquer que la var est maintenant initialiser */
void modifierChampInitialiserVariable(char * nom_var); 

int getAdressePile();

void viderPile();

/*int getAdressePremierOperandeCondition();

int getAdresseDeuxiemeOperandeCondition();

int getAdresseResultatComparaison();*/

void generer_fichier_table_des_symboles();

/*fonction pour la gestion de la profondeur */
void empilerProfondeur();
int depilerProfondeur(); 



#endif
