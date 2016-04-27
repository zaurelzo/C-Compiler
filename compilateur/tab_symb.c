#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tab_symb.h"


int ind = 0; 
int nombreDeVariabeleglobale = -1; 
int indPile = ADRESSE_DEBUTPILE;



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
	ind++;

	return 0 ; 
}

 int recherche(char * nom_var) {return 0 ;} // pour debug à effacer 


/*relative_ou_absolue : 0 relative , 1 :absolue*/
int recherchet(char * nom_var , int * relative_ou_absolue )
{
	int i =0 ;
	//int retour =-1 ; 
	int retour =-1 ;

	for (i=0 ; i<=ind-1;i++)
	{
		if ( (strcmp(Array[i].nom_var,nom_var)==0 ))
		{
			if (i <=nombreDeVariabeleglobale)
				*relative_ou_absolue= 1 ;
			else 
				*relative_ou_absolue = 0; 
			retour=  i ;
		} 
	}

	if(strcmp(getMode(),"Pasfonction")==0)//pas en mode fonction, c'est une global
	{
		return retour ; 
	}else
	{
		//if(strcmp(nom_var,"a")==0) printf("ret : %d / nbglo %d \n",retour,nombreDeVariabeleglobale);
		return retour - nombreDeVariabeleglobale -1 + getTailleTypeRetourFonction();
	}
	
}
 
void printTabVar()
{
	
	int i =0 ;
	//printf("indice %d  ",ind);
	for (i=0 ; i<=ind;i++)
	{
		printf("%s \n", Array[i].nom_var );
	}
}

int  empilert(int value ,int type ,int abs_ou_rel )
{
	char buf[32];
	sprintf(buf,"%d",value);
	strcpy(Array[indPile].nom_var,buf);
	Array[indPile].type=type;
	Array[indPile].abs_rel=abs_ou_rel ;
	indPile--;  
	return indPile+1 ; 
} 

int empiler(int value , int type  ) {return 0 ;} // pour debug à effacer 


int depiler(int *  type_val_depile ) {return 0 ;} // pour debug à effacer 

int depilert(int *  type_val_depile , int *  abs_ou_rel )
{
	 //* adresse = indPile; 
	if(indPile<=ADRESSE_DEBUTPILE) 
	{
			indPile++;
			*type_val_depile= Array[indPile].type; 
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