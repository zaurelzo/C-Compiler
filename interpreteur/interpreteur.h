#ifndef __INTERPRETEUR__
#define __INTERPRETEUR__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../compilateur/tab_label.h"
#include "../compilateur/tab_symb.h"


#define TAILLE_MEM_DATA TAILLE_TAB_SYMBOL
#define TAILLE_RAM 5000
#define TAILLE_PILE_D_EXECUTION 512

extern int debutMain; //l'@ de début du main, que l'on pourra fixer à la lecture du fichier quand on aura les lables main:
extern int nb_Instruction_Programme;
extern int ebp ; //base pointer 
extern int esp ; //stack pointer 
extern int savEbp; // indique où se trouve ebp que l'on vient de save 

struct Noeud_memoire_donnes
{
	int valeur ; 
	
};

typedef struct Noeud_memoire_donnes Noeud_memoire_donnes ;

Noeud_memoire_donnes Tab_Mem_data[TAILLE_MEM_DATA] ;


struct Noeud_Piles_d_execution
{
	int valeur;
	int addrRetour; //1 si la valeur présente dans le noeud est pc ,sinon 0 
	int ebp ;// 1 si la valeur présente dans le noeud est ebp,sinon 0 
};

typedef struct Noeud_Piles_d_execution Noeud_Piles_d_execution; 

Noeud_Piles_d_execution PILE_D_EXECUTION[TAILLE_PILE_D_EXECUTION ];

struct Ram 
{
	int format_instruction; //indique le nombre total de nombres 
	char code_operation[10] ;
	int result;
	int abs_relResult;//0 :relative , 1 abs
	int operande1;
	int abs_relOP1;
	int operande2;
	int abs_relOP2;
};

typedef struct Ram Ram ; 

Ram Tableau_Ram[TAILLE_RAM] ;


struct DebugInfo
{
	char nom_fonction[100];
	char  tab_nom_var[50][60];//alloué dynamiquement à la lecture du fichier (50 variables locales de 60 caracteres )
	int nbLocales;
	int  adresse[50] ;//50 var locales 
};

typedef struct DebugInfo DebugInfo; 

DebugInfo tab_DebugInfo[50] ; //au maximum 50 fonctions , faut le faire quand meme 
extern int indTabDebugInfo; 


void charger_Programme_dans_Ram(const char * name_fichier);
void afficher_ram();

/*permmet d'analyser la chaine pour en extraire les nombres 
@par1 : chaine à traiter 
@par: tableau pour stocker les nombres reucperer
@return : le nombre d'"entier que contient la chaine "*/
int traiter_ligne(char * chaine , char ** tab_retour);


/*
	permet de calculer l'adresse ou il faut aller lire ou faire une opération 
	en fonction du faite que l'on est dans le contexte principal ou
	le contexte d'une fonction
	param1 : l'adresse qu'il faut manipuler pour obtenir la bonne adresse 
	Ret : l'@ calculée
	Rm : cette fonction n'est appelé que dans le cadre dans @ relatives 
*/
int calculIndice(int adresse);

/*
permet d'extraire le num d'une chaine de type @@nb oou @nb 
*/
int extractNumber(char * stringTOanalyse,int * absOrRelat);
void interpreter(int modeDebug);

void InitiPileExecution();

void chargerDebugInformation(); 
void printTabDugInfo();
void printInstructionCourante(int numInst);


int  printVarLocal(int contexte,char *  name ) ;
int  printVarGlobale(char * name ) ; 

void printPileContextCOurant();


#endif