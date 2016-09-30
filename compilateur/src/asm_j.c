#include "../head/asm_j.h"
#include "../head/tab_symb.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int declarer_pointeur(char * nom_variable, int niveauPtr)
{
		//printf("valeur de getPtr: %d\n", niveauPtr); 

		if ( ajouter_Var_spec(nom_variable,1,0,0,1,niveauPtr, 0)==-1 )  
		{
			return -1;
		} 
		else
		{	//printf("valeur de getPtr: %d\n", get_nivPtr_var(0));	
			reinit_nom_pointeur();
			//On réinitialise la var niv_pointeur			
			reinitialiser_nivPointeur();
			//On sauvegarde dans une var globale le nom du pointeur, utile pour l'initialisation	
			init_nom_pointeur(nom_variable);
			//printf("Nom 1 : %s\n", nom_variable);
			return 0;
		}	
}

int init_pointeur_expression_pointeur(int dollar_addr, int dollar_relative_ou_absolue,int type_de_la_var)
{
				//printf("Nom 2 : %s\n", get_nom_pointeur());

	int abs_rel, typage_var; 
	int addr_ptr=recherchet_spec(get_nom_pointeur(), &abs_rel, &typage_var);
	//printf("Adresse de dollar : %d\n",dollar_addr);
	//printf("Niveau pointeur ptr : %d\n",get_nivPtr_var(dollar_addr));  
	if(get_nivPtr_var(dollar_addr)==(get_nivPtr_var(addr_ptr+getNombredevariableglobale()+1)))
	{
				
				//Sauvegarde de l'adresse de la variable dans la pile				
				//printf("AFC %d %d\n", empiler($2,0), $2);
				//incrementerPC();
				//On copie l'adresse sauvegardé dans la pile dans la variable pointeur
				// dollar_relative_ou_absolue toujours égaal à 1 car on c'est dans la pile(mémoire des données)
				if  (dollar_relative_ou_absolue==1 && abs_rel==0)
				{
			
					printf( "COP @@%d @%d\n", addr_ptr,dollar_addr);
				} else if  (dollar_relative_ou_absolue==1 && abs_rel==1)
				{
					printf( "COP @%d @%d\n", addr_ptr,dollar_addr);
				}
				init_adresse_pointee(addr_ptr+getNombredevariableglobale()+1,get_adresse_pointee_bis());
				//printf("adresse init : %d\n", get_adresse_pointee(addr_ptr+getNombredevariableglobale()+1));				
				incrementerPC();
				viderPile();
				reinit_champ_pointeur(dollar_addr);
				modifierChampInitialiserVariable(get_nom_pointeur());
				return (0);
	}
	else
	{
		return -1;
	}
}

int init_pointeur_dereferencement(char * nom_deref)
{
			int addr,i, addr1, abs_rel, typage_var, abs_rel_ptr, typage_var_ptr, addr_ptr=recherchet_spec(get_nom_pointeur(), &abs_rel_ptr, &typage_var_ptr) ;
			int typeOp1;
			if ((  addr =recherchet_spec(nom_deref, &abs_rel, &typage_var))==-1)
			{
				return -1;
			}
			else
			{	
				int addr_bis=addr;
				if(abs_rel==0)
				{
					addr_bis=addr+getNombredevariableglobale()+1;
				}
				//Si on initialise bien avec un pointeur			
				if(get_Pointeur(addr_bis)==1)
				{
					//printf("Bonjour\n");
					//On vérifie que les niveaux des pointeurs de part et d'autre du = sont équivalents	
					if(get_nivPtr_var(addr_ptr+getNombredevariableglobale()+1)==(get_nivPtr_var(addr_bis)-get_nivPtr()))
					{
						//On empile l'adresse pointée par le pointeur servant pour l'initialisation	
						addr1=empilert(addr,0,abs_rel_ptr,typage_var  );
						if (abs_rel==0)
						{
							printf( "COP @%d @@%d\n", addr1,addr);
						} else
						{
							printf( "COP @%d @%d\n", addr1,addr);
						}
						incrementerPC();
						init_adresse_pointee(addr1, get_adresse_pointee(addr_bis));				
						//addr=addr1;
						//On accède à la valeur/adresse obtenue suite au déréférencement		
						for(i=0; i<get_nivPtr(); i++)
						{
							printf("COPA @%d @%d\n", addr1, addr1);
							incrementerPC();
							int ad=get_adresse_pointee(addr1);
							init_adresse_pointee(addr1, get_adresse_pointee(ad));
						} 
						//Enfin on affecte cette valeur au pointeur à gauche du =
						if  (abs_rel_ptr==0)
						{
							printf( "COP @@%d @%d\n", addr_ptr,addr1);
						} else
						{
							printf( "COP @%d @%d\n", addr_ptr,addr1);
						}
						init_adresse_pointee(addr_bis, get_adresse_pointee(addr1));
						incrementerPC();
						reinit_champ_pointeur(addr1);
						viderPile();
						modifierChampInitialiserVariable(get_nom_pointeur());
						return 0;
					}
					else
					{
						return -2;
					}
				}
				else
				{
					return -3;	
				}
			}
}

int init_pointeur_AccesCase()
{
		
		//get_nivPtr() renvoi la valeur obtenue après déréférencement du tableau avec AccesCase et get_niv_tab()	
		//Contient la diemnsion du tableau. On vérifie que l'on n'accède pas à une valeur contenue dans le tab
		if(get_nivPtr()<get_niv_tab())
		{
			//On vérifie que les niveaux des pointeurs de chaque côté du = sont équivalents	
			int abs_rel, typage_var;			
			int addr= recherchet_spec(get_nom_pointeur(), &abs_rel, &typage_var);		
			if((get_niv_tab()-get_nivPtr())!=get_nivPtr_var(addr+getNombredevariableglobale()+1))
			{
				return -1;
			}
			else
			{
				//On copie l'adresse empilée suite à AccesCase dans le pointeur à droite de = 		
				if(abs_rel==0)
				{
					printf("COP @@%d @%d\n", addr,getAdressePile()+1);
				}
				else
				{
					printf("COP @%d @%d\n", addr,getAdressePile()+1);
				}				

				incrementerPC();
				init_adresse_pointee(recherche(get_nom_pointeur()),get_adresse_pointee_bis());
				viderPile();
				modifierChampInitialiserVariable(get_nom_pointeur());
				reinitialiser_nivPointeur();
				reinitialiser_nivTab();
				return 0;
			}	
		}
		else if(get_nivPtr()==get_niv_tab())
		{
			return -2;
		}
		else
		{
			return -3;
		}

}

