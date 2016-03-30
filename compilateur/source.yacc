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
	struct 
	{
		int adresse; 
		int relative_ou_absolue;
	 } adresseDesVariables ;
}



%error-verbose


%token tPRINT tMAIN  tCONST  tINT  tPO  tPF  tAO tAF  tMUL tDIV tEGAL tADD tSUB tVIR tPOINTVIR tNOMBREEXPO tERREUR tIF tELSE tOR tAND tWHILE tRETURN

%left tADD tSUB tOR 
%left tDIV tMUL tAND
%left NEG




%token <integer> tNOMBREDEC
%token <string> tID 
%token <comparateur> tEGALCOMP
%token <comparateur> tINF
%token <comparateur> tSUP
%type <adresseDesVariables> Expression
%type <adresseDesVariables> AppelFonctions
%type <comparateur> Comparateur
%type <adresseDesVariables> Cond 
%type <adresseDesVariables> NEG
%start Input
%%
 
Input: DeclarationGlobale {setNombredevariableglobale();} Input1 | Input1;

Input1 : AffectationGlobale Input2 | Input2;

Input2 : PrototypeGlobal   Input3 | Input3;

Input3 : ImplementationFonctionGlobal Main; 

DeclarationGlobale : Declaration DeclarationGlobale | ;
AffectationGlobale : Affectation AffectationGlobale | ;
PrototypeGlobal : Prototype PrototypeGlobal | ;
ImplementationFonctionGlobal : ImplementationFonction ImplementationFonctionGlobal | ;


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
																												int abs_rel ; 
																												int var = recherchet($3,&abs_rel);
																												
																												if ($5.relative_ou_absolue==0 && abs_rel==0)
																													printf( "COP @@%d @@%d \n",var,$5.adresse);
																												else if  ($5.relative_ou_absolue==0 && abs_rel==1)
																													printf( "COP @@%d @%d \n",var,$5.adresse);
																												else if  ($5.relative_ou_absolue==1 && abs_rel==0)
																													printf( "COP @%d @@%d \n",var,$5.adresse);
																												else if  ($5.relative_ou_absolue==1 && abs_rel==1)
																													printf( "COP @%d @%d \n",var,$5.adresse);
																													
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
																												int abs_rel ; 
																												int var = recherchet($2,&abs_rel);
																												
																												if ($4.relative_ou_absolue==0 && abs_rel==0)
																													printf( "COP @@%d @@%d \n",var,$4.adresse);
																												else if  ($4.relative_ou_absolue==0 && abs_rel==1)
																													printf( "COP @@%d @%d \n",var,$4.adresse);
																												else if  ($4.relative_ou_absolue==1 && abs_rel==0)
																													printf( "COP @%d @@%d \n",var,$4.adresse);
																												else if  ($4.relative_ou_absolue==1 && abs_rel==1)
																													printf( "COP @%d @%d \n",var,$4.adresse);
																													
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

Body : tAO SuiteBody Return tAF {
														//generer_fichier_table_des_symboles();
														generer_fichier_tab_label();
												} ;
			
SuiteBody :  Declaration SuiteBody
			|Affectation  SuiteBody
			|AppelFonctions SuiteBody
			|Print SuiteBody 
			|If SuiteBody
			|While SuiteBody
			|;
			
Affectation : tID tEGAL  Expression tPOINTVIR { 
																									int var ;
																									int abs_rel ; 
																									
																									if ((  var =recherchet($1,&abs_rel))==-1)
																									{
																											yyerror("ERROR \n") ;
																									}else 
																									{
																											//printf("nombre var glabal :  %d \n " ,nombreDeVariabeleglobale);
																											if ($3.relative_ou_absolue==0 && abs_rel==0)
																													printf( "COP @@%d @@%d \n",var,$3.adresse);
																												else if  ($3.relative_ou_absolue==0 && abs_rel==1)
																													printf( "COP @%d @@%d \n",var,$3.adresse);
																												else if  ($3.relative_ou_absolue==1 && abs_rel==0)
																													printf( "COP @@%d @%d \n",var,$3.adresse);
																												else if  ($3.relative_ou_absolue==1 && abs_rel==1)
																													printf( "COP @%d @%d \n",var,$3.adresse);
																													
																										incrementerPC();
																										viderPile();
																										modifierChampInitialiserVariable($1);									
																									}
																						} ;
										
										

/*Valeur : tNOMBREEXPO
		|tNOMBREDEC ;*/
		
		
//à completer
Print : tPRINT tPO Expression tPF tPOINTVIR {
																							if($3.relative_ou_absolue==0)
																								printf("PRI @@%d\n",$3.adresse);
																							else 
																								printf("PRI @%d\n",$3.adresse);
																							
																							incrementerPC();
																							viderPile();
																						} ; 
									
									
