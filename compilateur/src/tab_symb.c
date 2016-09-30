#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../head/tab_symb.h"
#include "../head/gestion_des_fonctions.h"


int ind = 0; 
int nombreDeVariabeleglobale = -1; 
int indPile = ADRESSE_DEBUTPILE;
int declaGlobale=1;
int lettre_acces_1_bis=0;
int lettre_acces_2_bis=0;

 int profondeur = 0 ;

 char nom_pointeur[TAILLE_NOM_POINTEUR]; 

 int nivPointeur=0;

int nivPointeur_2=0;

int nb_elements_tab=0;

int tab_elements[TAILLE_TAB_ELEMENTS];

int indice_tableau;

int size_tab=0;
int tabDimension[TAILLE];

int indiceDim;

int nb_AO=0;

int nb_elements_Tot;

int tab_AO_deb[TAILLE];

int tab_AO_fin[TAILLE];

char nomTableau[50];

int tabAcces[TAILLE];

int adresse_retour;

int niv_tab;

int adresse_pointee_bis;

int nivPointeur_3=0;

int presence_lettre=0;

int deb=0;

int lettre_acces=0;

int avt_nb=0;

int lettre_acces_2=0;


char mode[TAILLE]="Pasfonction";
int taille_type_retour_fonction = 0; 
/*int  ind_nb_variable_local = 0 ; 
 int nbParametre = 0 ;*/


// int profondeur = 0 ;

int ajouter_Var(char *nom_var,int type ,int initialiser ,int constante ) 
{

	int i =0 ;

	if (ind!=0)
	{
		for (i=0 ; i<=ind-1;i++)
		{
			if ( strcmp(Array[i].nom_var,nom_var)==0)
				return -1 ;
		}
	}
	
	strcpy(Array[ind].nom_var,nom_var); 
	Array[ind].type = type ; 
	Array[ind].initialiser=initialiser; 
	Array[ind].constante = constante ;
	Array[ind].pointeur = 0 ;
	Array[ind].tableau = 0 ;
	Array[ind].nivPtr = 0 ;
	ind++;

	return 0 ; 
}

int ajouter_Var_spec(char *nom_var,int type ,int initialiser ,int constante, int pointeur, int nivPtr, int tableau)
{
	int i =0 ;

	if (ind!=0)
	{
		for (i=0 ; i<=ind-1;i++)
		{
			if ( strcmp(Array[i].nom_var,nom_var)==0)
				return -1 ;
		}
	}
	
	strcpy(Array[ind].nom_var,nom_var); 
	Array[ind].type = type ; 
	Array[ind].initialiser=initialiser; 
	Array[ind].constante = constante ;
	Array[ind].pointeur = pointeur ; 
	Array[ind].nivPtr = nivPtr ; 
	Array[ind].tableau=tableau;
	if(nivPtr>=1 && tableau ==1)
	{
		getTabDim(); 
		if(ind+1<TAILLE_TAB_SYMBOL-1)
		{
			Array[ind].adresse_deb_tab=ind+1;
		}
	}
	
		Array[ind].adresse_pointee=NULL_2;
	
	ind++;

	return 0 ; 

}

int recherche(char * nom_var) {
	int i =0 ;
	//int retour =-1 ; 

	for (i=0 ; i<=ind;i++)
	{
		if ( (strcmp(Array[i].nom_var,nom_var)==0 ))
			return i ; 
	}

	return -1 ; 
} // pour adresse pointée 


/*relative_ou_absolue : 0 relative , 1 :absolue*/
int recherchet(char * nom_var , int * relative_ou_absolue ,int * typage_var )
{
	int i =0 ;
	//int retour =-1 ; 
	int retour =-1 ;

	for (i=0 ; i<=ind-1;i++)
	{
		if ( (strcmp(Array[i].nom_var,nom_var)==0 ))
		{
			if (i <=nombreDeVariabeleglobale)
			{
				*relative_ou_absolue= 1 ;
			}else 
			{
				*relative_ou_absolue = 0; 
			}
			*typage_var=Array[i].type ;
			retour=  i ;
		} 
	}

	if(strcmp(getMode(),"Pasfonction")==0)//pas en mode fonction, c'est une global
	{
		//printf("======je suis dans le if\n");
		return retour ; 
	}else
	{
		//printf("======je suis dans le if========== et nombre de var %d  et retour %d \n",nombreDeVariabeleglobale,retour );
		//if(strcmp(nom_var,"a")==0) printf("ret : %d / nbglo %d \n",retour,nombreDeVariabeleglobale);
		if (retour - nombreDeVariabeleglobale -1 <0)
		{
			return -1 ;
		}else 
		{
			return retour - nombreDeVariabeleglobale -1 /*+ getTailleTypeRetourFonction()*/;
		}
		
	}
	
}

