#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../head/interpreteur.h"

int main(int argc, char const *argv[])
{

	if (argc==1)
	{
		printf("Usage: ./interpreteur fichier\n");
		printf("ou \n Usage : ./interpreteur -d fichier \n"); //debug 
		exit(1);
	}else
	{
		
		//afficher_ram();
		chargerDebugInformation(); 

	 	if (argc==2)
	 	{
	 		charger_Programme_dans_Ram(argv[1]);
	 		interpreter(0);
	 	}else if (argc==3 && strcmp(argv[1],"-d")==0 )
	 	{
	 		charger_Programme_dans_Ram(argv[2]);
	 		interpreter(1);
	 	} else 
	 	{
	 		printf("trop d'arguments ou option de debug fausse \n");
	 		exit (1);
	 	}

	 }
	

	return 0;
}
