#include <stdio.h>
#include <stdlib.h>
#include "interpreteur.h"

int main(int argc, char const *argv[])
{

	if (argc!=2)
	{
		printf("Usage: ./interpreteur fichier\n");
		exit(1);
	}

	charger_Programme_dans_Ram(argv[1]);
	//afficher_ram();
	
	chargerDebugInformation(); 
	printTabDugInfo();
	//printf("=============================\n");
	interpreter(1);
	

	/*char inputUser[40] = "";

	fgets(inputUser,sizeof(inputUser), stdin);
	print("%s",inputUser);*/
	return 0;
}