int recherchet_spec(char * nom_var , int * relative_ou_absolue ,int * typage_var  )
{
	int i =0 ;
	//int retour =-1 ; 
	int retour =-1 ;

	/*if(strcmp(nom_var, "b")==0)
	{
		printf("Nom_var %s \n", nom_var);
	}*/
	for (i=0 ; i<=ind-1;i++)
	{
		if ( (strcmp(Array[i].nom_var,nom_var)==0 ))
		{
			if (i <=nombreDeVariabeleglobale)
			{
				*relative_ou_absolue= 1 ;
			}else 
			{
				*relative_ou_absolue = 0; 
			}
			if(Array[i].pointeur==1)
			{
				*typage_var=Array[i].nivPtr+1; 
			}
			else
			{
				*typage_var=Array[i].type; 
			}
			retour=  i ;
			/*if(strcmp(nom_var, "b")==0)
			{
				printf("valeur de retour %d \n", retour);
			}*/

		} 
	}
	/*if(strcmp(nom_var, "gt")==0)
	{
		printf("valeur de reour : %d\n", retour);
		printf("valeur du mode : %s \n", getMode());
		printf("----------------- %d \n", nombreDeVariabeleglobale);
	}*/
	if(strcmp(getMode(),"Pasfonction")==0)//pas en mode fonction, c'est une global
	{
		return retour ; 
	}else
	{
		//printf("////// nombre de var %d  et retour %d \n",nombreDeVariabeleglobale,retour );
		//if(strcmp(nom_var,"a")==0) printf("ret : %d / nbglo %d \n",retour,nombreDeVariabeleglobale);
		if (retour <=  nombreDeVariabeleglobale)
		{
			return retour ;
			//printf("haere ");
		}
		else if(retour - nombreDeVariabeleglobale -1 /*+ getTailleTypeRetourFonction()*/<0)
		{
			//printf("===========\n");
			return -1;
		}
		else 
		{
			//printf("valeur de retour %d \n", retour);
			return retour - nombreDeVariabeleglobale -1 /*+ getTailleTypeRetourFonction()*/;
		}
		
	}
}
 
void printTabVar()
{
	
	int i =0 ;
	//printf("indice %d  ",ind);
	for (i=0 ; i<=ind-1;i++)
	{
		printf("%s %d %d\n", Array[i].nom_var, Array[i].pointeur, Array[i].nivPtr );
	}
}

int  empilert(int value ,int type ,int abs_ou_rel,int typage_var )
{
	char buf[32];
	sprintf(buf,"%d",value);
	strcpy(Array[indPile].nom_var,buf);
	Array[indPile].type_push=type;
	Array[indPile].type=typage_var;
	Array[indPile].abs_rel=abs_ou_rel ;
	indPile--;  
	return indPile+1 ; 
} 

int empiler(int value , int type  ) {return 0 ;} // pour debug à effacer 


int depiler(int *  type_val_depile ) {return 0 ;} // pour debug à effacer 

int depilert(int *  type_val_depile , int *  abs_ou_rel ,int * type_var )
{
	 //* adresse = indPile; 
	if(indPile<=ADRESSE_DEBUTPILE) 
	{
			indPile++;
			*type_val_depile= Array[indPile].type_push;
			*type_var =  Array[indPile].type;
			*abs_ou_rel=Array[indPile].abs_rel;
			//printf("adr ou l'on cherche type:%d\n",indPile);
	return atoi(Array[indPile].nom_var);
	}
	
	return -1 ; 	
} 

int obtenirAdressePremierOperande()
{
	return indPile;
}

int obtenirAdressDeuxiemeOperande()
{
	return indPile-1;
}

void incrementerIndicePile()
{
	indPile--; 
}

void modifierChampInitialiserVariable(char * nom_var)
{
	int i =0 ;

	
		for (i=0 ; i<ind;i++)
		{
			if ( strcmp(Array[i].nom_var,nom_var)==0)
				Array[i].initialiser=1;
		}
}

int getAdressePile()
{
	return indPile;
}
 


void viderPile()
{
	indPile=ADRESSE_DEBUTPILE;
}

