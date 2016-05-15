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
	//printf("Valer de debutMain :%d\n", debutMain);
	//afficher_ram();
	
	//chargerDebugInformation(); 
	//printTabDugInfo();
	
	interpreter(0);//interpretation en mode pas debug
	

	return 0;
}