int init_pointeur_pointeur(char * nom_ptr)
{		
		int addr_ptr_gauche, abs_rel_gauche, typage_var_gauche;
		int addr_ptr_droite, abs_rel_droite, typage_var_droite;
		//ici initialisation entre pointeurs de même niveau
		addr_ptr_droite=recherchet_spec(nom_ptr, &abs_rel_droite, &typage_var_droite);
		addr_ptr_gauche=recherchet_spec(get_nom_pointeur(), &abs_rel_gauche, &typage_var_gauche);
		if(get_Pointeur(addr_ptr_droite+getNombredevariableglobale()+1)==1)
		{
			//On vérifie que els deux pointeurs sont de même niveau de part et d'autre du =
			if(get_nivPtr_var(addr_ptr_gauche+getNombredevariableglobale()+1)==get_nivPtr_var(addr_ptr_droite+getNombredevariableglobale()+1))
			{
				//On copie dans le pointeur à droite l'adresse sur laquelle pointe le pointeur à gauche
				if (abs_rel_gauche==0 && abs_rel_droite==0)
				{	
					/*if (strcmp(getMode(),"fonction")==0)
					{
						dollar_addr +=getTailleTypeRetourFonction() ;
					}*/
					printf( "COP @@%d @@%d\n",addr_ptr_gauche,addr_ptr_droite);

				} else if  (abs_rel_gauche==0 && abs_rel_droite==1)
				{
					/*if (strcmp(getMode(),"fonction")==0)
					{
						dollar_addr +=getTailleTypeRetourFonction() ;
					}*/
					printf( "COP @%d @@%d\n",addr_ptr_gauche,addr_ptr_droite);

				} else if  (abs_rel_gauche==1 && abs_rel_droite==0)
				{
			
					printf( "COP @@%d @%d\n",addr_ptr_gauche,addr_ptr_droite);
				} else if  (abs_rel_gauche==1 && abs_rel_droite==1)
				{
					printf( "COP @%d @%d\n",addr_ptr_gauche,addr_ptr_droite);
				}

				init_adresse_pointee(addr_ptr_gauche+getNombredevariableglobale()+1,get_adresse_pointee(addr_ptr_droite+getNombredevariableglobale()+1));
				printf("val pointee : %d\n", get_adresse_pointee(addr_ptr_gauche+getNombredevariableglobale()+1));			
				incrementerPC();
				viderPile();
				modifierChampInitialiserVariable(get_nom_pointeur());
				return 0;
			}
			else
			{
				return -1;	
			}
		}
		else
		{
			return -2;	
		}
}

void init_pointeur_Null()
{
		int addr_ptr_gauche, abs_rel_gauche, typage_var_gauche, addr1;
		//On va faire pointer le pointeur sur la dernière case de la table de symboles		
		printf("AFC @%d %d\n", addr1=empilert(NULL_2,0,1,1 ), NULL_2);
		incrementerPC();
		addr_ptr_gauche=recherchet_spec(get_nom_pointeur(), &abs_rel_gauche, &typage_var_gauche);
		if  (abs_rel_gauche==0)
		{
			printf( "COP @@%d @%d\n", addr_ptr_gauche,addr1);
		} else
		{
			printf( "COP @%d @%d\n", addr_ptr_gauche,addr1);
		}		
		init_adresse_pointee(addr_ptr_gauche+getNombredevariableglobale()+1,NULL_2);	
		incrementerPC();
		viderPile();
}

int expression_pointeur(char * nom_ptr, int * relative_ou_absolue ,int * typage_var )
{
	int addr1, r_a, t_v;
	int addr_ptr=recherchet_spec(nom_ptr, &r_a , &t_v );
	//printf("Adresse pointee : %d\n", addr_ptr + getNombredevariableglobale()+1);
	//il faut voir l'adresse comme une constante
	addr1=empilert(addr_ptr,0 ,r_a,t_v+1); 
	//Sauvegarde de l'adresse de la variable dans la pile	
	printf("AFC @%d %d\n", addr1, addr_ptr/*+getNombredevariableglobale()+1*/);
	incrementerPC();
	init_champ_pointeur(addr1, get_nivPtr_var(addr_ptr+getNombredevariableglobale()+1)+1);
	//printf("valeur de l'adresse %d et son niveau de pointeur %d\n", addr1, get_nivPtr_var(addr1/*+getNombredevariableglobale()+1)*/));
	*relative_ou_absolue=1;
	*typage_var=1;
	init_adresse_pointee_bis(addr_ptr/*+getNombredevariableglobale()+1*/);
	//printf("WHAT %d \n", get_adresse_pointee_bis());
	return(addr1);
}

int dereferencement(char * nomptr)
{
	int abs_rel, typage_var, addr;
	addr=recherchet_spec(nomptr, &abs_rel, &typage_var);
	if(abs_rel==0)
	{
		addr=addr+getNombredevariableglobale()+1;
	}
	if(get_Pointeur(addr)==-1)
	{
		return -1;
	}
	else
	{
		int i=0, addr;
		char str[50];
		strcpy(str, nomptr);
		while(i<get_nivPtr())
		{
			addr=recherche(str);
			init_adresse_pointee_bis(get_adresse_pointee(addr));
			strcpy(str, getNomVar(get_adresse_pointee_bis()));
			i++;
		}
		return 0;
	}

}







///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