/*int  getAdressePremierOperandeCondition()
{
	return ADRESSE_DEBUTPILE+1;
}
int  getAdresseDeuxiemeOperandeCondition()
{
	return ADRESSE_DEBUTPILE+2;
}

int getAdresseResultatComparaison()
{
	return ADRESSE_DEBUTPILE+3;
}*/

void  generer_fichier_table_des_symboles()
{
	FILE* fichier = NULL;
 	int i ;
    fichier = fopen("fichier_table_des_symbols", "w+");
 
    if (fichier != NULL)
    {
    	//printf("******indice : %d"indiceLabel)
    	for ( i = 0; i < ind; ++i)
    	{
    		fprintf(fichier,"%s %d %d %d\n",Array[i].nom_var,Array[i].type ,Array[i].initialiser,Array[i].constante);
    	}
    	 fclose(fichier);
	}
}


/*void empilerProfondeur()
{
	TAB_PROFONDEUR[profondeur]=ind; 
	profondeur++;
}
int depilerProfondeur()
{
	profondeur--; 
	return TAB_PROFONDEUR[profondeur] ;
} */

void setNombredevariableglobale()
{

	nombreDeVariabeleglobale = ind-1;
}

int getNombredevariableglobale()
{
	return nombreDeVariabeleglobale ;
}

void setIndiceTableLable()
{

	 ind = nombreDeVariabeleglobale ;
}

int getAdresseRetour(int nombres_de_casse_a_reserver) 
{
	int retour =  indPile ;
	 indPile=  indPile - nombres_de_casse_a_reserver ;
	return retour ;
} 

void changeMode()
{
	if (strcmp(mode,"Pasfonction")==0)
	{
		strcpy(mode,"fonction");
	}/*else 
	{
		strcpy(mode,"Pasfonction");
	}*/
}

char * getMode()
{
	return mode ; 
}


void initParametreForNewfunction() 
{
	
	taille_type_retour_fonction =0;
	ind=nombreDeVariabeleglobale+1;
	//nb_variable_local_deja_decalare=0 ; 
	//nbParametre =0; 
}


void setTailleTypeRetourFonction(int taille)
{
	taille_type_retour_fonction=taille ; 
}

int getTailleTypeRetourFonction()
{
	return taille_type_retour_fonction ; 
}


int getNombreDeVariableLocales()
{
	return ind - nombreDeVariabeleglobale+1;
}

/*void IncrementeNbVariableLocalDeClare()
{
	nb_variable_local_deja_decalare++;
}

int getNbVariableLocalDeClare()
{
	return nb_variable_local_deja_decalare ;
}

void IncrementeNBParametre()
{
	nbParametre++;
}

int getNbParametre()
{
	return nbParametre ;
}*/



//global_or_local =1veut dire que l'on écrit les variables globales dans le fichier 
void WriteDebugInformationAboutFunction(int global_or_local)
{
	FILE* fichier = NULL;
    fichier = fopen("file_debug_information", "a+"); //à vérifier 
   int i ; 
 	if (fichier!=NULL)
 	{
 		if(global_or_local==1)
 		{
 			if (nombreDeVariabeleglobale!=-1)//si on a des globales 
 			{
 				fprintf(fichier,"global : %d\n",nombreDeVariabeleglobale+1); //on indique que c'est les variables globales et on note leur nombres
 				for ( i = 0; i <= nombreDeVariabeleglobale; ++i)
 				{
 					fprintf(fichier,"%s %d\n",Array[i].nom_var,i); //nom adresse
 				}
 			}
 		}else //on écrit les locales
 		{
 			fprintf(fichier,"%s : %d\n", getIDprototypeOrImplementationFunction(),ind - (nombreDeVariabeleglobale+1) );//nom_fonction nombres de variables locales 
 			for ( i = nombreDeVariabeleglobale+1; i < ind; ++i)
 			{
 				fprintf(fichier,"%s %d\n",Array[i].nom_var,i - nombreDeVariabeleglobale-1  ); //nom adresse
 			}
 		} 
 	}

 	fclose(fichier);
}

void empilerProfondeur()
{
	TAB_PROFONDEUR[profondeur]=ind; 
	profondeur++;
}
int depilerProfondeur()
{
	profondeur--; 
	return TAB_PROFONDEUR[profondeur] ;
} 

void incrementer_niveau_pointeur()
{
	nivPointeur++;
}

int get_nivPtr()
{
	return nivPointeur;
}

void reinitialiser_nivPointeur()
{
	nivPointeur=0;
}

int get_nivPtr_var(int adresse)
{
	return Array[adresse].nivPtr;
}

