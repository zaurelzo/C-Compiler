#ifndef __ASM__
#define __ASM__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "color_constantes.h" 




int declaration_affectation_asm( char * nom_var,int constante ,int dollar_relative_ou_absolue , int dollar_addr);
int affection_asm( char * nom_var ,int dollar_relative_ou_absolue , int dollar_addr) ;
int print_asm(int addr );
int operation_arithmetique_asm(char * operation , int * dollar_addr, int * dollar_relative_ou_absolue );
void  nombre_negatif_asm(int * dollar_addr,int * dollar_relative_ou_absolue) ;
#endif 