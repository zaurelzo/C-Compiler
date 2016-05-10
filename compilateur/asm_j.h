#ifndef __ASM_J_
#define __ASM_J_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tab_symb.h"
#include "tab_label.h"

int declarer_pointeur(char * nom_variable, int niveauPtr);

int init_pointeur_expression_pointeur(int dollar_addr, int dollar_relative_ou_absolue,int type_de_la_var);

int init_pointeur_dereferencement(char * nom_deref);

int init_pointeur_AccesCase();

int init_pointeur_pointeur(char * nom_ptr);

void init_pointeur_Null();

int expression_pointeur(char * nom_ptr, int * relative_ou_absolue ,int * typage_var );

int dereferencement(char * nomptr);

int declaration_tab(char * nom_tab);

void deb_init_tab();

int fin_init_tab();

int deb_elts(int dim);

int fin_elts();

int deb_acces_case(char * nom_tab, int num_case);

int fin_accesCase(char * nom_tab, int * abs_rel, int * typage_var);

int debut_accesCase_id(char * nom_var);

int fin_accesCase_id(char * nom_var, int dim, int * abs_rel, int * typage_var);

int deb_AccesLettre(char * nom_tab, char * nom_lettre);

int fin_AccesLettre(char * nom_tab, int * abs_rel, int * typage_var);

int deb_suite_acces(int dim );

void fin_suite_acces(char * nom_lettre);

int deb_affectation_tab();

int fin_affectation_tab(int addr_case_tab, int abs_rel, int addr_var, int abs_rel2);

int dereferencement_expr(char * nom, int * abs_rel, int * typage_var);

int expression_ptr_expr(int addr);

int verification(int addr, int abs_rel);

int verif_niv_ptr(int addr_1, int addr_2, int addr1_rel , int addr2_rel);

int affectation_gauche_id_tab_ptrNonDeref(char * dollar_1, int addr_dollar_3, int addr_dollar_3_rel);

int affectation_Deref (char * dollar_1, int addr_dollar_3, int addr_dollar_3_rel);

#endif 