int get_Pointeur(int adresse)
{
	return Array[adresse].pointeur;
}

void deplacer_indice(int dep)
{
	ind=ind+dep;
}

int get_indice()
{
	return ind;
}

void incrementer_nbelements()
{
	nb_elements_tab++;
}

void reinitialiser_nbelements()
{
	nb_elements_tab=0;
}

int get_nbelements()
{
	return nb_elements_tab;
}




void taille_tableau(int taille)
{
	int i;

	//tab_elements=malloc(sizeof(int)*taille);
	indice_tableau=0;
	size_tab=taille;
	for(i=0; i<taille; i++)
	{
		tab_elements[i]=0;	
	}
}



void remplir_tableau(int element)
{
	tab_elements[indice_tableau]=element;
	indice_tableau++;
}

int get_tailleTab()
{
	return size_tab;
}

int get_element(int indice)
{
	return tab_elements[indice];
}

char * getNomVar(int addr)
{
	return Array[addr].nom_var;
}

int recherche_tableau(char * nom)
{
	int i =0 ;
	//int retour =-1 ; 

	for (i=0 ; i<=ind;i++)
	{
		if ( (strcmp(Array[i].nom_var,nom)==0 && Array[i].tableau==1))
			return i ; 
	}

	return -1 ; 
}

void ajouterTabDimension(int dim)
{
	//printf("Bonjour\n");
	tabDimension[indiceDim]=dim;
	indiceDim++;
}

void initialiserTabDimension()
{
	int i;
	for(i=0; i<TAILLE; i++)
	{
		tabDimension[i]=0;
	}
	indiceDim=0;
}

int getTotalDim(addr)
{
	int res=1,i;
	for(i=0; i<TAILLE; i++)
	{
		if (Array[addr].tabDim[i]!=0)
		{		
			
			res=res*Array[addr].tabDim[i];
		}
	}
	return res;
}

int getCaseTabDim(int j)
{
	return tabDimension[j];
}

void decrementer_niveau_pointeur()
{
	nivPointeur--;
}

void getTabDim()
{
	int j;
	for(j=0; j<TAILLE;j++)
	{	
		Array[ind].tabDim[j]=tabDimension[j];
		//printf("--------------- %d \n", Array[ind].tabDim[j]);
	}
}

void incrementer_nbAO()
{
	nb_AO++;
}

void decrementer_nbAO()
{
	nb_AO--;
}

int get_nbAO()
{
	return nb_AO;
}

void reinitialiser_nbAO()
{
	nb_AO=0;
}

int recherche_tableau_dans_mem()
{
	int i=ind-1;	
	while(Array[i].nivPtr==0){i--;}
	return i;
}

void initialiser_tab_AO_deb()
{
	int i;
	for(i=0; i<TAILLE; i++)
	{
		tab_AO_deb[i]=0;
	}
}

void initialiser_tab_AO_fin(int addr)
{
	int i;
	for(i=0; i<TAILLE; i++)
	{
		if(i==0)
		{
			tab_AO_fin[0]=Array[addr].tabDim[0];
		}
		else
		{
			tab_AO_fin[i]=Array[addr].tabDim[i]*tab_AO_fin[i-1];
		}
	}
}

void incrementer_tab_AO_deb(int adresse)
{
	tab_AO_deb[adresse]++;
}

int get_indice_nb_elements(int addr)
{
	int i;
	while(i<TAILLE && Array[addr].tabDim[i]!=0)
	{
		i++;
	}
	if(i<TAILLE)
	{
		i--;
		return i;
	}
	else
	{
		return -1;
	}
}

int get_tab_AO_deb_element(int i)
{
	return tab_AO_deb[i]; 
}

int get_tab_AO_fin_element(int k)
{
	return tab_AO_fin[k]; 
}

void print_tabElements(int addr)
{
	int j;
	printf("tab_Elements  : ");	
	for(j=0; j<getTotalDim(addr);j++)
	{
		printf(" %d |", tab_elements[j]);
	}
	printf("\n");
}

void print_tabDim_var(int addr)
{
	int j;
	printf("tab_Dim  : ");	
	for(j=0; j<TAILLE;j++)
	{
		printf(" %d |", Array[addr].tabDim[j]);
	}
	printf("\n");
}

int recuperer_nb_element_par_ligne(int addr)
{
	int j=get_indice_nb_elements(addr);
	return Array[addr].tabDim[j];

}

int get_nbelements_Tot()
{
	return nb_elements_Tot;
}

