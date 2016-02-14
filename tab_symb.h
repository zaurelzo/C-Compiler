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

struct Noeud {
	char nom_var[TAILLE];
	int type;
	int initialiser;
	int constante ; 
};

typedef struct Noeud  Noeud ; 

Noeud  Array[TAILLE_TAB_SYMBOL] ; 


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

/*cette void fonctionne est neccessaire pour le bon maintient de la pile
: étant donné que lorsque l'on dépile deux arg pour faire une opération arithmétique ,on
empile aucun résultat (ie on genere juste le code de l'opération , il est nécessaire de similuer l'empiler 
d'un résultat : sinon on aura des probles de gestions des adresses de la pile )*/
void empilerVide() ;

/*permet d'indiquer que la var est maintenant initialiser */
void modifierChampInitialiserVariable(char * nom_var); 


 
#endif
