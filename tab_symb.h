#ifndef __TAB_SYMBOL__
#define __TAB_SYMBOL__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TAILLE 30

#define TAILLE_TAB_SYMBOL 1000

#define ADRESSE_TEMPORAIRE TAILLE_TAB_SYMBOL-1

extern int ind ; 

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

void printTab(); 


#endif
