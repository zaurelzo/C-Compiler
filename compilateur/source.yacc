%{
#include <stdio.h>
#include <stdlib.h>
#include "tab_symb.h"
#include "tab_label.h"
#include "deuxieme_passe.h"
#include "asm.h"
#include "asm_j.h"
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


%token tPRINT tMAIN  tCONST  tINT  tPO  tPF  tAO tAF  tMUL tDIV tEGAL tADD tSUB tVIR tPOINTVIR tNOMBREEXPO tIF tELSE tOR tAND tWHILE tRETURN tETCOM tCO tCF tNULL tERREUR 

%left tADD tSUB tOR 
%left tDIV tMUL tAND
%left NEG PTR




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
%type <adresseDesVariables> AccesCase

%type <adresseDesVariables> ExpressionPointeur
%type <string> Etoile
%type <string> Dereferencement



%start Input
%%
 
Input: DeclarationGlobale;


DeclarationGlobale : 
		
	Declaration 
	{
		setDeclaGlobale();
		setNombredevariableglobale();
	} DeclarationGlobale 
	|  AffectationGlobale  ;


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
			WriteDebugInformationAboutFunction(0);
			initParametreForNewfunction() ;
		} PrototypeAndImplementationGlobalAndMain
	
	| {
			//debug 
			
			WriteDebugInformationAboutFunction(1);
			setIDprototypeOrImplementationFunction("Main");
			//print_TABLE_DES_FONCTION() ;
			reinitDeclaGlobale();
			changeMode();//on passe en mode fonction
			initParametreForNewfunction();
			setTailleTypeRetourFonction(0);
			//setDeclaGlobale();
			//printf("++++++++nb globale : %d and ind %d ",getNombredevariableglobale(),ind);
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
		} 
	| Pointeur
	| Tableau;
										
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
		printf("MAIN\n");	
		
	} tPO  tPF Body{WriteDebugInformationAboutFunction(0);} ;

Body : 
	tAO SuiteBody  tAF 
	{
		//generer_fichier_table_des_symboles();
		generer_fichier_tab_label();
		genererTableDesFonctions();
	} ;
			
SuiteBody :  Declaration SuiteBody
			|Affectation  SuiteBody
			|AppelFonctions tPOINTVIR SuiteBody
			|Print SuiteBody 
			|If SuiteBody
			|While SuiteBody
			|Return SuiteBody
			|;
			
Affectation : 
	tID tEGAL  Expression tPOINTVIR 
	{
		int retour = affectation_gauche_id_tab_ptrNonDeref( $1 ,$3.adresse,$3.relative_ou_absolue);
		if(retour==-1)		
		{
			yyerror("ERROR WHEN AFFECTATION,VAR %s IS NOT EXIST :",$1) ;
			yyerror("\n");	
		} 
		else if (retour==-2)
		{
			yyerror("Problème vous travailler avec des types différents de chaque côté du =\n");
		}
		else if (retour==-3)
		{
			yyerror("Problème de déréférencement tableau à droite \n");	
		}
		else if (retour==-4)
		{
			yyerror("Problème vous travailler avec des types différents de chaque côté du =\n");	
		}
		else if (retour==-5)
		{
			yyerror("problème vous devez avoir un pointeur à droite de l'affectation\n");
		}
	}
	|Dereferencement {init_niveau_pointeur_3(get_nivPtr());reinitialiser_nivPointeur_2();reinitialiser_nivPointeur();} tEGAL  Expression tPOINTVIR 
	{
		
		int retour = affectation_Deref( $1 ,$4.adresse,$4.relative_ou_absolue);
		if(retour==-1)		
		{
			yyerror("ERROR WHEN AFFECTATION,VAR %s IS NOT EXIST :",$1) ;
			yyerror("\n");	
		} 
		else if (retour==-2)
		{
			yyerror("Problème vous travailler avec des types différents de chaque côté du =\n");
		}
	}
	|AffectationTab;
	
	//| | Dereferencement {init_niveau_pointeur_3(get_nivPtr());reinitialiser_nivPointeur_2();reinitialiser_nivPointeur();} tEGAL  Expression tPOINTVIR 
	//| AffectationTab;									
										
		
