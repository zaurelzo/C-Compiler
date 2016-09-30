#ifndef __TAB_SYMBOL__
#define __TAB_SYMBOL__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TAILLE 30

#define TAILLE_TAB_SYMBOL 1001

#define ADRESSE_DEBUTPILE TAILLE_TAB_SYMBOL-3 //998 , 999 étant le registre de retour des fonctions
#define Registre_retour_fonctions TAILLE_TAB_SYMBOL-2

#define TAILLE_TAB_ELEMENTS 800

#define NULL_2 TAILLE_TAB_SYMBOL-1

#define TAILLE_NOM_POINTEUR 100

/*==============================VARIABLES *===========================*/ 

extern int ind ; 

extern int declaGlobale;

extern int indPile;
extern int profondeur ;
extern int nombreDeVariabeleglobale  ;


extern int lettre_acces_1_bis;
extern int lettre_acces_2_bis;
extern int nivPointeur;

extern int nivPointeur_2;

extern char nom_pointeur[TAILLE_NOM_POINTEUR]; 

extern int nb_elements_tab;

extern int tab_elements[TAILLE_TAB_ELEMENTS];

extern int indice_tableau;

extern int size_tab;

extern int tabDimension[TAILLE];

extern int indiceDim;


extern int tab_AO_deb[TAILLE];

extern int tab_AO_fin[TAILLE];

extern int nb_elements_Tot;

extern char nomTableau[50]; 

extern int tabAcces[TAILLE];

extern int adresse_retour;

extern int niv_tab;

extern int nb_AO;

extern int nivPointeur_3;

extern int deb;

extern int presence_lettre;

extern int lettre_acces;

extern int adresse_pointee_bis;

extern int avt_nb;


extern int lettre_acces_2;


struct Noeud_symbol {
	char nom_var[TAILLE];
	int type; //typage variable
	int initialiser;
	int constante ; 
	int abs_rel;
	int type_push ; //sert juste à savoir si la valeur empilé est une adresse ou une valeur
	//int global ; // 0 global , 1 pas global
	int pointeur; //0 pas un pointeur, 1 c'est un pointeur
	int nivPtr; 
	int tableau;
	int tabDim[TAILLE];
	int adresse_deb_tab;
	int adresse_pointee;//sans doute pas à la compilation
 
};

typedef struct  Noeud_symbol   Noeud_symbol ; 

 Noeud_symbol  Array[TAILLE_TAB_SYMBOL] ; 

/*TABLEAU POUR GERER LA LOCALITÉ DES VARIABLES ET LES VARIABLES DES FONCTIONS*/
int TAB_PROFONDEUR[TAILLE_TAB_SYMBOL] ; 


/*GENERATION D'ADRESSE POUR LES FONCTIONS*/

extern char mode[TAILLE];
extern int taille_type_retour_fonction ; 

/*====================================END VARIABLES ================================*/




/*====================================FONCTIONS ========================================*/

/* type : 1 pour entier 
 initialiser : 0 pas init  1 init 
constante = 0 pas constante , 1 si contante 
@retour : 0 si ok , -1 si probleme */ 
int ajouter_Var(char *nom_var,int type ,int initialiser ,int constante ) ;

int ajouter_Var_spec(char *nom_var,int type ,int initialiser ,int constante, int pointeur, int nivPtr, int tableau) ;


int recherchet(char * nom_var , int * relative_ou_absolue ,int * typage_var  ) ;

int recherchet_spec(char * nom_var , int * relative_ou_absolue ,int * typage_var  ) ;

 int recherche(char * nom_var); // pour adresse pointée 

/*lorsque l'on empile , la valeur est convertit en string , et stocké dans le champ nom_var
La base de la pile en la fin de la table des symbol

 type : 0 : la valeur empilé est une constante ,
	1: la valeur empilé est l'@ d'une variale qui est existe ! 
	@retour : l'@ ou on a empilé 
	*/ 
int empiler(int value , int type  ); //pour debug à effacer apres 


int  empilert(int value ,int type ,int abs_ou_rel,int typage_var ); 

/*par1: type  de la valeur depilée , 0 :constante , 1 : @ d'une var existante 
@retour : valeur depile , -1 si l'on ne peut pas dépiler(ie : on est début de la pile ) */
int depiler(int *  type_val_depile ) ;

int depilert(int *  type_val_depile , int *  abs_ou_rel ,int * type_var ) ;

void printTabVar(); 



int obtenirAdressePremierOperande();

int obtenirAdressDeuxiemeOperande();

void incrementerIndicePile();

/*permet d'indiquer que la var est maintenant initialiser */
void modifierChampInitialiserVariable(char * nom_var); 

int getAdressePile();

void viderPile();

/*int getAdressePremierOperandeCondition();

int getAdresseDeuxiemeOperandeCondition();

int getAdresseResultatComparaison();*/

