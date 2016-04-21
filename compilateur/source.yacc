%{
#include <stdio.h>
#include <stdlib.h>
#include "tab_symb.h"
#include "tab_label.h"
#include "asm.h"

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


Declaration : 
	tINT tID 
	{ 
		if ( ajouter_Var($2,1,0,0)==-1 )  
		{
			yyerror("ERROR WHEN DECLARATION OF ") ;
			yyerror($2);yyerror("\n");
		}/* else
		{	
			printTabVar();
		} */
	}  SuiteDeclarations  
							
							
	|tCONST tINT tID tEGAL Expression tPOINTVIR 
		{ 
			if (declaration_affectation_asm( $3 ,1 ,$5.relative_ou_absolue, $5.adresse)==-1)
			{
				yyerror("ERROR WHEN CONSTANT DECLARATION AFFECTATION,VARIABLE %s IS NOT EXIST :",$3) ;
				yyerror("\n");	
			}
		} 
		
		
	|tINT tID tEGAL Expression tPOINTVIR
		{ 
			if (declaration_affectation_asm( $2 ,0 ,$4.relative_ou_absolue, $4.adresse)==-1)
			{
				yyerror("ERROR WHEN DECLARATION AFFECTATION,VAR %s NOT EXIST :",$2) ;
				yyerror("\n");	
			}
		}; 
	
										
SuiteDeclarations : 
	 tPOINTVIR
	| tVIR tID SuiteDeclarations  
		{ 
			if ( ajouter_Var($2,1,0,0)==-1 )  
			{
				yyerror("ERROR WHEN DECLARATION OF");
				yyerror($2);yyerror("\n");	
			} 
		};
									

							
Main: tMAIN tPO  tPF Body ;

Body : 
	tAO SuiteBody Return tAF 
	{
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
			
Affectation : 
	tID tEGAL  Expression tPOINTVIR 
	{
		if (affection_asm( $1 ,$3.relative_ou_absolue, $3.adresse)==-1)
		{
			yyerror("ERROR WHEN AFFECTATION,VAR %s IS NOT EXIST :",$1) ;
			yyerror("\n");	
		} 
	};
										
										
		
//à completer
Print : 
	tPRINT tPO Expression tPF tPOINTVIR 
	{
		print_asm($3.relative_ou_absolue);
	} ; 
									
									
Expression : 
	tNOMBREDEC 
	{ 
		printf("AFC @%d %d\n",empilert($1,0,1) ,$1);
		incrementerPC();
		$$.adresse=getAdressePile()+1;
		$$.relative_ou_absolue=1 ; // absolue   
	}
												
	|tID 
	{ 
		int addr ;
		int abs_rel;
		if ((  addr =recherchet($1,&abs_rel))==-1)
		{
			yyerror("ERROR WHEN SEARCH VARIABLE %s\n",$1) ;
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
				
  | Expression tADD Expression 
  	{
   		if (operation_arithmetique_asm("ADD", &($$.adresse),&($$.relative_ou_absolue))==-1)
   		{
   			yyerror("ERROR WHEN POP OPERANDS\n");
   		}
		}
 
 
 
  | Expression tSUB Expression 
  	{
   		if (operation_arithmetique_asm("SUB", &($$.adresse),&($$.relative_ou_absolue))==-1)
   		{
   			yyerror("ERROR WHEN POP OPERANDS\n");
   		}
		}
  
  
  | Expression tMUL Expression
  	{
   		if (operation_arithmetique_asm("MUL", &($$.adresse),&($$.relative_ou_absolue))==-1)
   		{
   			yyerror("ERROR WHEN POP OPERANDS\n");
   		}
		}
  															
  															
  | Expression tDIV Expression 
  	{
   		if (operation_arithmetique_asm("ADD", &($$.adresse),&($$.relative_ou_absolue))==-1)
   		{
   			yyerror("ERROR WHEN POP OPERANDS\n");
   		}
		}
  
  
  | tSUB Expression %prec NEG  
		{
			nombre_negatif_asm(&($$.adresse),&($$.relative_ou_absolue));																	
		}
 	
	| tPO Expression tPF   
  	{
  		$$.adresse=$2.adresse;
  		$$.relative_ou_absolue=$2.relative_ou_absolue;  
  	} ; 
  												

If: 
	tIF tPO Cond tPF tAO 
	{
		JMF_IF_ASM($3.adresse); 
	}  SuiteBody 
	{
		JMP_IF_ASM();	
	} tAF Else ;
																						
Else : 
	tELSE tAO 
	{
		ELSE_FIRST_LABEL_ASM();
	} SuiteBody tAF 
	{
		ELSE_SECOND_LABEL_ASM();
		//viderPile();
	} 
|	{
		 IF_WITHOUT_ELSE_ASM();
	};
		
				
				
While : 
	tWHILE
	{
		 WHILE_FIRST_LABEL_ASM();
	} 
	tPO Cond tPF tAO 
	{
		JMF_WHILE_ASM($4.adresse);
	}SuiteBody
	{
		JMP_WHILE_ASM();
	} 
	tAF 
	{
		 WHILE_SECOND_LABEL_ASM();
	}; 			

//rajouter tOR et tAND
Cond: 
	Expression Comparateur Expression 
	{
		CONDITION_ASM($2, &($$.adresse),&($$.relative_ou_absolue) , $1.adresse,$1.adresse);
	}
	|Expression {$$=$1;}
	|Cond tOR Cond 
	{
		int retour = empilert(-1,1,1);
		$$.adresse=retour;
		$$.relative_ou_absolue=1; 
		printf("OR @%d @%d @%d\n", retour,$1.adresse,$3.adresse);
		incrementerPC();
	}
	|Cond tAND Cond
	{
		int retour = empilert(-1,1,1);
		$$.adresse=retour;
		$$.relative_ou_absolue=1;
		printf("AND %d %d %d\n", retour,$1.adresse,$3.adresse);
		incrementerPC();
	};

 
							
Comparateur:
	tEGALCOMP {strcpy($$,$1);}
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
  printf("%s %s",KRED,s);
}

int main(void) {
  yyparse();
}
