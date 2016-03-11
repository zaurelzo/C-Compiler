%{
#include <stdio.h>
#include <stdlib.h>
#include "tab_symb.h"
#include "tab_label.h"

%}



%union  {
	int integer ;
	char  string[300] ; 
	char comparateur[12];
}



%error-verbose


%token tPRINT tMAIN  tCONST  tINT  tPO  tPF  tAO tAF  tMUL tDIV tEGAL tADD tSUB tVIR tPOINTVIR tNOMBREEXPO tERREUR tIF tELSE tOR tAND tWHILE

%left tADD tSUB
%left tDIV tMUL
%left NEG


%token <integer> tNOMBREDEC
%token <string> tID 
%token <comparateur> tEGALCOMP
%token <comparateur> tINF
%token <comparateur> tSUP
%type <integer> Expression
%type <comparateur> Comparateur
%type <integer> Cond 

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
																												printf( "COP %d %d \n",recherche($3), $5);
																												incrementerPC();
																												 viderPile();
																												  modifierChampInitialiserVariable($3);
																											}
																									} 
			|tINT tID tEGAL Expression tPOINTVIR{ //pas complet , changer champ initialiser var
																											if (ajouter_Var($2,1,0,1)==-1)
																											{
																												yyerror("ERROR ") ;
																											}else 
																											{
																												printf( "COP %d %d \n",recherche($2), $4);
																												incrementerPC();
																												 viderPile();
																												  modifierChampInitialiserVariable($2);
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

Body : tAO SuiteBody tAF {
														generer_fichier_table_des_symboles();
														generer_fichier_tab_label();
												} ;
			
SuiteBody :  Declaration SuiteBody
			|Affectation  SuiteBody
			|Print SuiteBody 
			|If SuiteBody
			|While SuiteBody
			|;
			
Affectation : tID tEGAL  Expression tPOINTVIR { 
																									int var ;
																									if ((  var =recherche($1))==-1)
																									{
																											yyerror("ERROR \n") ;
																									}else 
																									{
																										printf( "COP %d %d \n",var, $3);
																										incrementerPC();
																										 viderPile();
																										 modifierChampInitialiserVariable($1);									
																									}
																						} ;
										
										

/*Valeur : tNOMBREEXPO
		|tNOMBREDEC ;*/
		
		
//à completer
Print : tPRINT tPO Expression tPF tPOINTVIR {
																							printf("PRI %d\n",$3);
																							incrementerPC();
																							viderPile();
																						} ; 
									
									
Expression : tNOMBREDEC { 
													//$$=$1;
													printf("AFC %d %d\n",empiler($1,0) ,$1);
													incrementerPC();
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
						empiler(addr,1);
						//printf("AFC %d %d\n",empiler(addr,1) ,addr);
						//incrementerPC();
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
																							printf("ADD %d %d %d\n",getAdressePile(),valeurOp2,valeurOp1);
																							incrementerPC();
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==1 && typeOp2==0) //si 1er est une var et 2iem est une constante
																						{
																							//printf("pff---\n");
																							$$= getAdressePile() ; 
																							int NouvAdrOp2= obtenirAdressDeuxiemeOperande();
																							printf("ADD %d %d %d\n",getAdressePile(),valeurOp1,NouvAdrOp2);
																							incrementerPC();
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==0 && typeOp2==1) //si 1er est une constante et 2iem est une var
																						{
																							//printf("pff//\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 
																							$$=getAdressePile();
																							printf("ADD %d %d %d\n",getAdressePile(),NouvAdrOp1,valeurOp2);
																							incrementerPC();
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==0 && typeOp2==0) //si 1er est une constante et 2iem est une constante
																						{
																							//printf("pff\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 	
																							$$=getAdressePile();
																							int NouvAdrOp2 = obtenirAdressDeuxiemeOperande();
																							printf("ADD %d %d %d\n",getAdressePile(),NouvAdrOp1,NouvAdrOp2);
																							incrementerPC();
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
																							printf("SOU %d %d %d\n",getAdressePile(),valeurOp2,valeurOp1);
																							incrementerPC();
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==1 && typeOp2==0) //si 1er est une var et 2iem est une constante
																						{
																							//printf("pff---\n");
																							$$= getAdressePile() ; 
																							int NouvAdrOp2= obtenirAdressDeuxiemeOperande();
																							printf("SOU %d %d %d\n",getAdressePile(),valeurOp1,NouvAdrOp2);
																							incrementerPC();
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==0 && typeOp2==1) //si 1er est une constante et 2iem est une var
																						{
																							//printf("pff//\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 
																							$$=getAdressePile();
																							printf("SOU %d %d %d\n",getAdressePile(),NouvAdrOp1,valeurOp2);
																							incrementerPC();
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==0 && typeOp2==0) //si 1er est une constante et 2iem est une constante
																						{
																							//printf("pff\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 	
																							$$=getAdressePile();
																							int NouvAdrOp2 = obtenirAdressDeuxiemeOperande();
																							printf("SOU %d %d %d\n",getAdressePile(),NouvAdrOp1,NouvAdrOp2);
																							incrementerPC();
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
																							printf("MUL %d %d %d\n",getAdressePile(),valeurOp2,valeurOp1);
																							incrementerPC();
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==1 && typeOp2==0) //si 1er est une var et 2iem est une constante
																						{
																							//printf("pff---\n");
																							$$= getAdressePile() ; 
																							int NouvAdrOp2= obtenirAdressDeuxiemeOperande();
																							printf("MUL %d %d %d\n",getAdressePile(),valeurOp1,NouvAdrOp2);
																							incrementerPC();
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==0 && typeOp2==1) //si 1er est une constante et 2iem est une var
																						{
																							//printf("pff//\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 
																							$$=getAdressePile();
																							printf("MUL %d %d %d\n",getAdressePile(),NouvAdrOp1,valeurOp2);
																							incrementerPC();
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==0 && typeOp2==0) //si 1er est une constante et 2iem est une constante
																						{
																							//printf("pff\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 	
																							$$=getAdressePile();
																							int NouvAdrOp2 = obtenirAdressDeuxiemeOperande();
																							printf("MUL %d %d %d\n",getAdressePile(),NouvAdrOp1,NouvAdrOp2);
																							incrementerPC();
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
																							printf("DIV %d %d %d\n",getAdressePile(),valeurOp2,valeurOp1);
																							incrementerPC();
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==1 && typeOp2==0) //si 1er est une var et 2iem est une constante
																						{
																							//printf("pff---\n");
																							$$= getAdressePile() ; 
																							int NouvAdrOp2= obtenirAdressDeuxiemeOperande();
																							printf("DIV %d %d %d\n",getAdressePile(),valeurOp1,NouvAdrOp2);
																							incrementerPC();
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==0 && typeOp2==1) //si 1er est une constante et 2iem est une var
																						{
																							//printf("pff//\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 
																							$$=getAdressePile();
																							printf("DIV %d %d %d\n",getAdressePile(),NouvAdrOp1,valeurOp2);
																							incrementerPC();
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==0 && typeOp2==0) //si 1er est une constante et 2iem est une constante
																						{
																							//printf("pff\n");
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 	
																							$$=getAdressePile();
																							int NouvAdrOp2 = obtenirAdressDeuxiemeOperande();
																							printf("DIV %d %d %d\n",getAdressePile(),NouvAdrOp1,NouvAdrOp2);
																							incrementerPC();
																							empiler(getAdressePile(),1) ;
																						}
																			}
  														}
  
  
  | tSUB Expression %prec NEG  {	
  																int typeOp;
  																int valeurOp = depiler(&typeOp);
  																if (typeOp==0)//si constante
  																{
  																	printf("AFC %d -%d\n",empiler(-valeurOp,0),valeurOp);
  																	incrementerPC();
  																}
  																$$=getAdressePile()+1;
  																//empiler(-valeurOp,0);	
  															}
  | tPO Expression tPF   {
  												 $$ = $2;  
  												} ; 
  												