int declaration_tab(char * nom_tab)
{
		char str[100], str2[100];
		int i,j=0;
		int typage_var, relative_ou_absolue ;
		//On ajoute dans la table des symboles une variable qui va contenir la dimension et les valeurs de chaque crochet
		if ( ajouter_Var_spec(nom_tab,1,0,0,1,get_nivPtr(), 1)==-1 )  
		{
			return -1;
		} 
		else
		{
			int addr_tab=recherchet_spec(nom_tab, &relative_ou_absolue , &typage_var);
			//Si la variable précédente a bien été ajoutée, on génère la ligne assembleur suivante et
			// on fera pointer cette variable sur la première case du tableau (adresse get_indice())
			if(relative_ou_absolue==0)
			{
				printf( "AFC @@%d %d\n", addr_tab , get_indice());
			}
			else
			{
				printf( "AFC @%d %d\n", addr_tab, get_indice());
			}
			incrementerPC();
			modifierChampInitialiserVariable(nom_tab);
			init_adresse_pointee(addr_tab+getNombredevariableglobale()+1, addr_tab+getNombredevariableglobale()+2);
			//On récupère l'adresse de la var précédente pour avoir le nombre total d'éléments dans le tableau
			int a=get_indice()-1;
			for(i=0; i<getTotalDim(a); i++)
			{
				//On crée des noms de variables du type nomTableauIndice, pour tab[2], on aura tab0 tab1.
				sprintf(str, "%d", i);
				strcpy(str2, nom_tab);
				//On ajoute chacune de ces var dans la table des symboles, on vérifie que des variables
				//de même noms n'existaient pas déjà
				if(ajouter_Var_spec(strcat(str2,str) ,1,0,0,0,0,1)==-1) 
				{
					return -2;
				}
				//printf("adresse pointée : %d\n", get_adresse_pointee(addr_tab+getNombredevariableglobale()+1));
			}
			return 0;	
		}
		
}

void deb_init_tab()
{
			//On récupère l'adresse du dernier tableau inséré dans la table des symboles			
			int addr_tab=recherche_tableau_dans_mem();	
			//On initialise un tableau qui va contenter le nombre d'accolades pour chaque dimension
			//du tableau			
			initialiser_tab_AO_deb();
			//On incrémente de 1 le compteur d'accolades lors de l'initialisation			
			incrementer_nbAO();
			//On initialise un tableau avec le nombre final d'accolades que devra contenir le tableau
			//tab_AO_deb en fin d'initialisation.
			initialiser_tab_AO_fin(addr_tab);
			reinitialiser_nb_elements_Tot();
			//On est dans une phase où l'on va saisir des nombres, on va sauvegarder dans un tableau
		
			//printf("ADDR : %d\n", addr);
			//La récupération de l'adresse du tableau nous permet de calculer le nombre d'éléments du 
			//tableau, on va récupérer le nombre d'éléments sur une ligne du tableau.
			int nb_nombres = recuperer_nb_element_par_ligne(addr_tab);	
		
			taille_tableau(nb_nombres);
			//Indice auquel se trouve le nombre d'élements dans une ligne du tableau
			maj_indiceDIm(addr_tab);
}

int fin_init_tab()
{
	int addr=recherche_tableau_dans_mem();	
			if((get_nbelements_Tot()!=getTotalDim(addr)) || get_nbAO()!=1 || test_tab_deb_fin_AO())
			{
				return -1;
			}
			else
			{
				//Vu qu'on ferme une accolade on décrémente le nombre d'accolades				
				decrementer_nbAO();
				//On va chercher l'adresse du dernier tableau inséré pour récupérer les dimensions
				int relative_absolue, typage_var;				
				int addr_tab=recherchet_spec(getNomVar(addr), &relative_absolue, &typage_var);		
				char str[100], str2[100];
				int i,j=0;
				//On affecte à chaque case du tableau sa valeur, getTotalDIm donne le nombre total
				//d'éléments que peut contenir le tableau		
				for(i=0; i<getTotalDim(addr); i++)
				{
					sprintf(str, "%d", i);
					strcpy(str2, getNomVar(addr_tab)); 
					printf("AFC @%d %d\n", empilert(get_element(i),0, relative_absolue, typage_var), get_element(i));
					incrementerPC();
					if(relative_absolue==0)
					{
						printf("COP @@%d @%d\n", (addr_tab+i+1),getAdressePile()+1);
					}
					else
					{
						printf("COP @%d @%d\n", (addr_tab+i+1),getAdressePile()+1);
					}
					incrementerPC();
					viderPile();
					modifierChampInitialiserVariable(strcat(str2,str));
				}
				return 0;									
			}
			
}

int deb_elts(int dim)
{
	
	int addr=recherche_tableau_dans_mem();
	//si le nombre d'éléments insérer est bien inférieur aux nombres d'éléments que peut contenir une 
	//ligne du tableau
	if(get_nbelements()<get_tailleTab())
	{
		//On va ajouter ce nombre dans le tableau intermédiaire initialiser avec taille_tableau();		
		incrementer_nbelements();
		incrementer_nb_elements_Tot();
		remplir_tableau(dim);
		if(get_indiceDIm()!=-1)
		{
			//On incrémente de 1 le nombres d'éléments dans tab_AO_deb
			incrementer_tab_AO_deb(get_indiceDIm());
		}
		return 0;
		//incrementer de 1 la nième-1 case précédente gérant les dimensions de AO
	}
	else
	{
		return -1;
	}
}

int fin_elts()
{
		int addr=recherche_tableau_dans_mem();
		//Si l'on travaille sur un tableau à une dimension		
		if(get_nivPtr_var(addr)==1)
		{
			//Si le nombre d'éléments ajoutés dans le tableau intermédiare est différent
			//du nombre total d'éléments que peut contenir le tableau			
			if(get_nbelements()!=getTotalDim(addr))
			{
				return -1;
			}
		}
		else
		{
			//Si l'on travaille avec un tableau de plus de une dimension, on va vérifier
			//que le tableau intermédiare contient autant d'éléments que peut contenir une
			//ligne du tableau que l'on a déclaré
			//printf("Nb_elements : %d\n", get_nbelements());
			//printf("taille_tab : %d\n", get_tailleTab());
			if (get_nbelements()!=get_tailleTab()) 
			{
				return -2;
			}
			return 0;		
		}
		

}