void generer_fichier_table_des_symboles();

/*fonction pour la gestion de la profondeur */
/*void empilerProfondeur();
int depilerProfondeur(); */

void setNombredevariableglobale();
int getNombredevariableglobale();
void setIndiceTableLable();

int getAdresseRetour(int nombres_de_casse_a_reserver) ; 




/*pour les fonctions*/
void changeMode();
char * getMode();

void initParametreForNewfunction() ;

void setTailleTypeRetourFonction(int taille) ;
int getTailleTypeRetourFonction();

int getNombreDeVariableLocales();



/*void IncrementeNbVariableLocalDeClare() ;
int getNbVariableLocalDeClare() ; 

void IncrementeNBParametre();
int getNbParametre() ;*/

void WriteDebugInformationAboutFunction(int global_or_local);
/*===============================END FONCTIONS================================================*/

void empilerProfondeur();
int depilerProfondeur(); 

void incrementer_niveau_pointeur();

void decrementer_niveau_pointeur();

int get_nivPtr();

void reinitialiser_nivPointeur();

int get_nivPtr_var(int adresse);

int get_Pointeur(int adresse);

void deplacer_indice(int dep);

int get_indice();

void incrementer_nbelements();

void getTabDim();

int get_nbelements();

void reinitialiser_nbelements();

void taille_tableau(int taille);

void remplir_tableau(int element);

int get_tailleTab();

int get_element(int indice);

int recherche_tableau(char * nom);

void ajouterTabDimension(int dim);

void initialiserTabDimension();

int getTotalDim();

int getCaseTabDim(int j);

void decrementer_nbAO();

int get_nbAO();

void reinitialiser_nbAO();

int recherche_tableau_dans_mem();

void initialiser_tab_AO_deb();

void initialiser_tab_AO_fin(int addr);

void incrementer_tab_AO_deb(int adresse);

int get_indice_nb_elements(int addr);

int get_tab_AO_deb_element(int i);

int get_tab_AO_fin_element(int k);

void print_tabElements();

void print_tabDim_var(int addr);

int recuperer_nb_element_par_ligne(int addr);

int get_nbelements_Tot();



void incrementer_nb_elements_Tot();

void reinitialiser_nb_elements_Tot();

int test_tab_deb_fin_AO();

void print_tab_deb_AO();

void print_tab_fin_AO();

void maj_indiceDIm(int addr);

int get_indiceDIm();

char * getNomVar(int addr);


void initialiser_nom_tableau(char * nom);

char * get_nomTableau();

void init_tabAcces(int addr);

void reinit_tabAcces();

int get_val_tabAcces(int i);

int dimension(char * nom_tableau, int num_case);

void init_adresse_retour(int addr_courante);

void incrementer_adresse_retour(int nb);

int get_adresse_retour();

void reinit_adresse_retour();

void print_tab_Acces();

void reinitialiser_nivTab();

int get_niv_tab();



void init_nom_pointeur(char * nom);

void reinit_nom_pointeur();

char * get_nom_pointeur();

void init_niveau_pointeur_2(int niv);

int get_nivPtr_2();

void reinitialiser_nivPointeur_2();

void tout_NivPtr_zero();

int verifier_Acces_case_OP(int addr);

int verifier_Dereferencement_OP(int addr);

void modifier_champ_init_deref(int addr);

void reinit_champ_init_deref(int addr);

void init_champ_pointeur(int addr, int val);


void renit_champ_pointeur(int addr);

int get_adresse_tab(int addr);

int get_champ_tab(int addr);

void modi_adresse_deb_tab(int addr, int a);

int get_adresse_pointee(int addr);

void init_adresse_pointee(int addr, int a);

int get_adresse_pointee_bis();

void init_adresse_pointee_bis(int a);

void init_niveau_pointeur_3(int niv);

int get_nivPtr_3();

void reinitialiser_nivPointeur_3();

void initialiser_deb_nombre();

int get_deb();

void initialiser_presence_lettre();

int get_presence_lettre();

void reinit_presence_lettre();

int get_lettre_acces();

void reinit_lettre_acces();

void init_get_lettre_acces();

int get_avt_nb();

void reinit_avt_nb();

void init_avt_nb();

int get_lettre_acces_2();

void reinit_lettre_acces_2();

void init_get_lettre_acces_2();

void setDeclaGlobale();

int getDeclaGlobale();


void reinitDeclaGlobale();

int get_lettre_acces_1_bis();

void init_lettre_acces_1_bis();

void reinit_lettre_acces_1_bis();

int get_lettre_acces_2_bis();

void init_lettre_acces_2_bis();

void reinit_lettre_acces_2_bis();

void init_champ_tab(int addr);

void reinit_champ_tab(int addr);

int length_table_symbole();


#endif
