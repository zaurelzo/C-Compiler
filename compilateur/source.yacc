%{
#include <stdio.h>
#include <stdlib.h>
#include "tab_symb.h"
#include "tab_label.h"
#include "asm.h"
#include "gestion_des_fonctions.h"
%}



%union  {
	int integer ;
	char  string[300] ; 
	char comparateur[12];
	struct 
	{
		int adresse; 
		int relative_ou_absolue;
		int typage_var ; 
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
 
Input: DeclarationGlobale ;


DeclarationGlobale : 
	Declaration 
	{
		setNombredevariableglobale();
	} DeclarationGlobale  
	|  AffectationGlobale ;


AffectationGlobale : 
	Affectation  AffectationGlobale 
	| PrototypeAndImplementationGlobalAndMain  ;



PrototypeAndImplementationGlobalAndMain  : 
	Prototype 
	{
		
		if (ADD_PROTOTYPE_ASM()==-1)
		{
			yyerror("ERROR WHEN ADD PROTOTYPE\n");
		} 
		 initParametreForNewfunction() ;
	} PrototypeAndImplementationGlobalAndMain
	
	| ImplementationFonction 
		{
			initParametreForNewfunction() ;
		} PrototypeAndImplementationGlobalAndMain
	
	| {
			//debug 
			//print_TABLE_DES_FONCTION() ;
			changeMode();//on passe en mode fonction
			initParametreForNewfunction();
			setTailleTypeRetourFonction(0);
			//printf("++++++++indice : %d and mode : %s\n",ind,mode);
		} Main ;


Declaration : 
	tINT tID 
	{ 
		
		if ( declaration_asm($2,1) ==-1 )  
		{
			yyerror("ERROR WHEN DECLARATION OF ") ;
			yyerror($2);yyerror("\n");
		}
	}  SuiteDeclarations  
							
							
	|tCONST tINT tID tEGAL Expression tPOINTVIR 
		{ 
			if (declaration_affectation_asm( $3 ,1 ,$5.relative_ou_absolue, $5.adresse,1)==-1)
			{
				yyerror("ERROR WHEN CONSTANT DECLARATION AFFECTATION,VARIABLE %s IS NOT EXIST :",$3) ;
				yyerror("\n");	
			}
		} 
		
		
	|tINT tID tEGAL Expression tPOINTVIR
		{ 
			if (declaration_affectation_asm( $2 ,0 ,$4.relative_ou_absolue, $4.adresse,1)==-1)
			{
				yyerror("ERROR WHEN DECLARATION AFFECTATION,VAR %s NOT EXIST :",$2) ;
				yyerror("\n");	
			}
		}; 
	
										
SuiteDeclarations : 
	 tPOINTVIR
	| tVIR tID 
	{ 
		if ( declaration_asm($2,1) ==-1 )  
		{
			yyerror("ERROR WHEN DECLARATION OF ") ;
			yyerror($2);yyerror("\n");
		}
	} SuiteDeclarations ; 
		
									

							
Main: 
	tMAIN 
	{
		
		
	} tPO  tPF Body ;

Body : 
	tAO SuiteBody  tAF 
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
			|Return SuiteBody
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
		print_asm( $3.adresse, $3.relative_ou_absolue);
	} ; 
									
									
Expression : 
	tNOMBREDEC 
	{ 
		printf("-AFC @%d %d\n",empilert($1,0,1,1) ,$1);
		incrementerPC();
		$$.adresse=getAdressePile()+1;
		$$.relative_ou_absolue=1 ; // absolue 
		$$.typage_var=1;  
	}
												
	|tID 
	{ 
		int addr ;
		int abs_rel;
		int typage_var;
		if ((  addr =recherchet($1,&abs_rel,&typage_var))==-1)
		{
			yyerror("ERROR WHEN SEARCH VARIABLE %s\n",$1) ;
		}else 
		{				
			empilert(addr,1,abs_rel,typage_var);
			$$.adresse= addr;
			$$.relative_ou_absolue=abs_rel ; // absolue
			$$.typage_var=typage_var;  
		} 
	}
				
	|AppelFonctions 
	{
		//APPEL_FONCTION_IN_EXPRESSION_ASM();
		$$.adresse=$1.adresse;//le résulat se trouve toujours à l'adresse 0!
		$$.relative_ou_absolue =$1.relative_ou_absolue; //qui est toujours une valeur relative
		$$.typage_var=$1.typage_var;
	}
				
  | Expression tADD Expression 
  	{
   		if (operation_arithmetique_asm("ADD", &($$.adresse),&($$.relative_ou_absolue),&($$.typage_var) )==-1)
   		{
   			yyerror("ERROR WHEN POP OPERANDS\n");
   		}
		}
 
 
 
  | Expression tSUB Expression 
  	{
   		if (operation_arithmetique_asm("SUB", &($$.adresse),&($$.relative_ou_absolue),&($$.typage_var))==-1)
   		{
   			yyerror("ERROR WHEN POP OPERANDS\n");
   		}
		}
  
  
  | Expression tMUL Expression
  	{
   		if (operation_arithmetique_asm("MUL", &($$.adresse),&($$.relative_ou_absolue),&($$.typage_var))==-1)
   		{
   			yyerror("ERROR WHEN POP OPERANDS\n");
   		}
		}
  															
  															
  | Expression tDIV Expression 
  	{
   		if (operation_arithmetique_asm("ADD", &($$.adresse),&($$.relative_ou_absolue),&($$.typage_var))==-1)
   		{
   			yyerror("ERROR WHEN POP OPERANDS\n");
   		}
		}
  
  
  | tSUB Expression %prec NEG  
		{
			nombre_negatif_asm(&($$.adresse),&($$.relative_ou_absolue),&($$.typage_var));																	
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
		CONDITION_ASM($2, &($$.adresse),&($$.relative_ou_absolue),&($$.typage_var) , $1.adresse,$3.adresse);
	}
	|Expression {$$=$1;}
	|Cond tOR Cond 
	{
		int retour = empilert(-1,1,1,1);//c'est @ qui nous interesse , pas ce que l'on empile
		$$.adresse=retour;
		$$.relative_ou_absolue=1; 
		printf("OR @%d @%d @%d\n", retour,$1.adresse,$3.adresse);
		incrementerPC();
	}
	|Cond tAND Cond
	{
		int retour = empilert(-1,1,1,1);//c'est @ qui nous interesse , pas ce que l'on empile
		$$.adresse=retour;
		$$.relative_ou_absolue=1;
		printf("AND %d %d %d\n", retour,$1.adresse,$3.adresse);
		incrementerPC();
	};

 
							
Comparateur:
	tEGALCOMP {strcpy($$,$1);}
	|tSUP {strcpy($$,$1);}
	|tINF {strcpy($$,$1);};
						

						
Prototype : 
	tINT tID tPO Params tPF tPOINTVIR
	{
		setTypeRetour(1);
		changeMode();//on passe en mode fonction
		setTailleTypeRetourFonction(1);//on fixe le faite que l'on est un type retour ou pas
		setIDprototypeOrImplementationFunction($2);
	} ;

ImplementationFonction : 
	tINT tID tPO Params tPF
	{
		setTypeRetour(1);
		changeMode();//on passe en mode fonction
		setTailleTypeRetourFonction(1);//on fixe le faite que l'on est un type retour ou pas
		setIDprototypeOrImplementationFunction($2);
		if (ADD_IMPLEMENTATION_FUNCTION_ASM(getPcValue())==-1)
		{
			yyerror("ERROR WHEN ADD FUNCTION IMPLEMENTATION\n");
		}
	} Body 
	{
		initNombreDeParametres() ; // on a fini de generer la code de la fonction, on peut init nb parametres pour que la fonction suivante puisse ajouter ces parametre 
	} ;


Params : 
	tINT tID 
	{
		//IncrementeNBParametre();//on est entrain de compter le nombre de parametre de la fonction
		if ( declaration_asm($2,1) ==-1 )  
		{
			yyerror("ERROR WHEN DECLARATION PARAMETER ") ;
			yyerror($2);yyerror("\n");
		}
		
		if (ajouter_parametre(1,0,$2)==-1 )
		{
			yyerror("ERROR : PARAMETER %s IS ALREADY DECLARED",$2);
		}
	} SuiteParams
	|;
				
SuiteParams : 
	tVIR tINT tID
	{
		if ( declaration_asm($3,1) ==-1 )  
		{
			yyerror("ERROR WHEN DECLARATION PARAMETER ") ;
			yyerror($3);yyerror("\n");
		}
		
		if (ajouter_parametre(1,0,$3) ==-1)
		{
			yyerror("ERROR : PARAMETER %s IS ALREADY DECLARED",$3);
		}
	} SuiteParams
	|;
						
AppelFonctions : 
	tID tPO 
	{
		//on fixe l'id de la fonction appelée 
		setIDprototypeOrImplementationFunction($1);
		 PUSH_PC_ASM();
	}
	ParamAppel tPF tPOINTVIR
	{
		if (VERIFICATION_AND_CALL_GENERATION_ASM()==-1)
		{
			yyerror("ERROR WHEN CALL FUNCTION");
			yyerror(getIDprototypeOrImplementationFunction());yyerror("\n");
		}
		empilert(0,1,0,1);//on empile l'adresse du résultat qui sera toujours 0 (@ relatif)		
		$$.adresse=0;
		$$.relative_ou_absolue=0;//l'@ du return sera toujours une @ relative et aura la valeur 0  
		$$.typage_var= getTypeRetour() ;
	};

ParamAppel : 
	Expression 
	{
		ajouter_parametreAPPEL($1.typage_var);		 
		CALL_PARAMETERS_ASM($1.adresse, $1.relative_ou_absolue);
	} SuiteParamAppel
	|;

SuiteParamAppel : 
	tVIR Expression
	{
		ajouter_parametreAPPEL($2.typage_var);	
		CALL_PARAMETERS_ASM($2.adresse, $2.relative_ou_absolue);
	} SuiteParamAppel
	|;
						
Return : 
	tRETURN /*{ empilert(0,1,0,1); }*/ Expression
	{
		RETURN_ASM ($2.adresse, $2.relative_ou_absolue,$2.typage_var); 
	} tPOINTVIR ;
	 
			
%%


int yyerror(char *s) {
  printf("%s %s\n",KRED,s);
  exit(1);
}

int main(void) {
  yyparse();
}
