#!//bin/bash
clear
./executable < fichier_test2 > fichier_asm
gcc main.c tab_label.c tab_symb.c deuxieme_passe.c gestion_des_fonctions.c 
#echo "=========================fichier binaire"
#cat fichier_asm_binaire
echo "================================fichier assembleur============="
cat fichier_asm
#echo "====================fichier_test============================"
#cat fichier_test
#echo "==============fichier TABLE DES LABEL==================================================="
#cat fichier_table_des_labels
#echo "====================fichier asm sans label "
#cat fichier_ass_correct