//à completer
Print : 
	tPRINT tPO Expression tPF tPOINTVIR 
	{
		print_asm( $3.adresse, $3.relative_ou_absolue);
	} ; 
									
									
Expression : 
	tNOMBREDEC 
	{ 
		printf("AFC @%d %d\n",empilert($1,0,1,1) ,$1);
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
		if ((  addr =recherchet_spec($1,&abs_rel,&typage_var))==-1)
		{
			yyerror("ERROR WHEN SEARCH VARIABLE %s\n",$1) ;
		}else 
		{				
			/*if(get_initialiser_var(addr+getNombredevariableglobale()+1)==0)
			{
				yyerror("Problème %s n'est pas initialisée\n", $1);
			}*/
			//else
			//{
				empilert(addr,1,abs_rel,typage_var);
				$$.adresse= addr;
				$$.relative_ou_absolue=abs_rel ; // absolue
				$$.typage_var=typage_var; 
				
			//} 
		} 
	}
	|tNULL{$$.adresse=NULL_2;
	       $$.relative_ou_absolue=1 ; // absolue
	       $$.typage_var=1; 
		}
	|Dereferencement
	      {			
			int abs_rel, typage_var	;
			int retour=dereferencement_expr($1, &abs_rel, &typage_var);
			if(retour==-1)
			{
				yyerror("ERROR : Variable %s non existante \n",$1) ;
			}
			else if (retour==-2)
			{
				yyerror("Problème %s n'est pas initialisé\n", $1);
			}
			else
			{
				//On renvoi toujurs adresse de pile
				$$.adresse=retour;
				//printf("Valeur addr : %d\n", retour);
				$$.relative_ou_absolue=1;
				$$.typage_var=1; 
			}
		}		
	|AppelFonctions 
	{
		//APPEL_FONCTION_IN_EXPRESSION_ASM();
		$$.adresse=$1.adresse;//le résulat se trouve toujours à l'adresse 0!
		$$.relative_ou_absolue =$1.relative_ou_absolue; //qui est toujours une valeur relative
		$$.typage_var=$1.typage_var;
	}
	|AccesCase
	|ExpressionPointeur	 
	{
		$$.adresse=expression_ptr_expr($1.adresse);
		$$.relative_ou_absolue=$1.relative_ou_absolue;
		$$.typage_var=$1.typage_var;
	}					
  | Expression tADD Expression 
  	{	
		
		int retour_1= verification($1.adresse, $1.relative_ou_absolue);
		if(retour_1==-2)
		{
			yyerror("Déréférencement incorrect\n");
		}
		else if(retour_1==-1)
		{
			yyerror("Vous devez accéder à une case du tableau à droite de l'affectation\n");
		}
		int retour_2=verification($3.adresse, $3.relative_ou_absolue);
		if(retour_2==-2)
		{
			yyerror("Déréférencement incorrect\n");
		}
		else if(retour_2==-1)
		{
			yyerror("Vous devez accéder à une case du tableau à droite de l'affectation\n");
		}
		/*if(verif_niv_ptr($1.adresse, $3.adresse, $1.relative_ou_absolue, $3.relative_ou_absolue)==-1)
		{
			yyerror("Probleme éléments de niveau de pointeurs différents\n");
		} */  		
		int retour=operation_arithmetique_asm("ADD", &($$.adresse),&($$.relative_ou_absolue),&($$.typage_var) );
		if(retour==-1)
   		{
   			yyerror("ERROR WHEN POP OPERANDS\n");
   		}
		else if(retour==-2)
		{
			yyerror("Probleme éléments de type différents dans l'opération\n");
		}
		else if(retour==-3)
		{
			yyerror("Probleme de déréférencements\n");
		};
		int addr1=$1.adresse, addr3=$3.adresse;
		if( $1.relative_ou_absolue==0){addr1=addr1+getNombredevariableglobale()+1;}
		if( $3.relative_ou_absolue==0){addr3=addr3+getNombredevariableglobale()+1;}
		reinit_champ_pointeur(addr1);reinit_champ_pointeur(addr3);
		}
 
 
 
  | Expression tSUB Expression 
  	{
//<<<<<<< HEAD
   		//if (operation_arithmetique_asm("SOU", &($$.adresse),&($$.relative_ou_absolue),&($$.typage_var))==-1)
//=======
		int retour_1= verification($1.adresse, $1.relative_ou_absolue);
		if(retour_1==-2)
		{
			yyerror("Déréférencement incorrect\n");
		}
		else if(retour_1==-1)
		{
			yyerror("Vous devez accéder à une case du tableau à droite de l'affectation\n");
		}

		int retour_2=verification($3.adresse, $3.relative_ou_absolue);
		if(retour_2==-2)
		{
			yyerror("Déréférencement incorrect\n");
		}
		else if(retour_2==-1)
		{
			yyerror("Vous devez accéder à une case du tableau à droite de l'affectation\n");
		}
		/*if(verif_niv_ptr($1.adresse, $3.adresse, $1.relative_ou_absolue, $3.relative_ou_absolue)==-1)
		{
			yyerror("Probleme éléments de niveau de pointeurs différents\n");
		}   	*/	
		int retour=operation_arithmetique_asm("SOU", &($$.adresse),&($$.relative_ou_absolue),&($$.typage_var) );
		if(retour==-1)
//>>>>>>> 3c65610403ddc4ee5af857ed36986294dc93881a
   		{
   			yyerror("ERROR WHEN POP OPERANDS\n");
   		}
		else if(retour==-2)
		{
			yyerror("Probleme éléments de type différents dans l'opération\n");
		}else if(retour==-3)
		{
			yyerror("Probleme de déréférencements\n");
		}
		int addr1=$1.adresse, addr3=$3.adresse;
		if( $1.relative_ou_absolue==0){addr1=addr1+getNombredevariableglobale()+1;}
		if( $3.relative_ou_absolue==0){addr3=addr3+getNombredevariableglobale()+1;}
		reinit_champ_pointeur(addr1);reinit_champ_pointeur(addr3);
		}
  
  | Expression tMUL Expression
  	{
		int retour_1= verification($1.adresse, $1.relative_ou_absolue);
		if(retour_1==-2)
		{
			yyerror("Déréférencement incorrect\n");
		}
		else if(retour_1==-1)
		{
			yyerror("Vous devez accéder à une case du tableau à droite de l'affectation\n");
		}
		int retour_2=verification($3.adresse, $3.relative_ou_absolue);
		if(retour_2==-2)
		{
			yyerror("Déréférencement incorrect\n");
		}
		else if(retour_2==-1)
		{
			yyerror("Vous devez accéder à une case du tableau à droite de l'affectation\n");
		}
		/*if(verif_niv_ptr($1.adresse, $3.adresse, $1.relative_ou_absolue, $3.relative_ou_absolue)==-1)
		{
			yyerror("Probleme éléments de niveau de pointeurs différents\n");
		}*/
   		int retour=operation_arithmetique_asm("MUL", &($$.adresse),&($$.relative_ou_absolue),&($$.typage_var) );
		if(retour==-1)
   		{
   			yyerror("ERROR WHEN POP OPERANDS\n");
   		}
		else if(retour==-2)
		{
			yyerror("Probleme éléments de type différents dans l'opération\n");
		}else if(retour==-3)
		{
			yyerror("Probleme de déréférencements\n");
		}
		int addr1=$1.adresse, addr3=$3.adresse;
		if( $1.relative_ou_absolue==0){addr1=addr1+getNombredevariableglobale()+1;}
		if( $3.relative_ou_absolue==0){addr3=addr3+getNombredevariableglobale()+1;}
		reinit_champ_pointeur(addr1);reinit_champ_pointeur(addr3);
		}
														
  															
  | Expression tDIV Expression 
  	{
		int retour_1= verification($1.adresse, $1.relative_ou_absolue);
		if(retour_1==-2)
		{
			yyerror("Déréférencement incorrect\n");
		}
		else if(retour_1==-1)
		{
			yyerror("Vous devez accéder à une case du tableau à droite de l'affectation\n");
		}
		int retour_2=verification($3.adresse, $3.relative_ou_absolue);
		if(retour_2==-2)
		{
			yyerror("Déréférencement incorrect\n");
		}
		else if(retour_2==-1)
		{
			yyerror("Vous devez accéder à une case du tableau à droite de l'affectation\n");
		}
		/*if(verif_niv_ptr($1.adresse, $3.adresse, $1.relative_ou_absolue, $3.relative_ou_absolue)==-1)
		{
			yyerror("Probleme éléments de niveau de pointeurs différents\n");
		}*/
   		int retour=operation_arithmetique_asm("DIV", &($$.adresse),&($$.relative_ou_absolue),&($$.typage_var) );
		if(retour==-1)
   		{
   			yyerror("ERROR WHEN POP OPERANDS\n");
   		}
		else if(retour==-2)
		{
			yyerror("Probleme éléments de type différents dans l'opération\n");
		}else if(retour==-3)
		{
			yyerror("Probleme de déréférencements\n");
		}
		int addr1=$1.adresse, addr3=$3.adresse;
		if( $1.relative_ou_absolue==0){addr1=addr1+getNombredevariableglobale()+1;}
		if( $3.relative_ou_absolue==0){addr3=addr3+getNombredevariableglobale()+1;}
		reinit_champ_pointeur(addr1);reinit_champ_pointeur(addr3);
		}
	
  
  | tSUB Expression %prec NEG  
		{
			int retour_1= verification($2.adresse,$2.relative_ou_absolue);
			if(retour_1==-2)
			{
				yyerror("Déréférencement incorrect\n");
			}
			else if(retour_1==-1)
			{
				yyerror("Vous devez accéder à une case du tableau à droite de l'affectation\n");
			}
			nombre_negatif_asm(&($$.adresse),&($$.relative_ou_absolue),&($$.typage_var));	
			int addr1;			
			if( $2.relative_ou_absolue==0){addr1=addr1+getNombredevariableglobale()+1;}
			reinit_champ_pointeur(addr1);					
		}
 	
	| tPO Expression tPF   
  	{
		int retour_1= verification($2.adresse,$2.relative_ou_absolue);
		if(retour_1==-2)
		{
			yyerror("Déréférencement incorrect\n");
		}
		else if(retour_1==-1)
		{
			yyerror("Vous devez accéder à une case du tableau à droite de l'affectation\n");
		} 		
		$$.adresse=$2.adresse;
  		$$.relative_ou_absolue=$2.relative_ou_absolue;
		int addr1;			
		if( $2.relative_ou_absolue==0){addr1=addr1+getNombredevariableglobale()+1;}
		reinit_champ_pointeur(addr1);
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
		CONDITION_ASM($2, &($$.adresse),&($$.relative_ou_absolue),&($$.typage_var) , $1.adresse,$3.adresse, $1.relative_ou_absolue, $3.relative_ou_absolue);
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
		reinitDeclaGlobale();
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
		 //PUSH_PC_ASM();
	}
	ParamAppel tPF 
	{
		if (VERIFICATION_AND_CALL_GENERATION_ASM()==-1)
		{
			yyerror("ERROR WHEN CALL FUNCTION");
			yyerror(getIDprototypeOrImplementationFunction());yyerror("\n");
		}
		initNombreDeParametresAPPEL() ; 
		PushResultFunction();//on empile l'adresse du résultat qui sera toujours 0 (@ relatif)		
		$$.adresse=Registre_retour_fonctions;
		$$.relative_ou_absolue=1;//l'@ du return sera toujours une @ relative et aura la valeur 0  
		$$.typage_var= getTypeRetour() ;
	};

