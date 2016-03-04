#ifndef __TAB_LABEL__
#define __TAB_LABEL__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE 30
#define TAILLE_TAB_LABEL 1000
#define ADRESSE_DEBUT_TABLABEL TAILLE_TAB_LABEL-1

extern int indiceLabel;
extern int indice; 
extern int pc ; 


struct Noeud_label {
	char nomLabel[TAILLE];
	int num_instruction; 
};


typedef struct Noeud_label  Noeud_label ; 

Noeud_label  TABLEAU[TAILLE_TAB_LABEL] ; 
Noeud_label TAB_FILE_DES_LABELS[TAILLE_TAB_LABEL] ;

char* ajouter_label();
int modifierNum_instruction(char * nom_label,int val_pc);
void afficherTab();
void incrementerPC();
void empilerLabel(char * label); 
char * depilerLabel();


/*ecrit la table des labels dans un fichier*/
void generer_fichier_tab_label();
#endif