int deb_acces_case(char * nom_tab, int num_case)
{
		reinitialiser_nivPointeur();
		int addr, addr_retour;
	    	if( (addr=recherche(nom_tab))==-1)
		{
			return -1;
		}       
		else
		{
			//On vérifie que la valeur entre les crochets est inférieure à la dimension de cette 
			//zone du tableau, exemple soit tab[2][3] on ne peut pas écrire tab[5]
			if(get_adresse_tab(addr)!=addr+1)
			{	
				return -2;
			}
			else
			{
				if(num_case<dimension(nom_tab,get_nivPtr()))
				{

					initialiser_nom_tableau(nom_tab);
					initialiser_deb_nombre();
					int a =recherche(nom_tab);
					init_tabAcces(recherche(nom_tab));
					//print_tab_Acces();	
					init_adresse_retour(recherche(nom_tab)+1);
					incrementer_adresse_retour(num_case*get_val_tabAcces(get_nivPtr()));
					incrementer_niveau_pointeur();
					init_avt_nb();
					return 0;
					//empiler(get_adresse_retour(), 0);
				}
				else
				{
					return -3;
				}
			
			}
		}     

}

int fin_accesCase(char * nom_tab, int * abs_rel, int * typage_var)
{
		int addr_retour;
		if(get_nivPtr()==get_nivPtr_var(recherche(nom_tab)))
	    	{
			//printf("=================== valeur de adresse retour : %d \n", get_adresse_retour());
			if(get_presence_lettre()==1)
			{
				init_champ_pointeur((getAdressePile()+1), 0);
				init_adresse_pointee_bis(recherche(nom_tab)+1);				
				addr_retour=getAdressePile()+1;
				*abs_rel=1;
				*typage_var=1;
				printf("COPA @%d @%d\n", addr_retour, addr_retour);
				incrementerPC();	
			}
			else
			{
				//printf("Valeur de l'adresse de retour %d\n", get_adresse_retour());
				//printf("valeur du nom de l'adresse retour %s\n", getNomVar(get_adresse_retour()));
				recherchet_spec(getNomVar(get_adresse_retour()), abs_rel, typage_var);
				//printf("valeur de abs_rel %d\n", *abs_rel);
				if(*abs_rel==0)
				{
					//printf("valeur de l'adresse retour dans le if %d\n", get_adresse_retour());
					addr_retour= get_adresse_retour()-(getNombredevariableglobale()+1);
				}
				else
				{
					addr_retour= get_adresse_retour();
				}
				empilert(addr_retour,1, *abs_rel, *typage_var );
				init_adresse_pointee_bis(get_adresse_retour());
				
			}
			
	    	}
		else if(get_nivPtr()<get_nivPtr_var(recherche(nom_tab)))
		{
			if(get_presence_lettre()==1)
			{

				init_champ_pointeur((getAdressePile()+1), get_nivPtr());
				init_adresse_pointee_bis(recherche(nom_tab)+1);
				addr_retour=getAdressePile()+1;
				*abs_rel=1;
				*typage_var= get_nivPtr()+1;
				//Peut-être initialiser toutes les cases du tab avec
				//init_champ_pointeur(get_adresse_retour(), get_nivPtr());
			}
			else
			{
				int absm,typ;
				int ad = recherchet_spec( getNomVar( get_adresse_retour()),&absm,&typ) ;
				printf("AFC @%d %d\n",empilert(ad,0, *abs_rel, *typage_var) ,ad);
				init_adresse_pointee_bis(get_adresse_retour());
				//printf("Niveau ptr pile : %d\n", get_nivPtr());
				init_champ_pointeur((getAdressePile()+1), get_nivPtr());
				incrementerPC();
				init_champ_pointeur(get_adresse_retour(), get_nivPtr());
				addr_retour=getAdressePile()+1;
				*abs_rel=1;
				*typage_var=get_nivPtr()+1;
			}	
		}
		reinit_tabAcces();
		reinit_adresse_retour();
		initialiser_niv_tab(recherche(nom_tab));
		reinit_presence_lettre();
		return addr_retour;
}

int debut_accesCase_id(char * nom_var)
{
	int addr;
	if ((addr=recherche(nom_var))==-1)
	{
		return -1;
	}
	else
	{
		if(get_Pointeur(addr)!=1)
		{
			return -2;
		}
		return 0;
	} 
}

int fin_accesCase_id(char * nom_var, int dim, int * abs_rel, int * typage_var)
{
			int addr=recherche(nom_var);
			int retour;
			if (get_champ_tab(get_adresse_pointee(addr))!=1 && get_Pointeur(get_adresse_pointee(addr))!=0)
			{
				return -1;
			}
			else
			{
				init_adresse_retour(get_adresse_pointee(addr));
				incrementer_adresse_retour(dim);
				retour=get_adresse_retour();
				init_adresse_pointee_bis(get_adresse_retour());
				recherchet_spec(getNomVar(get_adresse_retour()), abs_rel, typage_var);
				reinit_adresse_retour();
				return retour;
			}
}

int deb_AccesLettre(char * nom_tab, char * nom_lettre)
{
	//faire recherchet_spec dès le début
	int addr1, addrop2, a, a_typeOp2,a_abs_rel2,a_typage_Op2;
	reinitialiser_nivPointeur();
	init_get_lettre_acces();
	int addr;
	if( (addr=recherche(nom_tab))==-1)
	{
		return -1;
	}       
	else
	{
		//On vérifie que la valeur entre les crochets est inférieure à la dimension de cette 
		//zone du tableau, exemple soit tab[2][3] on ne peut pas écrire tab[5]
		if(get_adresse_tab(addr)!=addr+1)
		{	
			return -2;
		}
		else
		{
					
			initialiser_nom_tableau(nom_tab);
			initialiser_deb_lettre();
			initialiser_presence_lettre();
			int abs_rel, typage_var;
			int addr_lettre=recherchet_spec(nom_lettre, &abs_rel, &typage_var);
			//On copie la valeur contener par l'adresse dans la pile
			if(abs_rel==0)
			{
				printf("COP @%d @@%d\n", (addr1=empilert(addr_lettre,0,abs_rel, typage_var)), addr_lettre);
			}
			else
			{
				printf("COP @%d @%d\n", (addr1=empilert(addr_lettre,0,abs_rel, typage_var)), addr_lettre);
			}			
			init_tabAcces(recherche(nom_tab));
			incrementerPC();
			//On empile la valeur par laquelle on va multiplier la valeur précédente pour
			//savoir à quelle case accéder
			printf("AFC @%d %d\n", (addrop2=empilert(get_val_tabAcces(get_nivPtr()), 0, 1, 1)),get_val_tabAcces(get_nivPtr())) ;
			a = depilert(&a_typeOp2,&a_abs_rel2,&a_typage_Op2);
			printf("MUL @%d @%d @%d\n", addr1, addr1, addrop2);
			incrementerPC();
			incrementerPC();
			//Suite à cette addition on a à addr1 (dans la pile) l'adresse de la case tableau accédée
			//printf("ADD %d %d %d\n", addr1, recherche($1)+1, addr1);
			incrementer_niveau_pointeur();
			return 0;
		}
	}
}

