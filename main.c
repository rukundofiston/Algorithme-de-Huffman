/*Fichier: main.c */
#include <stdio.h>
#include <stdlib.h>
void main(){
    /*int aux =  65000;
    int r = aux%255;
    aux = aux - r;
    int nbre1 = aux/255;
    int nbre2 = 255;
    int nbre3 = r;

    printf("%d * %d + %d\n = %d", nbre1, nbre2, nbre3, (nbre1*nbre2+nbre3));*/
	/*FILE* binaires = fopen("textee.txt","w");
    if(binaires == NULL){// test si le fichier existe
        perror("Erreur d'ouverture du fichier: ");
        exit(1);
    }
    int taille_du_binaire = 3941;
	fprintf(binaires, "%d", taille_du_binaire);
	//fwrite(&taille_du_binaire, 3, 1, binaires);
	fclose(binaires);*/
    //int d = 127;
    //int marge = 8 - (d%8);
    //printf("%d et %d\n",(d%8),  marge);
    //char t[9] = {'0','1','1','0','0','1','1','1'};
    //int octet = atoi(t);
    /*int octet = 111011;
    char str[8];
    itoa(octet, str, 10);
    printf("INT %d et String %s %d\n", octet, str, sizeof(str));*/
    menu();
}
