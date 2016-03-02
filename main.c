#include <stdio.h>
#include <stdlib.h>
#include "tab_symb.h"
#include "tab_label.h"
int main()
{

	ajouter_label();
	ajouter_label();
	ajouter_label();

	afficherTab();
	modifierNum_instruction("label2", 12);
	printf("======================\n");
	afficherTab();

return 0 ; 
}
 
