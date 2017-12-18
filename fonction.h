/*Fichier: entete.h*/
#ifndef H_FONCTION
#define H_FONCTION

#include "variables.h"
int nombre_digit(int nombre);
void affichage_code(int nbr_bits, int code);
void occurence(char *nom_fichier, int tab[]);
struct noeud* creer_feuille(int* tab, unsigned int index);
void creer_arbre_huffman(struct noeud* arbre_huffman[LONGUEUR_ASCII], int *tab);
void creer_noeud(struct noeud *arbre_huffman[LONGUEUR_ASCII], int taille);
void creer_code(struct noeud* element, int code, int niveau);
void fprint_code(FILE *fichier, int nbr_bits, int code);
void compression(char *nom_fichier);
void decompression(char *nom_fichier);
void fprintf_alphabet(FILE *fichier);
void lire_contenu(char *nom_fichier);
void afficherHuffman(struct noeud *sommet,int niveau);
void creer_entete(FILE *fichier);
#endif
