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
@retour : l'@ ou on a empilé */ 

int empiler(int value ); 


/*par1: adreese de la valeur depilée 
@retour : valeur depile */
int depiler(); 


void printTabVar(); 


/*permet d'empiler le resultat d'une opération entre deux operation directement
@retour : adresse ou l'on a empilé*/
int  empiler_avec_calcul(int op1 , int op2, char * nom_operation);

 
#endif