If: tIF tPO Cond tPF tAO {
														viderPile();
														char * label =  ajouter_label();
														printf("JMF %d %s\n",$3,label);
														incrementerPC();
														empilerPremierLabelIF(label); 
													}  SuiteBody { 
																				char * label =  ajouter_label();
																					printf("JMP %s\n",label);
																					incrementerPC();
																					empilerDeuxiemeLabelIF(label);
																				} tAF Else ;
																						
Else : tELSE tAO {char label[TAILLE];
								depilerPremierLabelIF(label) ;
								modifierNum_instruction(label,pc);
								} SuiteBody tAF {
																	char label[TAILLE];
																	depilerDeuxiemeLabelIF(label);
																	modifierNum_instruction(label,pc);
																} 
																
			|{
				char label[TAILLE];
				depilerPremierLabelIF(label) ;
				char label2[TAILLE];
				depilerDeuxiemeLabelIF(label2);
				modifierNum_instruction(label,pc);
				printf("------------------------%d\n",pc);
				modifierNum_instruction(label2,pc);
				printf("------------------------%d\n",pc);
				};
		
				
				
While : tWHILE{
							char * label = ajouter_label(); 
							
							//printf("LABEL %s\n",label);
							empilerPremierLabelWhile(label);
							modifierNum_instruction(label,pc);
						//	incrementerPC();
							} 
						tPO Cond tPF tAO {
																viderPile();
																char * label =  ajouter_label();
																printf("JMF %d %s\n",$4,label);
																incrementerPC();
																empilerDeuxiemeLabelWhile(label); 													
																}
								SuiteBody{
													char  label[TAILLE] ; 
													depilerPremierLabelWhile(label);
													printf("JMP %s\n", label);
													incrementerPC();
												} 
								tAF {
											char  label[TAILLE] ;
											depilerDeuxiemeLabelWhile(label) ;
											//printf("LABEL %s\n",label);
											modifierNum_instruction(label,pc);
										}; 			

//rajouter tOR et tAND
Cond: Expression Comparateur Expression {
																						//on s'en fou de ce que l'empile , c'est l' qui nous interesse 
																					int retour =empiler(-1,1);
																					$$=retour; 
																					if(strcmp($2,"==")==0)
																					{
																					
																						printf("EQU %d %d %d\n", retour,$1,$3);
																					
																						incrementerPC();

																					}else if(strcmp($2,"<")==0)
																					{
																						printf("INF %d %d %d\n",  retour,$1,$3);
																						incrementerPC();

																					}else if(strcmp($2,">")==0)
																					{
																						printf("SUP %d %d %d\n",retour,$1,$3);
																						incrementerPC();

																					}
																					
																				}
			|Expression {$$=$1;}
			|Cond tOR Cond {int retour = empiler(-1,1);
											$$=retour; 
											printf("OR %d %d %d\n", retour,$1,$3);
										}
			|Cond tAND Cond{int retour = empiler(-1,1);
											$$=retour; 
											printf("AND %d %d %d\n", retour,$1,$3);
										};

 
							
Comparateur:tEGALCOMP {strcpy($$,$1);}
						|tSUP {strcpy($$,$1);}
						|tINF {strcpy($$,$1);};
			
%%


int yyerror(char *s) {
  printf("%s\n",s);
}

int main(void) {
  yyparse();
}