int fin_AccesLettre(char * nom_tab, int * abs_rel, int * typage_var)
{
			int retour, abs_rel_1, typage_var_1, addrOp2;
			char nom_premiere_case[50];
			strcpy(nom_premiere_case, nom_tab);
			strcat(nom_premiere_case, "0");
			int addr=recherchet_spec(nom_premiere_case, &abs_rel_1, &typage_var_1);
			/*if(abs_rel_1==0)
			{
				addr=addr+getNombredevariableglobale()+1;
			}
			*/
			printf("AFC @%d %d\n", (addrOp2=empilert(addr, 0, 1, 1)),addr) ; 
			incrementerPC();		
			/*if(abs_rel_1==0)
			{
				printf("ADD @%d @%d @%d\n", getAdressePile()+2, addrOp2, getAdressePile()+2);  
			}
			else
			{*/
				printf("ADD @%d @%d @%d\n", getAdressePile()+2, addrOp2, getAdressePile()+2);  
			//}
			incrementerPC();
			if(get_nivPtr()==get_nivPtr_var(recherche(nom_tab)))
			{
				init_champ_pointeur((getAdressePile()+1), 0);
			}
			else if (get_nivPtr()<get_nivPtr_var(recherche(nom_tab)))
			{
				init_champ_pointeur((getAdressePile()+1), get_nivPtr());
			}
			init_adresse_pointee_bis(recherche(nom_tab)+1);
			retour=getAdressePile()+2;
			reinit_tabAcces();
			reinit_adresse_retour();
			initialiser_niv_tab(recherche(nom_tab));
			reinit_presence_lettre();
			*abs_rel=1;
			*typage_var=get_nivPtr()+1;
			printf("COPA @%d @%d\n", retour, retour);
			incrementerPC();			
			return retour;
}

