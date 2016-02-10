%{
#include <stdio.h>
#include <stdlib.h>
#include "tab_symb.h"


%}



%union  {
	int integer ;
	char  string[300] ; 
}



%error-verbose


%token tPRINT tMAIN  tCONST  tINT  tPO  tPF  tAO tAF  tMUL tDIV tEGAL tADD tSUB tVIR tPOINTVIR tNOMBREEXPO tERREUR

%token <integer> tNOMBREDEC
%token <string> tID 

%start Input
%%
 
Input: Main;



Declaration : tINT tID { 
							if ( ajouter_Var($2,1,0,0)==-1 )  
							{
								yyerror("ERROR ") ;
							} else
							{	
								//printTab();
							} 
						}  SuiteDeclarations  

			|tCONST tINT tID tEGAL Valeur tPOINTVIR { 
														if (ajouter_Var($3,1,0,1)==-1)
														{
															yyerror("ERROR ") ;
														}else 
														{
															printf( "AFC @%d %d \n",recherche($3), yylval.integer);
														}
													} ; 



SuiteDeclarations :  tPOINTVIR
					| tVIR tID SuiteDeclarations  { 
											if ( ajouter_Var($2,1,0,0)==-1 )  
											{
												yyerror("ERROR ") ;
											} else
											{	
												//printTab();
											} 
										};
									

							
Main: tMAIN tPO  tPF Body ;

Body : tAO SuiteBody tAF ;
			
SuiteBody :  Declaration SuiteBody
			|Affectation  SuiteBody
			|Print SuiteBody 
			|;
			
Affectation : tID tEGAL Valeur tPOINTVIR { int var ;
											if ((  var =recherche($1))==-1)
											{
															yyerror("ERROR \n") ;
											}else 
											{
												printf( "AFC @%d %d \n",var, yylval.integer);												
											}
										};

Valeur : tNOMBREEXPO
		|tNOMBREDEC ;

Print : tPRINT tPO Valeur tPF tPOINTVIR {
											printf("AFC @%d %d\n",ADRESSE_TEMPORAIRE, yylval.integer);
											printf("PRI @%d\n",ADRESSE_TEMPORAIRE);
										}
		|  tPRINT tPO tID tPF tPOINTVIR {int var ;
											if ((  var =recherche($3))==-1)
											{
															yyerror("ERROR \n") ;
											}
											else 
											{
												printf("PRI @%d\n", var);
											}
									} ; 
			
%%


int yyerror(char *s) {
  printf("%s\n",s);
}

int main(void) {
  yyparse();
}