Expression : tNOMBREDEC { 
													//$$=$1;
													printf("AFC @%d %d\n",empilert($1,0,1) ,$1);
													incrementerPC();
													$$.adresse=getAdressePile()+1;
													$$.relative_ou_absolue=1 ; // absolue   
												}
												
	|tID {
					//$$=$1; 
					int addr ;
					int abs_rel;
					if ((  addr =recherchet($1,&abs_rel))==-1)
					{
						yyerror("ERROR : Variable %s non existante \n",$1) ;
					}else 
					{
					
						
						empilert(addr,1,abs_rel);
						//printf("AFC %d %d\n",empiler(addr,1) ,addr);
						//incrementerPC();
						$$.adresse= addr;
						$$.relative_ou_absolue=abs_rel ; // absolue 
					} 
				}
				
	|AppelFonctions /*{return }*/
				
  | Expression tADD Expression {
																			int typeOp1,typeOp2; 
																			int  abs_rel1,abs_rel2;
																			int valeurOp2 = depilert(&typeOp2,&abs_rel2);
																			int valeurOp1= depilert(&typeOp1,&abs_rel1);
																			//printf("type 1 : %d | type 2 : %d\n",typeOp1,typeOp2);
																			if (valeurOp2==-1 || valeurOp1==-1 )
																			{
																				//printf("ERREUR indicePile :%d\n",indPile );
																				yyerror("ERREUR LORS DU DÉPILEMENT DES OPÉRANDES \n") ;
																			}else 
																			{
																						$$.adresse= getAdressePile();
																						incrementerPC();
																						$$.relative_ou_absolue=1;
																						
																						if (typeOp1==1 && typeOp2==1)//si 1er et 2iem opérande est une variable 
																						{
																							if (abs_rel1==0 && abs_rel2==0)
																								printf("ADD @%d @@%d @@%d\n",getAdressePile(),valeurOp2,valeurOp1);
																							else if (abs_rel1==0 && abs_rel2==1)
																								printf("ADD @%d @%d @@%d\n",getAdressePile(),valeurOp2,valeurOp1); 	
																							else if (abs_rel1==1 && abs_rel2==0)
																								printf("ADD @%d @@%d @%d\n",getAdressePile(),valeurOp2,valeurOp1); 	
																							else if (abs_rel1==1 && abs_rel2==1)
																								printf("ADD @%d @%d @%d\n",getAdressePile(),valeurOp2,valeurOp1); 	
																							empiler(getAdressePile(),1) ;
																							
																						}else if( typeOp1==1 && typeOp2==0) //si 1er est une var et 2iem est une constante
																						{
																							//printf("++++++++++++++++++++++++ %d %d \n",abs_rel1,abs_rel2);
																							int NouvAdrOp2= obtenirAdressDeuxiemeOperande();
																							if (abs_rel1==0 && abs_rel2==0)
																								printf("ADD @%d @@%d @@%d\n",getAdressePile(),valeurOp1,NouvAdrOp2);
																							else if (abs_rel1==0 && abs_rel2==1)
																								printf("ADD @%d @@%d @%d\n",getAdressePile(),valeurOp1,NouvAdrOp2);
																							else if (abs_rel1==1 && abs_rel2==0)
																								printf("ADD @%d @%d @@%d\n",getAdressePile(),valeurOp1,NouvAdrOp2);
																							else if (abs_rel1==1 && abs_rel2==1)
																								printf("ADD @%d @%d @%d\n",getAdressePile(),valeurOp1,NouvAdrOp2);
																						
																							empiler(getAdressePile(),1) ;
																						}else if( typeOp1==0 && typeOp2==1) //si 1er est une constante et 2iem est une var
																						{
																							
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 
																							
																							if (abs_rel1==0 && abs_rel2==0)
																								printf("ADD @%d @@%d @@%d\n",getAdressePile(),NouvAdrOp1,valeurOp2);
																							if (abs_rel1==0 && abs_rel2==1)
																								printf("ADD @%d @@%d @%d\n",getAdressePile(),NouvAdrOp1,valeurOp2);
																							if (abs_rel1==1 && abs_rel2==0)
																								printf("ADD @%d @%d @@%d\n",getAdressePile(),NouvAdrOp1,valeurOp2);
																							if (abs_rel1==1 && abs_rel2==1)
																								printf("ADD @%d @%d @%d\n",getAdressePile(),NouvAdrOp1,valeurOp2);
																								
																							empiler(getAdressePile(),1) ;
																							
																						}else if( typeOp1==0 && typeOp2==0) //si 1er est une constante et 2iem est une constante
																						{
																							//printf("++++++++++++++++++++++++ %d %d \n",abs_rel1,abs_rel2);
																							int NouvAdrOp1=obtenirAdressePremierOperande(); 	
																							int NouvAdrOp2 = obtenirAdressDeuxiemeOperande();
																							
																							if (abs_rel1==0 && abs_rel2==0)
																								printf("ADD @%d @@%d @@%d\n",getAdressePile(),NouvAdrOp1,NouvAdrOp2);
																							else if (abs_rel1==0 && abs_rel2==1)
																								 printf("ADD @%d @@%d @%d\n",getAdressePile(),NouvAdrOp1,NouvAdrOp2);
																							else if (abs_rel1==1 && abs_rel2==0)
																								 printf("ADD @%d @%d @@%d\n",getAdressePile(),NouvAdrOp1,NouvAdrOp2);
																							else if (abs_rel1==1 && abs_rel2==1)
																								 printf("ADD @%d @%d @%d\n",getAdressePile(),NouvAdrOp1,NouvAdrOp2);
																						
																							empiler(getAdressePile(),1) ;
																						}
																			}
  														}
 
 
 
  | Expression tSUB Expression /*{
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
  														}*/
  
  
  | Expression tMUL Expression /*{
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
  														}*/
  															
  															
  | Expression tDIV Expression /*{
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
  														}*/
  
  
  | tSUB Expression %prec NEG  {	
  																$$.adresse=1;
  																$$.relative_ou_absolue=1;
  																/*int typeOp;
  																int valeurOp = depiler(&typeOp);
  																if (typeOp==0)//si constante
  																{
  																	printf("AFC %d -%d\n",empiler(-valeurOp,0),valeurOp);
  																	incrementerPC();
  																}
  																$$=getAdressePile()+1;
  																//empiler(-valeurOp,0);*/	
  															}
  | tPO Expression tPF   {
  													$$.adresse=1;
  													$$.relative_ou_absolue=1;
  												 //$$ = $2;  
  												} ; 
  												