ParamAppel : 
	Expression 
	{
		ajouter_parametreAPPEL($1.typage_var);		 
		CALL_PARAMETERS_ASM($1.adresse, $1.relative_ou_absolue,1);
	} SuiteParamAppel
	|{ CALL_PARAMETERS_ASM_WITHOUT_AARGUMENT() ; };//TODO GERER LE CAS OU LES PARAMETRES SONT VIDES POUR AJOUTER CREER LE APB 

SuiteParamAppel : 
	tVIR Expression
	{
		ajouter_parametreAPPEL($2.typage_var);	
		CALL_PARAMETERS_ASM($2.adresse, $2.relative_ou_absolue,0);
	} SuiteParamAppel
	|;
						
Return : 
	tRETURN /*{ empilert(0,1,0,1); }*/ Expression
	{
		RETURN_ASM ($2.adresse, $2.relative_ou_absolue,$2.typage_var); 
	} tPOINTVIR ;
	 

//Pointeurs

Pointeur : tINT Etoile 	
	   {	
		//printf("valeur de nivPtr: %d\n", get_nivPtr()); 
		if ( declarer_pointeur($2, get_nivPtr())==-1 )  
		{
			yyerror("Ce pointeur est déjà défini ") ;
		} 
		//int abs_rel, typage_var, addr_ptr=recherchet_spec(get_nom_pointeur(), &abs_rel, &typage_var);
			/*printf("Nom pointeur : %s\n", get_nom_pointeur());
			printf("Niveau pointeur ptr : %d\n",get_nivPtr_var(addr_ptr)); 
			printf("Adresse ptr: %d \n", addr_ptr); */ 	
	   } 
	   InitPointeur SuitePointeur ;

