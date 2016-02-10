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

"("  return tPO;

")"  return tPF;

"{" return tAO;

"}" return tAF;

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




