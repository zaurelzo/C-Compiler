%{
#include <stdlib.h>
#include <stdio.h>
#include "y.tab.h"
%}

%%



"print" return tPRINT ;  

"main" return tMAIN;

"const" return tCONST;

"int"  { return tINT ;}

"return" {return tRETURN;}

"if" {return tIF;}

"while" {return tWHILE;}

"else" {return tELSE;}

"&" {return tETCOM;}

"NULL" {return tNULL;}

"==" {strcpy(yylval.comparateur,yytext);return tEGALCOMP;}

"<" {strcpy(yylval.comparateur,yytext);return tINF;}

">" {strcpy(yylval.comparateur,yytext);return tSUP;}

"and" {return tAND;}

"or" {return tOR;}

"("  return tPO;

")"  return tPF;

"{" return tAO;

"}" return tAF;

"[" return tCO;

"]" return tCF;

[\*] return tMUL;

[\/] return tDIV;

[\=] return tEGAL;

[\+] return tADD;

[\-] return tSUB;

[a-z][a-zA-Z0-9_]* {strcpy(yylval.string,yytext);return tID; }

[0-9]+ {yylval.integer=atoi(yytext);  return tNOMBREDEC  ; }



"," return tVIR;

[\t \n] {}

";" return tPOINTVIR ;

[0-9]+"."[0-9]+ return tNOMBREEXPO;

.	return tERREUR;