SuitePointeur : tVIR Etoile	
		{	
			if ( declarer_pointeur($2, get_nivPtr())==-1 )  
			{
				yyerror("Ce pointeur est déjà défini ") ;
			}
							
		} 
		InitPointeur SuitePointeur
		|tPOINTVIR;
		
InitPointeur : tEGAL ExpressionPointeur
	       {
			//On affaire à une initialisation avec un &
			if(init_pointeur_expression_pointeur($2.adresse, $2.relative_ou_absolue, $2.typage_var)==-1)	
			{
				yyerror("Pointeurs de niveaux différents de chaque côté de l'affectation\n");
			}
	       }
		|
		//Initialisation avec un pointeur déréférencé
		tEGAL Dereferencement				
		{	
			int retour= init_pointeur_dereferencement($2);

			if(retour==-1)
			{
				yyerror("ERROR : Variable %s non existante \n",$2) ;
			}
			else if (retour==-2)
			{	
				yyerror("Problème pointeurs de niveaux différents de chaque côté de l'affectation\n");	
			}
			else if (retour==-3)
			{
				yyerror("Problème ce n'est pas un pointeur à droite\n");	
			}
		}
		|
		tEGAL AccesCase 
		{
			{	
				int retour=init_pointeur_AccesCase();

				if(retour==-1)
				{
					yyerror("Pointeur de niveau différent de part et d'autre du signe égal\n");
				}
				else if (retour==-2)
				{	
					yyerror("il faut un int * à gauche de l'affectation\n");
				}
				else if (retour==-3)
				{
					yyerror("Accès au tableau a planté\n");
				}
			}
		
		}
		|
		tEGAL tID
		{
				int retour=init_pointeur_pointeur($2);

				if(retour==-1)
				{
					yyerror("Problème pointeurs de niveaux différents de chaque côté de l'affectation\n");	
				}
				else if (retour==-2)
				{	
					yyerror("Problème ce n'est pas un pointeur à droite\n");	
				}
			
		}
		|tEGAL tNULL
		{		
			init_pointeur_Null();
		}
		|;