int deb_suite_acces(int dim )
{
		//On vérifie que le déréférencement du tableau ne va pas trop loin et que la valeur entre les crochets
		//est toujours correcte
		int a,a_typeOp2,a_abs_rel2,a_typage_Op2, addrop2, addr1;
		if(get_nivPtr()<get_nivPtr_var(recherche(get_nomTableau()))) 
		{	    	 
			if(dim<dimension(get_nomTableau(),get_nivPtr()))
			{
				
				if(get_presence_lettre()==1)
				{
					//print_tab_Acces();	
					init_adresse_retour(0);
					incrementer_adresse_retour(dim*get_val_tabAcces(get_nivPtr()));
					int abs_rel3, typage_var_3;
					int addr_emp=recherchet_spec(getNomVar(get_adresse_retour()), &abs_rel3, &typage_var_3);
					printf("AFC @%d %d\n", (addrop2=empilert(addr_emp, 0,1, 1)), addr_emp);
					a = depilert(&a_typeOp2,&a_abs_rel2,&a_typage_Op2);
					addr1=getAdressePile()+1;
					printf("ADD @%d @%d @%d\n", addr1, addrop2, addr1);
					incrementerPC();
			
				}
				else
				{
					incrementer_adresse_retour(dim*get_val_tabAcces(get_nivPtr()));
				}
				
				incrementer_niveau_pointeur();
				return 0;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -2;
		}

}

void fin_suite_acces(char * nom_lettre)
{
	int addr1, addrop2, addrop3, a,a_typeOp2,a_abs_rel2,a_typage_Op2;
	init_get_lettre_acces();
	//On copie la valeur contener par l'adresse dans la pile
	if(get_avt_nb()==1)
	{
		int absp,typ;
		addrop3=empilert(get_adresse_retour(),  0, 0, 1);
		int ad= recherchet_spec(getNomVar(get_adresse_retour()),&absp,&typ);
		printf("AFC @%d %d\n", addrop3, ad);
		incrementerPC();
		reinit_avt_nb();
	}
	int abs_rel, typage_var;
	int addr_lettre=recherchet_spec(nom_lettre, &abs_rel, &typage_var);
	//On copie la valeur contener par l'adresse dans la pile
	if(abs_rel==0)
	{
		printf("COP @%d @@%d\n", (addr1=empilert(addr_lettre,0,abs_rel, typage_var)), addr_lettre);
	}
	else
	{
		printf("COP @%d @%d\n", (addr1=empilert(addr_lettre,0,abs_rel, typage_var)), addr_lettre);
	}	
	incrementerPC();
	initialiser_presence_lettre();
	//On empile la valeur par laquelle on va multiplier la valeur précédente pour
	//savoir à quelle case accéder
	printf("AFC @%d %d\n", (addrop2=empilert(get_val_tabAcces(get_nivPtr()), 0,1,1)), get_val_tabAcces(get_nivPtr()));
	incrementerPC();
	a = depilert(&a_typeOp2,&a_abs_rel2,&a_typage_Op2);
	printf("MUL @%d @%d @%d\n", addr1, addr1, addrop2);
	incrementerPC();
	addrop2=addr1;
	a = depilert(&a_typeOp2,&a_abs_rel2,&a_typage_Op2);
	addr1=getAdressePile()+1;
	//Suite à cette addition on a à addr1 (dans la pile) l'adresse de la case tableau accédée
	printf("ADD @%d @%d @%d\n", addr1, addr1, addrop2);
	incrementerPC();
	//printf("ADD %d %d %d\n", addr1, recherche(get_nomTableau()))+1, addr1);
	incrementer_niveau_pointeur();
}

int deb_affectation_tab()
{
			reinit_lettre_acces_2();
			if(get_nivPtr()<get_niv_tab())
			{
				return -1;
			}
			else
			{
				reinitialiser_nivPointeur();
				reinitialiser_nivTab();
				if(get_lettre_acces())
				{
					init_get_lettre_acces_2();
					reinit_lettre_acces();
				}
				return 0;
			}
}

int fin_affectation_tab(int addr_case_tab, int abs_rel, int addr_var, int abs_rel2)
{
			//printf("Bonjour\n");
			int addr_c_t=addr_case_tab, addr_v=addr_var;
			if(abs_rel==0)
			{
				addr_c_t=addr_case_tab+getNombredevariableglobale()+1;
				
			}
			if(abs_rel2==0)
			{
				addr_v=addr_var+getNombredevariableglobale()+1;
			}		
			//Si Expression n'est pas un tableau on vérifie qu'il n'y a pas de problèmes de niveaux de pointeurs 
			if(get_nivPtr_var(addr_v)!=get_nivPtr_var(addr_c_t) && get_niv_tab()==0)
			{
				return -1;	
			}
			//Si Expression est un tableau on vérifie qu'il n'y a pas de problèmes de niveaux de pointeurs 
			//Si on renvoi avec Expression une ligne du tableau
			else if(get_nivPtr_var(addr_c_t)!=(get_niv_tab()-get_nivPtr()) && get_niv_tab()!=0)
			{
				return -2;	
			}	
			else
			{	
				//printf("Aurevoir\n");
				if(get_lettre_acces_2()==1)
				{
					if (abs_rel==0 && abs_rel2==0)
					{
						printf("COPB @@%d @@%d\n",addr_case_tab, addr_var); 
					}
					else if (abs_rel==0 && abs_rel2==1)
					{
						printf("COPB @@%d @%d\n", addr_case_tab, addr_var); 
					}
					else if (abs_rel==1 && abs_rel2==0)
					{
						printf("COPB @%d @@%d\n", addr_case_tab, addr_var); 
					}
					else if(abs_rel==1 && abs_rel2==1)
					{
						printf("COPB @%d @%d\n", addr_case_tab, addr_var); 
					}
					incrementerPC();
					modifierChampInitialiserVar(addr_c_t);
					//problème on en pourra pas savoir si la case accédée par lettre est initialisée
				}				
				else
				{
					if (abs_rel==0 && abs_rel2==0)
					{
						printf("COP @@%d @@%d\n", addr_case_tab, addr_var); 
					}
					else if (abs_rel==0 && abs_rel2==1)
					{
						printf("COP @@%d @%d\n", addr_case_tab, addr_var); 
					}
					else if (abs_rel==1 && abs_rel2==0)
					{
						printf("COP @%d @@%d\n", addr_case_tab, addr_var); 
					}
					else if(abs_rel==1 && abs_rel2==1)
					{
						printf("COP @%d @%d\n", addr_case_tab, addr_var); 
					}
					incrementerPC();
					modifierChampInitialiserVar(addr_c_t);
				}
				viderPile();
				reinitialiser_nivPointeur();
				reinitialiser_nivTab();	
				reinit_champ_init_deref(addr_v);
				reinit_champ_pointeur(addr_v);
				return 0;	
			}
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int dereferencement_expr(char * nom, int * abs_rel, int * typage_var)
{
	int addr,i, addr1;
	int typeOp1;
	
	//On vérifie si le pointeur que l'on a déréférencé existe
	if ((addr=recherchet_spec(nom, abs_rel,typage_var))==-1)
	{
		return -1 ;
	}
	else 
	{
		int addr_glo=addr+getNombredevariableglobale()+1;
		if(get_initialiser_var(addr_glo)==0)
		{
			return -2;
		}
		else
		{
			addr1=empilert(addr,1, *abs_rel, (*typage_var-get_nivPtr()));			
			//On dit que l'on a empilé un pointeur dans la pile
			reinit_champ_pointeur(addr1);	
			//on stocke l'adresse du pointeur dans la pile
			printf("AFC @%d %d\n",addr1, addr);
			incrementerPC();
			//On déréférence l'adresse stockée pour arriver au niveau souhaité			
			for(i=0; i<get_nivPtr()+1; i++)
			{
				//COPA va permettre de récupérer la valeur pointée par l'adresse stockée en addr1, genre si
				//dans addr1 qui vaut 999 j'avais stocké le pointeur se trouvant à l'adresse 3
				//COPA va nous donner l'adresse sur laquelle pointait ce pointeur
				int a, a_t, a_t2, a_t3;
				a=depilert(&a_t, &a_t2, &a_t3);
				addr1=empilert(addr1,1, 1, (*typage_var-get_nivPtr()));				
				printf("COPA @%d @%d\n", addr1, addr1);
				incrementerPC();
			} 
			//On dit que l'endroit de la pile où est stocké le pointeur est initialisé
			modifier_champ_init_deref(addr1);
			//On sauvegarde e niveau du pointeur après déréférencement
			init_niveau_pointeur_2(get_nivPtr_var(addr_glo));
			//Si on a un déréférencement jusqu'à accès à la valeur pointée				
			//if(get_nivPtr_var(addr)-get_nivPtr()==0)
			//{
				//le champ niveau pointeur dans la pile sera égal au niveau du pointeur
				init_champ_pointeur(addr1, get_nivPtr_var(addr_glo)-get_nivPtr());

			//}
			//Si on a un déréférencement jusqu'à accès à une adresse pointée			
			//else if(get_nivPtr_var(addr)-get_nivPtr()>0)
			//{
				//le champ niveau pointeur dans la pile sera égal au niveau du pointeur après déréférencement 
				//	init_champ_pointeur(addr1, get_nivPtr_var(addr)-get_nivPtr());
				//}	
				//On renvoi l'adresse de la pile où on a stocké l'adresse du pointeur
				init_adresse_pointee(addr1, get_adresse_pointee_bis());
				return addr1;			
						
		}
	}
}

int expression_ptr_expr(int addr)
{
			int addr1=getAdressePile()+1;
			int addr_glo=addr+getNombredevariableglobale()+1;
			int retour;
			//On affaire à une initialisation avec un &
			//il faut voir l'adresse comme une constante cette fois-ci
				
			//On copie l'adresse sauvegardé dans la pile dans la variable pointeur
			//printf("COP %d %d\n", recherche(get_nom_pointeur()),getAdressePile()+1);
			init_adresse_pointee(addr1,get_adresse_pointee_bis());
			init_niveau_pointeur_2(get_nivPtr_var(addr_glo)+1);
			init_champ_pointeur(addr1, (get_nivPtr_var(addr_glo)+1));
			modifier_champ_init_deref(addr1);
			reinit_champ_pointeur(addr_glo);
			reinitialiser_nivTab();
			retour=addr1;
			//printf("AFC @%d %d \n", addr1, addr_glo); 
			return retour;
			//modifierChampInitialiserVariable(get_nom_pointeur());
}

int verification(int addr, int abs_rel)
{
		int addr_bis;
		int retour_OP;
		int retour_Acces;
		if(abs_rel==0)
		{
			addr_bis=addr+getNombredevariableglobale()+1;
		}	
		else
		{
			addr_bis=addr;
		}	

		retour_Acces=verifier_Acces_case_OP(addr_bis);
		if(retour_Acces==-1)
		{
			return -1;
		}
	  	retour_OP=verifier_Dereferencement_OP(addr_bis);
		if(retour_OP==-1)
		{
			return -2;
		}
		/*if(get_nivPtr()!=get_nivPtr_var($1))
		{
			reinit_champ_init_deref($1);
		}*/
		reinit_champ_init_deref(addr_bis);
		reinit_champ_pointeur(addr_bis);	
		reinitialiser_nivPointeur();
		reinitialiser_nivTab();	
		reinit_lettre_acces();
		return 0;
}


int verif_niv_ptr(int addr_1, int addr_2, int addr1_rel , int addr2_rel)
{
	int addr_1_bis=addr_1;
	int addr_2_bis=addr_2;

	if(addr1_rel==0)
	{
		addr_1_bis=addr_1+getNombredevariableglobale()+1;
	}
	if(addr2_rel==0)
	{
		addr_2_bis=addr_2+getNombredevariableglobale()+1;
	}
	
	if(get_nivPtr_var(addr_1_bis)!=get_nivPtr_var(addr_2_bis))
	{
		return -1;
	}		
	return 0;
}

int affectation_gauche_id_tab_ptrNonDeref(char * dollar_1, int addr_dollar_3, int addr_dollar_3_rel)
{
		//printf("il a pas dit Bonjour__________________________\n");
		int var, addr1, addr3, abs_rel, typage_var;
		int  addr_dollar_1=recherchet_spec(dollar_1, &abs_rel, &typage_var);
		if(addr_dollar_1==-1)
		{
			return -1;
		}
		else
		{
			if(abs_rel==0)
			{
				addr1=addr_dollar_1+getNombredevariableglobale()+1;
			}
			else
			{
				addr1=addr_dollar_1;
			}
		
			if(addr_dollar_3_rel==0)
			{
				addr3=addr_dollar_3+getNombredevariableglobale()+1;
			}
			else
			{
				addr3=addr_dollar_3;
			}
			 /*addr=recherche($1) ;
				if ((  var =recherche($1))==-1)
				{
					yyerror("ERROR \n") ;
				}
				else 
				{*/
					//Si Expression n'est pas un tableau on vérifie qu'il n'y a pas de problèmes de niveaux de pointeurs 
					//printf("Niveau Pointeur : %d\n", get_nivPtr_var($3));
					//printf("Niveau Pointeur gauche : %d\n", get_nivPtr_var(recherche($1)));
					//printf("Niveau Pointeur tab : %d\n", get_niv_tab());
					//printf("VAleur de Pointeur 1 : %d \n", get_nivPtr_var(recherche($1)));
					//printf("VAleur de Pointeur 2 : %d \n", get_niv_tab()-get_nivPtr());
					if(get_nivPtr_var(addr3)!=get_nivPtr_var(addr1) && get_niv_tab()==0)
					{				
						return -2;						
					}
					//Si Expression est un tableau on vérifie qu'il n'y a pas de problèmes de niveaux de pointeurs 
					//Si on renvoi avec Expression une ligne du tableau
				
					else if(get_nivPtr_var(addr1)!=(get_niv_tab()-get_nivPtr()) && get_niv_tab()!=0)
					{
						return -3;						

					}
					//Si on a le nom d'un tableau à gauche et à droite le nom d'un pointeur
					else if(get_champ_tab(addr1)==1)
					{
						//On vérifie qu'on a bien un pointeur à droite
						if(get_nivPtr_var(addr3)>0)
						{
							if(get_nivPtr_var(addr3)!=get_nivPtr_var(addr1))
							{
								return -4;						

							}
							else
							{
								//Le tableau pointera plus sur l'adresse du début du tableau 
								//mais sur l'adresse pointée par le pointeur à droite du =
								//printf("######################################\n");
								modi_adresse_deb_tab(addr1, get_adresse_pointee(addr3));
								if (addr_dollar_3_rel==0 && abs_rel==0)
								{
									/*if (strcmp(getMode(),"fonction")==0)
									{
										dollar_addr +=getTailleTypeRetourFonction() ;
									}*/
										//printf("Bonjour((((((((((((((((((((");
									printf( "COP @@%d @@%d\n",addr_dollar_1, addr_dollar_3);
								}else if  (addr_dollar_3_rel==0 && abs_rel==1)
								{	
									/*if (strcmp(getMode(),"fonction")==0)
									{
										dollar_addr +=getTailleTypeRetourFonction() ;
									}*/
									printf( "COP @%d @@%d\n",addr_dollar_1, addr_dollar_3);
								}else if  (addr_dollar_3_rel==1 && abs_rel==0)
								{
									printf( "COP @@%d @%d\n",addr_dollar_1, addr_dollar_3);
								}else if  (addr_dollar_3_rel==1 && abs_rel==1)
								{
									printf( "COP @%d @%d\n",addr_dollar_1, addr_dollar_3);
								}													
								incrementerPC();
								viderPile();
								if(addr_dollar_3!=NULL_2)
								{
									modifierChampInitialiserVariable(dollar_1);	
								}	
								reinitialiser_nivPointeur();
								reinitialiser_nivTab();	
								//reinit_champ_init_deref($3);
								//reinit_champ_pointeur($3);	

							}
						}
						else
						{
							return -5;						

						}	
					}
					else
					{	
						//Si on a un pointeur à droite et un tableau ou un pointeur à gauche		
						if(get_champ_tab(addr1)==0 && get_nivPtr_var(addr1)>0)
						{
							if(get_champ_tab(addr3)==0)
							{
								init_adresse_pointee(addr1, get_adresse_pointee(addr3));
							}
							//On a un tableau à droite du =	
							else
							{
								init_adresse_pointee(addr1, get_adresse_pointee(addr3));
							}
						}
						/*if(get_lettre_acces()==1 && (get_niv_tab()-get_nivPtr())==0) 
						{
							//On récupère dans la pile la valeur contenue par la case
							printf("COPA @%d @%d\n", addr_dollar_3, addr_dollar_3);
							incrementerPC();
						}*/
						if (addr_dollar_3_rel==0 && abs_rel==0)
						{
							/*if (strcmp(getMode(),"fonction")==0)
							{
								dollar_addr +=getTailleTypeRetourFonction() ;
							}*/

							//printf("######################################\n");
							printf( "COP @@%d @@%d\n",addr_dollar_1, addr_dollar_3);
						}else if  (addr_dollar_3_rel==0 && abs_rel==1)
						{	
							/*if (strcmp(getMode(),"fonction")==0)
							{
								dollar_addr +=getTailleTypeRetourFonction() ;
							}*/
							printf( "COP @%d @@%d\n",addr_dollar_1, addr_dollar_3);
						}else if  (addr_dollar_3_rel==1 && abs_rel==0)
						{
							printf( "COP @@%d @%d\n",addr_dollar_1, addr_dollar_3);
						}else if  (addr_dollar_3_rel==1 && abs_rel==1)
						{
							printf( "COP @%d @%d\n",addr_dollar_1, addr_dollar_3);
						}
						incrementerPC();
						viderPile();
						if(addr_dollar_3!=NULL_2)
						{
							modifierChampInitialiserVariable(dollar_1);	
						}	
						reinitialiser_nivPointeur();
						reinitialiser_nivTab();	
						//reinit_champ_init_deref($3);
						//reinit_champ_pointeur($3);
						return 0;	

					}
							
				
		}
}

int affectation_Deref (char * dollar_1, int addr_dollar_3, int addr_dollar_3_rel)
{	

		int var, i, addr1,addr1_pile, addr3, abs_rel, typage_var, addr_dollar_1=recherchet_spec(dollar_1, &abs_rel, &typage_var);//printf("niveau du pointeur : %d\n", get_nivPtr_var(0));printf("valeur de addr1 : %d\n", addr1);
		//printf("valeur de addr_1 :%d\n", addr_dollar_1); 
		if(addr_dollar_1==-1)
		{
			return -1;
		}
		else
		{
			if(abs_rel==0)
			{
				addr1=addr_dollar_1+getNombredevariableglobale()+1;
			}
			else
			{
				addr1=addr_dollar_1;
			}
		
			if(addr_dollar_3_rel==0)
			{
				addr3=addr_dollar_3+getNombredevariableglobale()+1;
			}
			else
			{
				addr3=addr_dollar_3;
			}
			//printf("valeur de addr1 : %d\n", addr1);
			//printf("niveau du pointeur : %d\n", get_nivPtr_var(addr1));
			if((get_nivPtr_var(addr1)-get_nivPtr_3())!= get_nivPtr_var(addr3))
				{
					return -2;
				}
				else
				{
					//printf("Niv ptr var : %d \n", get_nivPtr_var(addr1));
					//printf("Niv ptr : %d \n", get_nivPtr());
					if (get_nivPtr_var(addr1)>1)
					{
						//printf("Bonjourrrr\n");
						//On stocke dans la pile l'adresse du pointeur que l'on déréférence
						addr1_pile=empilert(addr_dollar_1, 0,abs_rel, typage_var-get_nivPtr_3());
						printf("AFC @%d %d\n", addr1_pile, addr_dollar_1);
						incrementerPC();
						for(i=0; i<=get_nivPtr_3()-1; i++)
						{						
							printf("COPA @%d @%d\n", addr1_pile, addr1_pile);
							incrementerPC();
						} 
						if(addr_dollar_3_rel==0)
						{
							printf("COPB @%d @@%d\n", addr1_pile, addr_dollar_3);
						}else
						{
							printf("COPB @%d @%d\n", addr1_pile, addr_dollar_3);
						}
						incrementerPC();					
					}
					else
					{
						if (addr_dollar_3_rel==0 && abs_rel==0)
						{
							/*if (strcmp(getMode(),"fonction")==0)
							{
								dollar_addr +=getTailleTypeRetourFonction() ;
							}*/
							printf( "COPB @@%d @@%d\n",addr_dollar_1, addr_dollar_3);
						}else if  (addr_dollar_3_rel==0 && abs_rel==1)
						{	
							/*if (strcmp(getMode(),"fonction")==0)
							{
								dollar_addr +=getTailleTypeRetourFonction() ;
							}*/
							printf( "COPB @%d @@%d\n",addr_dollar_1, addr_dollar_3);
						}else if  (addr_dollar_3_rel==1 && abs_rel==0)
						{
							printf( "COPB @@%d @%d\n",addr_dollar_1, addr_dollar_3);
						}else if  (addr_dollar_3_rel==1 && abs_rel==1)
						{
							printf( "COPB @%d @%d\n",addr_dollar_1, addr_dollar_3);
						}		
						incrementerPC();
						//on peut pas dire si la var pointée par le pointeur sera initialisée 
					}
					viderPile();
					reinitialiser_nivTab();	
					reinitialiser_nivPointeur();
					return 0;
				}			
		}
}
