Input: Main
			|Fonctions Main;
			 
Declaration : tINT tID SuiteDeclarations  ;

SuiteDeclarations :  tPOINTVIR
									| tVIR tID SuiteDeclarations ;
									

							
Main: tINT tMAIN tPO  tPF Body  ;

Body : tAO SuiteBody tRETURN tID tAF ;
			|tAO SuiteBody tRETURN tNOMBREDEC  tAF ;
			
SuiteBody :  Declaration SuiteBody
						|Affectation  SuiteBody
						|Print SuiteBody
						|If SuiteBody
						|While SuiteBody
						|;
			
Affectation : tID tEGAL Valeur tPOINTVIR ;

Valeur = tNOMBREEXPO
					|tNOMBREDEC ;

Print : tPRINT tPO Valeur tPF tPOINTVIR
				|  tPRINT tPO tID tPF tPOINTVIR ; 
				
//rajouter tIF 	,ie : suiteBody represente les instructions possibles 			
If: tIF tPO Cond tPF tAO SuiteBody tAF Else; 

Else : tELSE tAO SuiteBody tAF
			|;


//rajouter tWHILE
While : tWHILE tPO Cnd tPF tAO SuiteBody tAF ; 

//rajouter tOR et tAND
Cond: ExpAComparer Comparateur ExpAComparer 
			|Cond tOR Cond 
			|Cond tAND Cond;


//reflechir aux nombre décimaux 
ExpAComparer : Valeur 
							|tID ;  
							

//justification: on inclut la déclaration pour ne pas avoir d\'ordre fixe avant le main \(ie decl fonc main ou declar implemnt decla main etc... )
Fonctions:Prototype Fonctions
				|ImplementationFonction Fonctions
				|; 
				
Prototype : tINT tID tPO Params tPF tPOINTVIR;

ImplementationFonction : tINT tID tPO Params tPF Body ;


Params : tIND tID SuiteParams
				|;
				
SuiteParams : tVIR tIND tID SuiteParams
						|;






