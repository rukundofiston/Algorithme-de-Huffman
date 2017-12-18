/*Fichier: entete.h*/
#ifndef H_ENTETE
#define H_ENTETE
#define LONGUEUR_ASCII 256
//#define LONGUEUR_MAX 100000

struct noeud{
        //Caractère initial
        unsigned char c;
        //Nombre des occurences dans le fichier
        unsigned int occurence;
        //Codage dans l'arbre
        int code;
        //Nombre de bits sur lesquels est codée le caractère
        int bits;
        //Lien vers les noeuds suivants
        struct noeud *gauche, *droite;
} noeud;

int nombre_feuille, taille_du_text, taille_du_binaire;
struct noeud* arbre_huffman[LONGUEUR_ASCII];
struct noeud *alphabet[256];
int tableau_occurence[LONGUEUR_ASCII] = {0};
char nom_fichier[30];
FILE *fichier = NULL;

#endif
