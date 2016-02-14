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
																												printf( "COP @%d @%d \n",recherche($3), $5);
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
																										printf( "COP @%d @%d \n",var, $3);												
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
																			//printf("type 1 : %d | type 2 : %d\n",typeOp1,typeOp2);
																			if (valeurOp2==-1 || valeurOp1==-1 )
																			{
																				//printf("ERREUR indicePile :%d\n",indPile );
																				yyerror("ERREUR LORS DU DÉPILEMENT DES OPÉRANDES \n") ;
																			}else 
																			{
																						if (typeOp1==1 && typeOp2==1)//si 1er et 2iem opérande est une variable 
																						{
																						//printf("pff++\n");
																							$$= valeurOp2 ; 
																							printf("ADD @%d @%d @%d\n",valeurOp2,valeurOp2,valeurOp1);
																							empilerVide() ;
																						}else if( typeOp1==1 && typeOp2==0) //si 1er est une var et 2iem est une constante
																						{
																							//printf("pff---\n");
																							$$= valeurOp2 ; 
																							int NouvAdrOp2= obtenirAdressePremierOperande();

																							printf("ADD @%d @%d @%d\n",valeurOp2,valeurOp2,NouvAdrOp2);
																							empilerVide() ;
																						}else if( typeOp1==0 && typeOp2==1) //si 1er est une constante et 2iem est une var
																						{
																							//printf("pff//\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 
																							$$=obtenirAdressePremierOperande();
																							printf("ADD @%d @%d @%d\n",NouvAdrOp1,NouvAdrOp1,valeurOp1);
																							empilerVide() ;
																						}else if( typeOp1==0 && typeOp2==0) //si 1er est une constante et 2iem est une constante
																						{
																							//printf("pff\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 	
																							$$=NouvAdrOp1;
																							int NouvAdrOp2 = obtenirAdressDeuxiemeOperande();
																							printf("ADD @%d @%d @%d\n",NouvAdrOp1,NouvAdrOp1,NouvAdrOp2);
																							empilerVide() ;
																						}
																			}
  														}
 
 
 
  | Expression tSUB Expression {
																			int typeOp1,typeOp2; 
																			int valeurOp2 = depiler(&typeOp2);
																			int valeurOp1= depiler(&typeOp1);
																			//printf("type 1 : %d | type 2 : %d\n",typeOp1,typeOp2);
																			if (valeurOp2==-1 || valeurOp1==-1 )
																			{
																				//printf("ERREUR indicePile :%d\n",indPile );
																				yyerror("ERREUR LORS DU DÉPILEMENT DES OPÉRANDES \n") ;
																			}else 
																			{
																						if (typeOp1==1 && typeOp2==1)//si 1er et 2iem opérande est une variable 
																						{
																						//printf("pff++\n");
																							$$= valeurOp2 ; 
																							printf("SOU @%d @%d @%d\n",valeurOp2,valeurOp2,valeurOp1);
																							empilerVide() ;
																						}else if( typeOp1==1 && typeOp2==0) //si 1er est une var et 2iem est une constante
																						{
																							//printf("pff---\n");
																							$$= valeurOp2 ; 
																							int NouvAdrOp2= obtenirAdressePremierOperande();

																							printf("SOU @%d @%d @%d\n",valeurOp2,valeurOp2,NouvAdrOp2);
																							empilerVide() ;
																						}else if( typeOp1==0 && typeOp2==1) //si 1er est une constante et 2iem est une var
																						{
																							//printf("pff//\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 
																							$$=obtenirAdressePremierOperande();
																							printf("SOU @%d @%d @%d\n",NouvAdrOp1,NouvAdrOp1,valeurOp1);
																							empilerVide() ;
																						}else if( typeOp1==0 && typeOp2==0) //si 1er est une constante et 2iem est une constante
																						{
																							//printf("pff\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 	
																							$$=NouvAdrOp1;
																							int NouvAdrOp2 = obtenirAdressDeuxiemeOperande();
																							printf("SOU @%d @%d @%d\n",NouvAdrOp1,NouvAdrOp1,NouvAdrOp2);
																							empilerVide() ;
																						}
																			}
  														}
  
  
  | Expression tMUL Expression {
																			int typeOp1,typeOp2; 
																			int valeurOp2 = depiler(&typeOp2);
																			int valeurOp1= depiler(&typeOp1);
																			//printf("type 1 : %d | type 2 : %d\n",typeOp1,typeOp2);
																			if (valeurOp2==-1 || valeurOp1==-1 )
																			{
																				//printf("ERREUR indicePile :%d\n",indPile );
																				yyerror("ERREUR LORS DU DÉPILEMENT DES OPÉRANDES \n") ;
																			}else 
																			{
																						if (typeOp1==1 && typeOp2==1)//si 1er et 2iem opérande est une variable 
																						{
																						//printf("pff++\n");
																							$$= valeurOp2 ; 
																							printf("MUL @%d @%d @%d\n",valeurOp2,valeurOp2,valeurOp1);
																							empilerVide() ;
																						}else if( typeOp1==1 && typeOp2==0) //si 1er est une var et 2iem est une constante
																						{
																							//printf("pff---\n");
																							$$= valeurOp2 ; 
																							int NouvAdrOp2= obtenirAdressDeuxiemeOperande();

																							printf("MUL @%d @%d @%d\n",valeurOp2,valeurOp2,NouvAdrOp2);
																							empilerVide() ;
																						}else if( typeOp1==0 && typeOp2==1) //si 1er est une constante et 2iem est une var
																						{
																							//printf("pff//\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 
																							$$=obtenirAdressePremierOperande();
																							printf("MUL @%d @%d @%d\n",NouvAdrOp1,NouvAdrOp1,valeurOp1);
																							empilerVide() ;
																						}else if( typeOp1==0 && typeOp2==0) //si 1er est une constante et 2iem est une constante
																						{
																							//printf("pff\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 	
																							$$=NouvAdrOp1;
																							int NouvAdrOp2 = obtenirAdressDeuxiemeOperande();
																							printf("MUL @%d @%d @%d\n",NouvAdrOp1,NouvAdrOp1,NouvAdrOp2);
																							empilerVide() ;
																						}
																			}
  														}
  
  															
  															
  | Expression tDIV Expression {
																			int typeOp1,typeOp2; 
																			int valeurOp2 = depiler(&typeOp2);
																			int valeurOp1= depiler(&typeOp1);
																			//printf("type 1 : %d | type 2 : %d\n",typeOp1,typeOp2);
																			if (valeurOp2==-1 || valeurOp1==-1 )
																			{
																				//printf("ERREUR indicePile :%d\n",indPile );
																				yyerror("ERREUR LORS DU DÉPILEMENT DES OPÉRANDES \n") ;
																			}else 
																			{
																						if (typeOp1==1 && typeOp2==1)//si 1er et 2iem opérande est une variable 
																						{
																						//printf("pff++\n");
																							$$= valeurOp2 ; 
																							printf("DIV @%d @%d @%d\n",valeurOp2,valeurOp2,valeurOp1);
																							empilerVide() ;
																						}else if( typeOp1==1 && typeOp2==0) //si 1er est une var et 2iem est une constante
																						{
																							//printf("pff---\n");
																							$$= valeurOp2 ; 
																							int NouvAdrOp2= obtenirAdressDeuxiemeOperande();

																							printf("DIV @%d @%d @%d\n",valeurOp2,valeurOp2,NouvAdrOp2);
																							empilerVide() ;
																						}else if( typeOp1==0 && typeOp2==1) //si 1er est une constante et 2iem est une var
																						{
																							//printf("pff//\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 
																							$$=obtenirAdressePremierOperande();
																							printf("DIV @%d @%d @%d\n",NouvAdrOp1,NouvAdrOp1,valeurOp1);
																							empilerVide() ;
																						}else if( typeOp1==0 && typeOp2==0) //si 1er est une constante et 2iem est une constante
																						{
																							//printf("pff\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 	
																							$$=NouvAdrOp1;
																							int NouvAdrOp2 = obtenirAdressDeuxiemeOperande();
																							printf("DIV @%d @%d @%d\n",NouvAdrOp1,NouvAdrOp1,NouvAdrOp2);
																							empilerVide() ;
																						}
																			}
  														}
  
  
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