void incrementer_nb_elements_Tot()
{
	nb_elements_Tot++;
}

void reinitialiser_nb_elements_Tot()
{
	nb_elements_Tot=0;
}

int test_tab_deb_fin_AO()
{
	int i;
	int res=0;
	for(i=0; i<TAILLE; i++)
	{
		if(tab_AO_deb[i]!=tab_AO_fin[i])
		{
			res=1;
		}
	}
	return res;
}

void print_tab_deb_AO()
{
	int j;
	printf("tab_AO_deb  : ");	
	for(j=0; j<TAILLE;j++)
	{
		printf(" %d |", tab_AO_deb[j]);
	}
	printf("\n");
}

void print_tab_fin_AO()
{
	int j;
	printf("tab_AO_fin  : ");	
	for(j=0; j<TAILLE;j++)
	{
		printf(" %d |", tab_AO_fin[j]);
	}
	printf("\n");
}

void maj_indiceDIm(int addr)
{
	indiceDim=get_indice_nb_elements(addr);
}

int get_indiceDIm()
{
	return indiceDim;
}

void initialiser_nom_tableau(char * nom)
{
	strcpy(nomTableau, nom);
}

char * get_nomTableau()
{
	return nomTableau;
}

void init_tabAcces(int addr)
{
	int i,j, res=1;
	for(i=0;i<TAILLE; i++)
	{
		j=i;		
		while(j<TAILLE && (Array[addr].tabDim[j] !=0))
		{			
			res=res*Array[addr].tabDim[j];
			j++;		
		}
		if(Array[addr].tabDim[i] !=0)
		{		
			tabAcces[i]=res/Array[addr].tabDim[i];	
		}
		else
		{
			tabAcces[i]=0;
		}
		res=1;	
	}
	
}

void reinit_tabAcces()
{
	int i;
	for(i=0; i<TAILLE; i++)
	{
		tabAcces[i]=0;
	}
}

int get_val_tabAcces(int i)
{
	return tabAcces[i];
}

int dimension(char * nom_tableau, int num_case)
{
	return (Array[recherche(nom_tableau)].tabDim[num_case]);
}

void init_adresse_retour(int addr_courante)
{
	adresse_retour=addr_courante;
}

void incrementer_adresse_retour(int nb)
{
	adresse_retour=adresse_retour+nb;
}

int get_adresse_retour()
{
	return adresse_retour;
}

void reinit_adresse_retour()
{
	adresse_retour=0;
}

void print_tab_Acces()
{
	int j;
	printf("tab_Acces  : ");	
	for(j=0; j<TAILLE;j++)
	{
		printf(" %d |", tabAcces[j]);
	}
	printf("\n");
}

void modifierChampInitialiserVar(int addr)
{
	if(Array[addr].initialiser!=1)
	{
		Array[addr].initialiser=1;
	}
}

void reinitialiser_nivTab()
{
	niv_tab=0;
}

int get_niv_tab()
{
	return niv_tab;
}

void initialiser_niv_tab(int addr)
{	
	niv_tab=Array[addr].nivPtr;
}

int get_initialiser_var(int addr)
{
	return Array[addr].initialiser;
}

void init_nom_pointeur(char * nom)
{
	strcpy(nom_pointeur, nom);
}

void reinit_nom_pointeur()
{
	int i;
	for(i=0; i< TAILLE_NOM_POINTEUR; i++)
	{ 	
		nom_pointeur[i]='\0';
	}
}

char * get_nom_pointeur()
{
	return nom_pointeur;
}

void init_niveau_pointeur_2(int niv)
{
	nivPointeur_2=niv;
}

int get_nivPtr_2()
{
	return nivPointeur_2;
}

void reinitialiser_nivPointeur_2()
{
	nivPointeur_2=0;
}

void tout_NivPtr_zero()
{
	int i;
	for(i=0; i<TAILLE_TAB_SYMBOL; i++)
	{
		Array[i].nivPtr=0;
	}
}

int verifier_Acces_case_OP(int addr)
{
			//On vérifie que si Expression était un AccesCase, on a bien accédé à une valeur contenue
			//dans le tableau
			if(get_nivPtr()!=get_niv_tab() && get_niv_tab()!=0)
			{
				//yyerror("Vous devez accéder à une case du tableau à droite de l'affectation\n");
				return -1;			
			}
			return 0;
			/*else
			{	
				//on vérifie que la case du tableau contient bien une valeur ssi on a affaire à un tableau
				//ce code ne sera donc exécuté que si Expression est un AccesCase
				if(get_initialiser_var(addr)!=1 && get_niv_tab()!=0)
				{
					yyerror("Case du tableau non initialiser\n");
				}
				
				
			}*/
} 

