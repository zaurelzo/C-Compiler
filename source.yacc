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
													}/* else
													{	
														printTabVar();
													} */
												}  SuiteDeclarations  

			|tCONST tINT tID tEGAL Expression tPOINTVIR  { 
																											if (ajouter_Var($3,1,0,1)==-1)
																											{
																												yyerror("ERROR ") ;
																											}else 
																											{
																												printf( "AFC @%d %d \n",recherche($3), $5);
																											}
																									} ; 
										
SuiteDeclarations :  tPOINTVIR
					| tVIR tID SuiteDeclarations  { 
																					if ( ajouter_Var($2,1,0,0)==-1 )  
																					{
																						yyerror("ERROR ") ;
																					} /*else
																					{	
																						printTabVar();
																					} */
																			};
									

							
Main: tMAIN tPO  tPF Body ;

Body : tAO SuiteBody tAF ;
			
SuiteBody :  Declaration SuiteBody
			|Affectation  SuiteBody
			|Print SuiteBody 
			|;
			
Affectation : tID tEGAL  Expression tPOINTVIR { 
																									int var ;
																									if ((  var =recherche($1))==-1)
																									{
																											yyerror("ERROR \n") ;
																									}else 
																									{
																										printf( "AFC @%d %d \n",var, $3);												
																									}
																						} ;
										
										

/*Valeur : tNOMBREEXPO
		|tNOMBREDEC ;*/
		
		
//à completer
Print : tPRINT tPO Expression tPF tPOINTVIR {
																							printf("PRI @%d\n",$3);
																						}
																			
																			
		|  tPRINT tPO tID tPF tPOINTVIR {
																			int var ;
																			if ((  var =recherche($3))==-1)
																			{
																							yyerror("ERROR \n") ;
																			}
																			else 
																			{
																				printf("PRI @%d\n", var);
																			}
																} ; 
									
									
Expression : tNOMBREDEC { 
													//$$=$1;
													printf("AFC @%d %d\n",empiler($1,0) ,$1);
												}
												
	|tID {
					//$$=$1; 
					int addr ;
					if ((  addr =recherche($1))==-1)
					{
						yyerror("ERROR : Variable %s non existante \n",$1) ;
					}else 
					{
						
						printf("AFC @%d %d\n",empiler(addr,1) ,addr);
					} 
				}
				
  | Expression tADD Expression {
																	 int typeOp1,typeOp2; 
																	int valeurOp2 = depiler(&typeOp2);
																	int valeurOp1= depiler(&typeOp1);
																	
																	if (valeurOp2==-1 || valeurOp1==-1 )
																	{
																		yyerror("ERREUR LORS DU DÉPILEMENT DES OPÉRANDES  \n") ;
																	}else 
																	{
																				if (typeOp1==1 && typeOp2==1)//si 1er et 2iem opérande est une variable 
																				{
																					$$= valeurOp1 ; 
																					printf("ADD @%d @%d @%d\n",valeurOp1,valeurOp1,valeurOp2);
																				}else if( typeOp1==1 && typeOp2==0) //si 1er est une var et 2iem est une constante
																				{
																					$$= valeurOp1 ; 
																					int NouvAdrOp2=empiler(valeurOp2,0); // je réempile la valeur que je viens de dépiler
																					printf("AFC @%d %d\n",NouvAdrOp2,valeurOp2);
																					printf("ADD @%d @%d @%d\n",valeurOp1,valeurOp1,NouvAdrOp2);
																			
																				}else if( typeOp1==0 && typeOp2==1) //si 1er est une constante et 2iem est une var
																				{
																					int NouvAdrOp1=empiler(valeurOp1,0); // je réempile la valeur que je viens de dépiler
																					printf("AFC @%d %d\n",NouvAdrOp1,valeurOp1);
																					$$=NouvAdrOp1;
																					printf("ADD @%d @%d @%d\n",NouvAdrOp1,NouvAdrOp1,valeurOp2);
																			
																				}else if( typeOp1==0 && typeOp2==0) //si 1er est une constante et 2iem est une constante
																				{
																					int NouvAdrOp1=empiler(valeurOp1,0); // je réempile la valeur que je viens de dépiler
																					printf("AFC @%d %d\n",NouvAdrOp1,valeurOp1);
																					$$=NouvAdrOp1;
																					int NouvAdrOp2 = empiler(valeurOp2,0);
																					printf("AFC @%d %d\n",NouvAdrOp2,valeurOp2);
																					printf("ADD @%d @%d @%d\n",NouvAdrOp1,NouvAdrOp1,NouvAdrOp2);
																				}
  																}
  														}
 
 
 
  | Expression tSUB Expression { 
  															
  															}
  
  
  | Expression tMUL Expression {}
  															
  															
  | Expression tDIV Expression {  }
  
  
  | tSUB Expression %prec NEG  {	}
  | tPO Expression tPF   {//pas complet 
  												/* $$ = $2; */ } ; 
			
%%


int yyerror(char *s) {
  printf("%s\n",s);
}

int main(void) {
  yyparse();
}
