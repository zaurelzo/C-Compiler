#!/bin/bash

yacc -dv source.yacc
flex source_avec_return.lex

gcc -g gestion_des_fonctions.c  asm_j.c asm.c y.tab.c tab_label.c  tab_symb.c lex.yy.c -ll -o executable