Etoile : SuiteEtoile tID %prec PTR {strcpy($$,$2);};

SuiteEtoile : tMUL{incrementer_niveau_pointeur();} SuiteEtoile 
	      |tMUL{incrementer_niveau_pointeur();};

ExpressionPointeur :  tETCOM tID 
			{ 
				int abs_rel, typage_var;
				$$.adresse=expression_pointeur($2, &abs_rel, &typage_var);
				$$.relative_ou_absolue=abs_rel;
				$$.typage_var= typage_var+1;
			};
		     /*| tETCOM AccesCase //AccesCA
			{
				int abs_rel, typage_var, a;
				$$=get_adresse_pointee_bis();
				a=recherchet_spec(getNomVar($$.adresse), &abs_rel, &typage_var);
				$$.relative_ou_absolue=abs_rel;
				$$.typage_var= typage_var;
			};*/

Dereferencement : {reinitialiser_nivPointeur();} Etoile
			/*
			{if(get_nivPtr()!=get_nivPtr_var(recherche($2)))
			{
				yyerror("Déréférencement incorrect\n");
			}
			else*/
			{
				if(dereferencement($2)==-1)
				{
					yyerror("Vous ne déréférencez pas un pointeur\n");
				}
				strcpy($$,$2);
			};

//Tableau//////////////////////////////////////////////

