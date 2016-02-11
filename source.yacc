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

%left tADD tSUB
%left tDIV tMUL
%left NEG


%token <integer> tNOMBREDEC
%token <string> tID 

%type <integer> Expression

%start Input
%%
 
Input: Main;



Declaration : tINT tID { 
							if ( ajouter_Var($2,1,0,0)==-1 )  
							{
								yyerror("ERROR ") ;
							} else
							{	
								//printTabVar();
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
													} 
													
		|tCONST tINT tID tEGAL Expression tPOINTVIR ; //ICI


SuiteDeclarations :  tPOINTVIR
					| tVIR tID SuiteDeclarations  { 
											if ( ajouter_Var($2,1,0,0)==-1 )  
											{
												yyerror("ERROR ") ;
											} else
											{	
												//printTabVar();
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
															//on empile la valeur à afficher
															int adresse =empiler(yylval.integer);
															printf("AFC @%d %d\n",adresse, yylval.integer);
															printf("PRI @%d\n",adresse);
															
															depiler();//on depile la valeur 
															
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
									
									
Expression : tNOMBREDEC { $$=$1;
													/*int adresse =empiler(yylval.integer);
													printf("AFC @%d %d\n",adresse, yylval.integer); */
												}
  | Expression tADD Expression { 
  															/*int adr1 = empiler($1);
  															printf("AFC @%d %d\n",adr1,$1);
  															int adr2= empiler($3); 
  															printf("AFC @%d %d\n",adr2,$3);*/
  															//$$=$1+$3;
  														/*	depile();depile();
  															int adrResult = empiler($$);
  															printf("ADD @%d @%d @%d",adrResult,adr1,adr2);*/
  															
  															}
 
 
 
  | Expression tSUB Expression { /*$$ = $1 - $3; */}
  | Expression tMUL Expression { /*$$ = $1 * $3; */}
  | Expression tDIV Expression { /* $$ = $1 / $3;*/ }
  | tSUB Expression %prec NEG  {/* $$ = -$2; */}
  | tPO Expression tPF   { /* $$ = $2; */ } ; 
			
%%


int yyerror(char *s) {
  printf("%s\n",s);
}

int main(void) {
  yyparse();
}
