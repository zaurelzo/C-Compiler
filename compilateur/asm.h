#ifndef __ASM__
#define __ASM__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "color_constantes.h" 
#include "gestion_des_fonctions.h"



int declaration_asm(char * nom_var,int type_de_la_var) ;

int declaration_affectation_asm( char * nom_var,int constante ,int dollar_relative_ou_absolue , int dollar_addr,int type_de_la_var);

int affection_asm( char * nom_var ,int dollar_relative_ou_absolue , int dollar_addr) ;

void print_asm( int dollar_addr , int dollar_relative_ou_absolue ) ;

int operation_arithmetique_asm(char * operation , int * dollar_addr, int * dollar_relative_ou_absolue, int * dollar_typage_result );

void nombre_negatif_asm(int * dollar_addr,int * dollar_relative_ou_absolue,int * dollar_typage_result);

void JMF_IF_ASM(int addr_condition);

void JMP_IF_ASM() ;

void ELSE_FIRST_LABEL_ASM();

void ELSE_SECOND_LABEL_ASM();

void IF_WITHOUT_ELSE_ASM() ;

void CONDITION_ASM(char * comparateur, int * dollar_addr , int * dollar_relative_ou_absolue, int * dollar_typage_result ,int addr_exp1,int addr_exp2, int abs_rel1, int abs_rel2 );

void WHILE_FIRST_LABEL_ASM();

void JMF_WHILE_ASM(int dollar_addr);

void JMP_WHILE_ASM();

void WHILE_SECOND_LABEL_ASM() ;

int  ADD_PROTOTYPE_ASM() ;

int ADD_IMPLEMENTATION_FUNCTION_ASM(int addr_implementation);

void PUSH_PC_ASM();

void CALL_PARAMETERS_ASM(int dollar_addr , int dollar_relative_ou_absolue,int first_push); 

int  VERIFICATION_AND_CALL_GENERATION_ASM();

void RETURN_ASM (int dollar_addr, int dollar_relative_ou_absolue,int dollar_typage_result) ;
//void APPEL_FONCTION_IN_EXPRESSION_ASM();

void PushResultFunction();
void CALL_PARAMETERS_ASM_WITHOUT_AARGUMENT() ;

#endif 
