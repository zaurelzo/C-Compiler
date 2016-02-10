%{
#include <stdlib.h>
#include <stdio.h>

%}


%%



"main" {printf("programme principal\n");}

"const" {printf("constante\n");}

"int" {printf("int\n");}

"("  {printf("paranthèse ouvrante\n");}

")"  {printf("paranthèse fermante\n");}

"{" {printf("accolade ouvrante\n");}

"}" {printf("accolade fermante\n");}

[a-z][a-zA-Z0-9_]*  {printf("nom de fonction ou nom variable\n");}

[0-9]+  {printf("nombre forme décimale\n");}

[\*] {printf("expression algébrique multiplication\n");}

[\/] {printf("expression algébrique division\n");}

[\=] {printf("expression algébrique égal\n");}

[\+] {printf("expression algébrique addition \n");}

[\-] {printf("expression algébrique soustraction\n");}

"," {printf("virgule\n");}

[\t \n] ;

";" {printf("point-virgule\n");}

[0-9]+"."[0-9]+ {printf("nombre forme exponentielle\n");}

"." ;

"_" {}

[^a-zA_Z0-9.,;\+\-\*\=\/{}()]	{printf("caractère non_autorisé\n");}

