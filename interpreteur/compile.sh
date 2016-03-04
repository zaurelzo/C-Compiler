#cd ..
#mv fichier_table_des_labels interpreteur
#mv fichier_table_des_symbols interpreteur
#cd interpreteur
gcc main.c interpreteur.c ../tab_label.c ../tab_symb.c