Tableau : tINT tID tCO 
	{	//Permet de définir la dimension du tableau : si nivPointeur du tableau vaut 2 alors c'est une matrice
		reinitialiser_nivPointeur();
		//Correspond au tableau dans lequel nous allons sauvegarder les dimensions de chaque crochet du tableau
		//Au maximum trentes crochets.
	   	initialiserTabDimension();
	}
	tNOMBREDEC 
	{
		//On ajoute une dimension au tableau, nivPointeur++ pour le tableau déclaré		
		incrementer_niveau_pointeur();
		//On ajoute la dimension dans le TabDimensio, si on a déclaré un tableau : int tab[3] alors on ajoute 3
		//à la première case de TabDmension
		ajouterTabDimension($5);
	} 
	tCF SuiteDecTab	
	{	//Avec SuiteDecTab on va gérer les tableaux de plus de 1 dimension
		//On ajoute maintenant la variable tableau dans la table des symboles
		//str et str2 vont nous permettre d'ajouter des variables du types : nomDuTableauIndice		
		
		int retour = declaration_tab($2);
		if(retour==-1)
		{
			yyerror("ERROR ce tableau est déjà défini");
		}
		else if(retour==-2)
		{
			yyerror("la variable existe déjà\n");
		}

	} 
	Initialisation
	{
		//La déclaration du tableau est terminée, on réinitialise le niveau de la variable globale
		//nivPointeur pour la prochaine déclaration de tableau ou de pointeur
		reinitialiser_nivPointeur();
	} 
	SuiteTableau;

SuiteTableau : tVIR 
	       {
			
	   		initialiserTabDimension();
	       }
	       tID tCO tNOMBREDEC 
	       { 
			incrementer_niveau_pointeur();
			ajouterTabDimension($5);
	       } 
	       tCF SuiteDecTab	
	       {
			int retour = declaration_tab($3);
			if(retour==-1)
			{
				yyerror("ERROR ce tableau est déjà défini");
			}
			else if(retour==-2)
			{
				yyerror("la variable existe déjà\n");
			}
	       }	
	       Initialisation
	       {
			reinitialiser_nivPointeur();
	       } 
	       SuiteTableau
	       | tPOINTVIR;

SuiteDecTab : tCO tNOMBREDEC  
	      { 
			//Si on arrive dans cette règle alors on affaire à un tableau à plus de 1 dimension
			//On incrémente donc la var globale nivPointeur et on ajoute dans la dimension
			//Dans la case suivante de TabDimension
			incrementer_niveau_pointeur(); 
			ajouterTabDimension($2);
	      } 
	      tCF SuiteDecTab
	      | ;


Initialisation : tEGAL tAO
		 {	
			deb_init_tab();
		
		 } 
	         Elements tAF
		 {

			if(fin_init_tab()==-1)
			{
				yyerror("PROblème les cases du tableau n'ont pas toutes été initialisées ou problème d'accolades\n");
			}
		 }	
		 |;

Elements : tNOMBREDEC  
	   {
		reinitialiser_nbelements();
		if(deb_elts($1)==-1)
		{
			yyerror("insertion d'une valeur dans une case hors-limite du tableau\n");
		}
	    } 
            SuiteElements 
	    {
		int retour =fin_elts();
			
		if (retour==-1)
		{
			yyerror("PROBLEME vous avez déclaré un tableau de dimension 1 et vous l'avez mal initialisé, trop de valeurs ou pas assez\n");
		}
		else if (retour==-2) 
		{
			yyerror("PROBLEME une des lignes du tableau contient trop ou pas assez de valeurs\n");
		}
	    }
	    |tAO
	    {		//Ce code concerne l'initialisation d'un tableau de plus de une dimension
			incrementer_nbAO();
			if(get_nbAO()>get_nivPtr())
	  		{
				yyerror("trop d'accolades\n");
	  		}
			//on réinitialise le nombre d'éléments
			reinitialiser_nbelements();
	      } 
	      Elements tAF
	      {
			decrementer_nbAO();			
			//On incrémente de 1 la valeur dans la case de tab_deb_AO de l'accolade que l'on vient de fermer : 				//exemple si on a tab[2][3]={{1,2,4},on vient de fermer une des accolades du 2 de 
			//tab[2][3] on va alors incrémenter de 1 la case [0] de tab_AO_deb			
			incrementer_tab_AO_deb(get_nbAO()-1);	
	      }
	      SuiteElements2;
