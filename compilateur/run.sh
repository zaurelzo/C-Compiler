#!//bin/bash
clear
./executable < fichier_test > fichier_asm
gcc main.c tab_label.c tab_symb.c deuxieme_passe.c gestion_des_fonctions.c
./a.out 
#echo "=========================fichier binaire"
#cat fichier_asm_binaire
echo "================================fichier assembleur====================================="
cat fichier_asm
echo "====================fichier_test======================================================="
cat fichier_test
#echo "====================fichier asm sans label "
#cat fichier_ass_correct
