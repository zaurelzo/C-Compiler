#include <stdio.h>
#include <stdlib.h>
#include "interpreteur.h"

int main(int argc, char const *argv[])
{

	//TODO : permettre le passage en parametre du fichier a interprété 

	charger_Programme_dans_Ram("../compilateur/fichier_asm_binaire");
	//afficher_ram();
	interpreter();
	return 0;
}
