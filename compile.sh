#!/bin/bash

yacc -d source.yacc
flex source_avec_return.lex

gcc  y.tab.c tab_symb.c lex.yy.c -ll -o executable
