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

			|tCONST tINT tID tEGAL Expression tPOINTVIR  { //pas complet , changer champ initialiser var
																											if (ajouter_Var($3,1,0,1)==-1)
																											{
																												yyerror("ERROR ") ;
																											}else 
																											{
																												printf( "COP @%d @%d \n",recherche($3), $5);
																												 viderPile();
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
																										 viderPile();												
																									}
																						} ;
										
										

/*Valeur : tNOMBREEXPO
		|tNOMBREDEC ;*/
		
		
//à completer
Print : tPRINT tPO Expression tPF tPOINTVIR {
																							printf("PRI @%d\n",$3);
																							viderPile();
																						} ; 
									
									
Expression : tNOMBREDEC { 
													//$$=$1;
													printf("AFC @%d %d\n",empiler($1,0) ,$1);
													$$=getAdressePile()+1;	
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
						$$= addr;
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
																							$$= getAdressePile();	
																							printf("ADD @%d @%d @%d\n",getAdressePile(),valeurOp2,valeurOp1);
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==1 && typeOp2==0) //si 1er est une var et 2iem est une constante
																						{
																							//printf("pff---\n");
																							$$= getAdressePile() ; 
																							int NouvAdrOp2= obtenirAdressDeuxiemeOperande();
																							printf("ADD @%d @%d @%d\n",getAdressePile(),valeurOp1,NouvAdrOp2);
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==0 && typeOp2==1) //si 1er est une constante et 2iem est une var
																						{
																							//printf("pff//\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 
																							$$=getAdressePile();
																							printf("ADD @%d @%d @%d\n",getAdressePile(),NouvAdrOp1,valeurOp2);
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==0 && typeOp2==0) //si 1er est une constante et 2iem est une constante
																						{
																							//printf("pff\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 	
																							$$=getAdressePile();
																							int NouvAdrOp2 = obtenirAdressDeuxiemeOperande();
																							printf("ADD @%d @%d @%d\n",getAdressePile(),NouvAdrOp1,NouvAdrOp2);
																							empiler(getAdressePile(),1) ;
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
																							$$= getAdressePile();	
																							printf("SOU @%d @%d @%d\n",getAdressePile(),valeurOp2,valeurOp1);
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==1 && typeOp2==0) //si 1er est une var et 2iem est une constante
																						{
																							//printf("pff---\n");
																							$$= getAdressePile() ; 
																							int NouvAdrOp2= obtenirAdressDeuxiemeOperande();
																							printf("SOU @%d @%d @%d\n",getAdressePile(),valeurOp1,NouvAdrOp2);
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==0 && typeOp2==1) //si 1er est une constante et 2iem est une var
																						{
																							//printf("pff//\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 
																							$$=getAdressePile();
																							printf("SOU @%d @%d @%d\n",getAdressePile(),NouvAdrOp1,valeurOp2);
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==0 && typeOp2==0) //si 1er est une constante et 2iem est une constante
																						{
																							//printf("pff\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 	
																							$$=getAdressePile();
																							int NouvAdrOp2 = obtenirAdressDeuxiemeOperande();
																							printf("SOU @%d @%d @%d\n",getAdressePile(),NouvAdrOp1,NouvAdrOp2);
																							empiler(getAdressePile(),1) ;
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
																							$$= getAdressePile();	
																							printf("MUL @%d @%d @%d\n",getAdressePile(),valeurOp2,valeurOp1);
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==1 && typeOp2==0) //si 1er est une var et 2iem est une constante
																						{
																							//printf("pff---\n");
																							$$= getAdressePile() ; 
																							int NouvAdrOp2= obtenirAdressDeuxiemeOperande();
																							printf("MUL @%d @%d @%d\n",getAdressePile(),valeurOp1,NouvAdrOp2);
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==0 && typeOp2==1) //si 1er est une constante et 2iem est une var
																						{
																							//printf("pff//\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 
																							$$=getAdressePile();
																							printf("MUL @%d @%d @%d\n",getAdressePile(),NouvAdrOp1,valeurOp2);
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==0 && typeOp2==0) //si 1er est une constante et 2iem est une constante
																						{
																							//printf("pff\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 	
																							$$=getAdressePile();
																							int NouvAdrOp2 = obtenirAdressDeuxiemeOperande();
																							printf("MUL @%d @%d @%d\n",getAdressePile(),NouvAdrOp1,NouvAdrOp2);
																							empiler(getAdressePile(),1) ;
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
																							$$= getAdressePile();	
																							printf("DIV @%d @%d @%d\n",getAdressePile(),valeurOp2,valeurOp1);
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==1 && typeOp2==0) //si 1er est une var et 2iem est une constante
																						{
																							//printf("pff---\n");
																							$$= getAdressePile() ; 
																							int NouvAdrOp2= obtenirAdressDeuxiemeOperande();
																							printf("DIV @%d @%d @%d\n",getAdressePile(),valeurOp1,NouvAdrOp2);
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==0 && typeOp2==1) //si 1er est une constante et 2iem est une var
																						{
																							//printf("pff//\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 
																							$$=getAdressePile();
																							printf("DIV @%d @%d @%d\n",getAdressePile(),NouvAdrOp1,valeurOp2);
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==0 && typeOp2==0) //si 1er est une constante et 2iem est une constante
																						{
																							//printf("pff\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 	
																							$$=getAdressePile();
																							int NouvAdrOp2 = obtenirAdressDeuxiemeOperande();
																							printf("DIV @%d @%d @%d\n",getAdressePile(),NouvAdrOp1,NouvAdrOp2);
																							empiler(getAdressePile(),1) ;
																						}
																			}
  														}
  
  
  | tSUB Expression %prec NEG  {	
  																int typeOp;
  																int valeurOp = depiler(&typeOp);
  																if (typeOp==0)//si constante
  																{
  																	printf("AFC @%d -%d\n",empiler(-valeurOp,0),valeurOp);
  																}
  																$$=getAdressePile()+1;
  																//empiler(-valeurOp,0);	
  															}
  | tPO Expression tPF   {
  												 $$ = $2;  
  												} ; 
			
%%


int yyerror(char *s) {
  printf("%s\n",s);
}

int main(void) {
  yyparse();
}
