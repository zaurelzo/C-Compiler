#ifndef __INTERPRETEUR__
#define __INTERPRETEUR__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../tab_label.h"
#include "../tab_symb.h"


#define TAILLE_MEM_DATA TAILLE_TAB_SYMBOL
#define TAILLE_RAM 5000

//extern int PC ;
extern int nb_Instruction_Programme; 

struct Noeud_memoire_donnes
{
	int valeur ; 
};

typedef struct Noeud_memoire_donnes Noeud_memoire_donnes ;

Noeud_memoire_donnes Tableau_Memoire_des_donnes[TAILLE_MEM_DATA] ;





struct Ram 
{
	int format_instruction; //indique le nombre total de nombres 
	int code_operation ;
	int result;
	int operande1;
	int operande2; 
};

typedef struct Ram Ram ; 

Ram Tableau_Ram[TAILLE_RAM] ;


void charger_Programme_dans_Ram(char * name_fichier);
void afficher_ram();

/*permmet d'analyser la chaine pour en extraire les nombres 
@par1 : chaine à traiter 
@par: tableau pour stocker les nombres reucperer
@return : le nombre d'"entier que contient la chaine "*/
int traiter_ligne(char * chaine , int *tab_retour);


void interpreter();
#endif