If: tIF tPO Cond tPF tAO {
														viderPile();
														char * label =  ajouter_label();
														printf("JMF @%d %s\n",$3.adresse,label);
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
																	//viderPile();
																} 
																
			|{
				char label[TAILLE];
				depilerPremierLabelIF(label) ;
				char label2[TAILLE];
				depilerDeuxiemeLabelIF(label2);
				modifierNum_instruction(label,pc);
				//printf("------------------------%d\n",pc);
				modifierNum_instruction(label2,pc);
				//printf("------------------------%d\n",pc);
				//viderPile();
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
																printf("JMF @%d %s\n",$4.adresse,label);
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
											//viderPile();
										}; 			

//rajouter tOR et tAND
Cond: Expression Comparateur Expression {
																						//on s'en fou de ce que l'empile , c'est l'@ qui nous interesse 
																					int retour =empilert(-1,1,1);
																					$$.adresse=retour;
																					$$.relative_ou_absolue=1;
																					if(strcmp($2,"==")==0)
																					{
																							printf("EQU @%d @%d @%d\n", retour,$1.adresse,$3.adresse);
																						incrementerPC();

																					}else if(strcmp($2,"<")==0)
																					{
																						printf("INF @%d @%d @%d\n", retour,$1.adresse,$3.adresse);
																						incrementerPC();

																					}else if(strcmp($2,">")==0)
																					{
																						printf("SUP @%d @%d @%d\n", retour,$1.adresse,$3.adresse);
																						incrementerPC();

																					}
																					
																				}
			|Expression {$$=$1;}
			|Cond tOR Cond {int retour = empilert(-1,1,1);
											$$.adresse=retour;
											$$.relative_ou_absolue=1; 
											printf("OR @%d @%d @%d\n", retour,$1.adresse,$3.adresse);
										}
			|Cond tAND Cond{int retour = empilert(-1,1,1);
											$$.adresse=retour;
											$$.relative_ou_absolue=1;
											printf("AND %d %d %d\n", retour,$1.adresse,$3.adresse);
										};

 
							
Comparateur:tEGALCOMP {strcpy($$,$1);}
						|tSUP {strcpy($$,$1);}
						|tINF {strcpy($$,$1);};
						

						
Prototype : tINT tID tPO Params tPF tPOINTVIR;

ImplementationFonction : tINT tID tPO Params tPF Body  ;


Params : tINT tID SuiteParams
				|;
				
SuiteParams : tVIR tINT tID SuiteParams
						|;
						
AppelFonctions : tID tPO ParamAppel tPF tPOINTVIR{
																									$$.adresse=4;
																									$$.relative_ou_absolue=1;
																									};

ParamAppel : Expression SuiteParamAppel
					|;

SuiteParamAppel : tVIR Expression SuiteParamAppel
						|;
						
Return : tRETURN Expression tPOINTVIR
				|;
			
%%


int yyerror(char *s) {
  printf("%s\n",s);
}

int main(void) {
  yyparse();
}
