#cd ..
#mv fichier_table_des_labels interpreteur
#mv fichier_table_des_symbols interpreteur
#cd interpreteur
gcc main.c interpreteur.c ../compilateur/tab_label.c ../compilateur/tab_symb.c -o interpreteur