int verifier_Dereferencement_OP(int addr)
{

	//printf("Niveau Ptr : %d\n", get_nivPtr());	
	//printf("Niveau Ptr  Var : %d\n", get_nivPtr_2());
	//printf("niveau tab : %d\n", get_niv_tab());
	//On vérifie que l'on a correctement déréférencé et que l'on accède à une valeur pointée
	//Ne peut être exécuté que pour un déréférencement grâce à get_niv_tab()==0
	if(get_Pointeur(addr)==1)
	{
		if(get_nivPtr()!=get_nivPtr_2() && get_niv_tab()==0)
		{
			//yyerror("Déréférencement incorrect\n");
			return -1;
		}
		else
		{
			/*if(get_initialiser_var(addr)!=1 && get_nivPtr_var(addr)!=0)
			{
				yyerror("valeur pointeur non initialisée\n");
			}
			else
			{*/
				reinitialiser_nivPointeur();
				return 0;
			//}
		}
	}
}	

void modifier_champ_init_deref(int addr)
{
	Array[addr].initialiser=1;
}

void reinit_champ_init_deref(int addr)
{
	Array[addr].initialiser=0;
}

void init_champ_pointeur(int addr, int val)
{
	Array[addr].nivPtr=val;
}

void reinit_champ_pointeur(int addr)
{
	Array[addr].nivPtr=0;
}

int get_adresse_tab(int addr)
{
	return Array[addr].adresse_deb_tab;
}

int get_champ_tab(int addr)
{
	return Array[addr].tableau;
}

void modi_adresse_deb_tab(int addr, int a)
{
	Array[addr].adresse_deb_tab=a;
}

int get_adresse_pointee(int addr)
{
	return Array[addr].adresse_pointee;
}

void init_adresse_pointee(int addr, int a)
{
	Array[addr].adresse_pointee=a;
}

int get_adresse_pointee_bis()
{
	return adresse_pointee_bis;
}

void init_adresse_pointee_bis(int a)
{
	adresse_pointee_bis=a;
}

void init_niveau_pointeur_3(int niv)
{
	nivPointeur_3=niv;
}

int get_nivPtr_3()
{
	return nivPointeur_3;
}

void reinitialiser_nivPointeur_3()
{
	nivPointeur_3=0;
}

void initialiser_deb_nombre()
{
	deb=0;
}

void initialiser_deb_lettre()
{
	deb=1;
}

int get_deb()
{
	return deb;
}

void initialiser_presence_lettre()
{
	presence_lettre=1;
}

int get_presence_lettre()
{
	return presence_lettre;
}

void reinit_presence_lettre()
{
	presence_lettre=0;
}

int get_lettre_acces()
{
	return lettre_acces;
}

void reinit_lettre_acces()
{
	lettre_acces=0;
}

void init_get_lettre_acces()
{
	lettre_acces=1;
}

int get_avt_nb()
{
	return avt_nb;
}

void reinit_avt_nb()
{
	avt_nb=0;
}

void init_avt_nb()
{
	avt_nb=1;
}

int get_lettre_acces_2()
{
	return lettre_acces_2;
}

void reinit_lettre_acces_2()
{
	lettre_acces_2=0;
}

void init_get_lettre_acces_2()
{
	lettre_acces_2=1;
}

void setDeclaGlobale()
{
	declaGlobale=1;
}

int getDeclaGlobale()
{
	return declaGlobale;
}

void reinitDeclaGlobale()
{
	declaGlobale=0;
}

int get_lettre_acces_1_bis()
{
	return lettre_acces_1_bis;
}

void init_lettre_acces_1_bis()
{
	lettre_acces_1_bis=1;
}

void reinit_lettre_acces_1_bis()
{
	lettre_acces_1_bis=0;
}

int get_lettre_acces_2_bis()
{
	return lettre_acces_2_bis;
}

void init_lettre_acces_2_bis()
{
	lettre_acces_2_bis=1;
}

void reinit_lettre_acces_2_bis()
{
	lettre_acces_2_bis=0;
}

void init_champ_tab(int addr)
{
	Array[addr].tableau=1;
}

void reinit_champ_tab(int addr)
{
	Array[addr].tableau=0;
}

int length_table_symbole()
{
	return  ind - nombreDeVariabeleglobale - 1 ;
}


