/*Fichier: outil.h */
#ifndef H_OUTIL
#define H_OUTIL

#define LONGUEUR_ASCII 256
#define LONGUEUR_MAX 500

typedef struct noeud{
    //Caractère initial
    unsigned char c;
    //Nombre des occurences dans le fichier
    int occurence;
    //Codage dans l'arbre
    int code;
    //Nombre de bits sur lesquels est codée le caractère
    int bits;
    //Lien vers les noeuds suivants
    struct noeud *gauche, *droite;
} noeud;

int nombre_feuille, taille_du_text, taille_du_binaire;
noeud* arbre_huffman[LONGUEUR_ASCII];
noeud *alphabet[256];
int tableau_occurence[LONGUEUR_ASCII] = {0};
char nom_fichier[30];
//FILE *fichier;

//Entete des fonctions d'aide à la compression
//int nombre_digit(int nombre);
//void affichage_code(int nbr_bits, int code);
//void menu();
#endif