//vérifier que nb_AO_dim est toujours inférieur à tabDim[0]+1


SuiteElements : tVIR tNOMBREDEC
		{
			if(deb_elts($2)==-1)
			{
				yyerror("insertion d'une valeur dans une case hors-limite du tableau\n");
			}
		 } SuiteElements
		 |;  

SuiteElements2 : tVIR Elements 
		|;

AccesCase : tID tCO tNOMBREDEC
	    {
		int retour=deb_acces_case($1, $3);
		if( retour ==-1)
		{
			yyerror("Vous tentez d'accéder à une case d'un tableau qui n'existe pas\n");
		}
		else if (retour ==-2)
		{
			yyerror("Problème tableau plus accessible\n");
		}
		else if(retour ==-3)
		{
			yyerror("Overflow tableau\n");
		}
	    }
	    tCF SuiteAcces
	    {
		int abs_rel, typage_var;
		$$.adresse=fin_accesCase($1,&abs_rel, &typage_var);
		$$.relative_ou_absolue=abs_rel;
		$$.typage_var= typage_var;
	    }
	    | tINF tID 
		{	
			int retour = debut_accesCase_id($2);
			if (retour==-1)
			{
				yyerror("Variable inexistante\n");
			}
			else if(retour==-2)
			{
					yyerror("Vous devez travailler avec un pointeur\n");
			} 
		}
		tADD tNOMBREDEC tSUP 
		{
			int abs_rel, typage_var;
			int retour = fin_accesCase_id($2, $5, &abs_rel, &typage_var);
			if (retour==-1)
			{
				yyerror("Vous n'accédez pas à une case du tableau avec l'algèbre des pointeurs\n");
			}
			else
			{
				$$.adresse=retour;
				$$.relative_ou_absolue=abs_rel;
				$$.typage_var= typage_var;
			}
		}
		|tID tCO tID 
		{
			int retour=deb_AccesLettre($1, $3);
			if( retour ==-1)
			{
				yyerror("Vous tentez d'accéder à une case d'un tableau qui n'existe pas\n");
			}
			else if (retour ==-2)
			{
				yyerror("Problème tableau plus accessible\n");
			}
		}
		tCF SuiteAcces
		{
			int abs_rel, typage_var;
			$$.adresse=fin_AccesLettre($1,&abs_rel, &typage_var);
			$$.relative_ou_absolue=abs_rel;
			$$.typage_var= typage_var;
		};

SuiteAcces : tCO tNOMBREDEC 	    
            {
		int retour=deb_suite_acces($2);
		if( retour ==-1)
		{
			yyerror("Overflow tableau\n");
		}
		else if (retour ==-2)
		{
			yyerror("déréférencement tableau trop important\n");
		}
            }
	    tCF SuiteAcces 
	|tCO tID tCF 
	{
		fin_suite_acces($2);
	}SuiteAcces
	|;/**/

//Affectation
//On accède à gauche du tableau seulement à des cases on interdit l'accès à des lignes pour ne pas foutre la merde dans 
//les tableaux déclarés
AffectationTab : AccesCase
		{	
			int retour = deb_affectation_tab();
			if(retour==-1)
			{
				yyerror("Problème déréférencement tableau à gauche de l'affectation\n");
			}
		} 
		tEGAL Expression tPOINTVIR
		{	
			int retour=fin_affectation_tab($1.adresse, $1.relative_ou_absolue, $4.adresse, $4.relative_ou_absolue);
			if( retour ==-1)
			{
				yyerror("Problème vous travailler avec des types différents de chaque côté du =\n");
			}
			else if (retour ==-2)
			{
				yyerror("Problème de déréférencement tableau à droite\n");
			}		
		};
			
%%


int yyerror(char *s) {
  printf("%s\n",s);
  exit(1);
}

//<<<<<<< HEAD
int main(int argc, char ** argv) {
  yyparse();
/*=======
int main(void) {
	yyparse();
	printf("\n");
	printTabVar();
>>>>>>> 3c65610403ddc4ee5af857ed36986294dc93881a*/
}
