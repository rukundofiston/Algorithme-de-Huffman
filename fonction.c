/*Fichier: fonction.h */
#include <stdio.h>
#include <stdlib.h>

#include "variables.h"
#include "fonction.h"

//Definition des methodes qui sont dans outil.h
int nombre_digit(int nombre){
    if(nombre == 0){
        return 1;
    }

    int compteur = 0;
    while(nombre != 0) {
        nombre = nombre/10;
        compteur++;
    }
    return compteur;
}

void affichage_code(int nbr_bits, int code){
    int nbre_digit, nbre_zero, i;
    //nbre_digit = Compte le nombre de bits sur lequel est codé le caractère afin de connaitre les 0
    //à afficher devant pour faire un code binaire (sans la perte des 0 de devant
    //par exemple on peut avoir un code 0, sur 4 bits, dan ce cas il faudre afficher "0000"
    nbre_digit = nombre_digit(code);
    nbre_zero = nbr_bits - nbre_digit;

    //printf("Digit: %d \t Codage: %d \t Zero: %d Nbre Bits: %d \n", nbre_digit, codage, nbre_zero, nbr_bits);
    for(i=0; i<nbre_zero; i++){
        printf("0");
    }
    printf("%d\n", code);
}

void menu(){
    int choix = 0;
    do{
        printf("\n  ********************************************************\n");
        printf("\n  * (1): Donner le nom du fichier                        *\n");
        printf("\n  * (2): Tableau des occurences                          *\n");
        printf("\n  * (3): Compression + Taux de compression               *\n");
        printf("\n  * (6): Afficher l'arbre de Huffman                     *\n");
        printf("\n  * (4): Afficher le contenu du fichier                  *\n");
        printf("\n  * (5): Décompression                                   *\n");
        printf("\n  * (0): Quitter                                         *\n");
        printf("\n  ********************************************************\n");

        printf("\n  Quelle est votre choix: ?:");
        scanf("%d", &choix);
        //Fonction clear screen
        system("cls");

        switch(choix){
            case 1:
                //Donner le nom du fichie
                printf("\n  Donner le nom du fichier: ");
                scanf("%s",nom_fichier);
                printf("\n\tLe nom saisi est %s\n\n\n", nom_fichier);
                break;

            case 2:
                //Tableau des occurences
                occurence(nom_fichier, tableau_occurence);
                int i;
                for(i=0; i<LONGUEUR_ASCII; i++){
                    if(tableau_occurence[i]>0){
                        printf("%c: %d\n", i, tableau_occurence[i]);
                    }
                }
                break;

            case 3:
                //Compression + Taux de compression
                //Etape 0: Tableau des occurence
                occurence(nom_fichier, tableau_occurence);
                //Etape 1 de la compression: Créer l'arbre de huffman
                creer_arbre_huffman(arbre_huffman, tableau_occurence);
                //Etape 2 de la compression: Créer les noeuds de l'arbre de huffman
                creer_noeud(arbre_huffman, LONGUEUR_ASCII);

                //Etape 3 de la compression
                i = 0;
                while(i< LONGUEUR_ASCII){
                    if(arbre_huffman[i]!= NULL){
                        creer_code(arbre_huffman[i],0, 0);
                    }
                    i++;
                }

                printf("\n\n********* DEBUT DE COMPRESSION *****************\n");
                compression(nom_fichier);
                //free(arbre_huffman);
                //printf("Taille_du_binaire %d et taille_du_text %d ", taille_du_binaire, taille_du_text);
                double taux_compression = taille_du_binaire/(taille_du_text*8.0);
                printf("Taux de compression est: %f\n\n", taux_compression);
                printf("\n************FIN DE COMPRESSION ******************\n\n");
                break;

            case 6:
                i=0;
                while (arbre_huffman[i] == NULL && i < LONGUEUR_ASCII) i++;
                //printf("%c:%d",arbre_huffman[i]->c, arbre_huffman[i]->occurence);
                //afficherHuffman(arbre_huffman[i],0);
                afficherHuffman(arbre_huffman[i],0);
                break;
            case 4:
                //Afficher le Contenu apres la compression
                lire_contenu(nom_fichier);
                break;

            case 5:
                //Décompression
                printf("\n\n********* DEBUT DE DECOMPRESSION *****************\n");
                decompression(nom_fichier);
                printf("\n************FIN DE DECOMPRESSION ******************\n");
                break;

            case 0:
                //Fonction clear screen
                system("cls");
                printf("\t  ************************************************\n");
                printf("\t  *                                              *\n");
                printf("\t  *                                              *\n");
                printf("\t  *                                              *\n");
                printf("\t  *                                              *\n");
                printf("\t  *    MERCI D'AVOIR UTILISE NOTRE PROGRAMME     *\n");
                printf("\t  *                                              *\n");
                printf("\t  *                                              *\n");
                printf("\t  *                                              *\n");
                printf("\t  *                                              *\n");
                printf("\t  ************************************************\n\n\n");
                break;

            default:
                printf("\t Vous avez fait un mauvais choix! (Choisir entre 0 et 6)");
                break;
            }
    }while(choix !=0);
}

/*definition des fonctions*/
void occurence(char nom_fichier[30], int tab[LONGUEUR_ASCII]){
    unsigned int caract = 0, i;
    //"r" : Ouverture en lecture seule
	FILE* fichier = fopen(nom_fichier,"r");

	if (fichier == NULL){		    // test si le fichier existe
        perror("Probleme ouverture fichier: ");
        exit(1);
	}
    //Remettre à zero les valeurs du tableau
	for(i=0; i<LONGUEUR_ASCII; i++){
        tab[i] = 0;
	}

	while((caract=fgetc(fichier)) != EOF){
		tab[caract]++;
    }
    //Car les caractère spéciaux sont compté deux fois
    for (i = 128; i<LONGUEUR_ASCII; i++){
        tab[caract]=tab[caract]/2;
    }
}

struct noeud* creer_feuille(int* tab, unsigned int index ){
    /*Allocation de memoire, tres important */
    struct noeud *n = (struct noeud *) malloc(sizeof(noeud));
    n->c = index;
    n->occurence = tab[index];
    n->bits = 0;
    n->code = 0;
    n->droite = NULL;
    n->gauche = NULL;

    return n;
}

void creer_arbre_huffman(struct noeud* arbre_huffman[LONGUEUR_ASCII], int *tab){
    unsigned int i;
    //On remet à zéro la valeur nombre_feuille
    nombre_feuille = 0;
    for (i=0; i<LONGUEUR_ASCII; i++){
        //Allocation memoire pour l'élément i du tableau ASCII
        arbre_huffman[i] = (struct noeud *) malloc(sizeof(noeud));
        if(tab[i] > 0) {
            arbre_huffman[i] = creer_feuille(tab, i);
            nombre_feuille++;
        }
        else{
            arbre_huffman[i] = NULL;
        }
    }
}

void creer_noeud(struct noeud *arbre_huffman[LONGUEUR_ASCII], int taille){
    int i, j, k, min, minIndex, secondMin, secondMinIndex;

    k = 1;
    do{
        minIndex = -1;
        //On saute/ignore les noeuds NULL, d'où la boucle while
        i = 0;
        while (arbre_huffman[i] == NULL && i < taille) i++;

        minIndex = i; //On suppose que le premier element est celui ayant le plus petit occurence
        min = arbre_huffman[i]->occurence;

        for (j=i; j<taille; j++){
            if(arbre_huffman[j] != NULL && arbre_huffman[j]->occurence < min ) {
                min = arbre_huffman[j]->occurence;
                minIndex = j;
            }
        }

        secondMinIndex = -1;
        i = 0;
        //Meme principe que precedement
        while ((i<taille && arbre_huffman[i]==NULL) || (i== minIndex && arbre_huffman[i]!=NULL)){
            i++;
        }

        secondMin = arbre_huffman[i]->occurence;
        secondMinIndex = i;

        for (j=i; j<taille; j++){
            if (arbre_huffman[j] != NULL && arbre_huffman[j]->occurence < secondMin && j!= minIndex){
                secondMin = arbre_huffman[j]->occurence;
                secondMinIndex = j;
            }
        }

        //Double controle pour s'assuré qu'on a bien les deux minimum et surtout que leurs indexes sont differentes
        if (minIndex != -1 && secondMinIndex != -1 && minIndex != secondMinIndex){
            struct noeud *aux = (struct noeud *) malloc(sizeof(noeud));
            //Le nouveau noeud, on additionne les occurences des deux minimum
            aux->occurence = arbre_huffman[minIndex]->occurence + arbre_huffman[secondMinIndex]->occurence;
            aux->c = '!';
            aux->droite = arbre_huffman[minIndex];
            aux->gauche  = arbre_huffman[secondMinIndex];

            arbre_huffman[minIndex] = aux; //C'est  pour cela que hari noeud imwe yabaa replacé muri ya schema
            arbre_huffman[secondMinIndex] = NULL;
        }
        k++;
    }while(k < nombre_feuille);
}

void creer_code(struct noeud* element, int code, int niveau){
    /* On est sur une feuille */
    if(element->droite==NULL && element->gauche==NULL){
        element->bits = niveau;
        element->code = code;

        /*Remplir dans l'alphabet: cfr pdf pour comprendre ce que contient struct noeud* alphabet[256]*/
        alphabet[element->c] = malloc(sizeof(noeud));
        alphabet[element->c] = element;
    }
    else{
        /*A Gauche, on ajouter un 0 à droite du code */
        creer_code(element->gauche, (code*10)+0, niveau+1);
        /* On va a droite (on injecte un 1 à droite)*/
        creer_code(element->droite, (code*10)+1, niveau+1);
    }
}


void fprint_code(FILE *file, int nbr_bits, int code){
    int nbre_digit, nbre_zero, i;
    /*
    Passer un entier puis e convertir en binaire sur n bits puis l'ecrire dans le fichier
    Pas besoin d'attendre que ça soit 8 bits
    */
    /*Compte le nombre de caractère de codage afin de connaitre
    les 0 à afficher devant pour faire un code binaire*/
    nbre_digit = nombre_digit(code);
    nbre_zero = nbr_bits - nbre_digit;

    for(i=0; i<nbre_zero; i++){
        fprintf(file,"0");
    }
    fprintf(file,"%d",code);
}


void compression(char *nom_fichier){
    unsigned int caract, i=0;
    char *contenu = 0;
    FILE *fichier = fopen(nom_fichier, "r"); // car on doit parcourir le fichier
    if (fichier == NULL){		    // test si le fichier existe
        perror("Erreur d'ouverture fichier des entrées: ");
        exit(1);
	}

    fseek (fichier, 0, SEEK_END);
    taille_du_text = ftell(fichier);
    printf("Taille du texte: %d\n", taille_du_text);
    fseek (fichier,0,SEEK_SET);
    //allocation memoire pour le contenu
    contenu = (char*)malloc ((taille_du_text+1)*sizeof(char));

    if(contenu){//Si l'allocation memoire est réussie
        fread(contenu, sizeof(char), taille_du_text, fichier);
    }
    fclose(fichier);

    //fichier = fopen(nom_fichier,"w");
    FILE *binaires = fopen("binaires.txt","w");
    if (binaires == NULL){// test si le fichier existe
        perror("Erreur d'ouverture du fichier: ");
        exit(1);
	}

    printf("Dans lecture binaire\n");
	i=0;
	taille_du_binaire = 0;
    while(i<taille_du_text && contenu[i] !=NULL){
        caract = contenu[i];
        printf("caract a traite %c à l'indice %d \n", caract, i);
        /*Pour chaque caractère, on écrit sont code dans le fichier et
        on l'affiche à l'écran avec la fonction affichage_code*/
        fprint_code(binaires, alphabet[caract]->bits, alphabet[caract]->code);
        //Compteur de la taille des bits ecrit dans le fichier
        taille_du_binaire = taille_du_binaire + alphabet[caract]->bits;
        //printf("Compression %c: ", caract);
        //affichage_code(alphabet[caract]->bits, alphabet[caract]->code);
        i++;
    }
    fclose(binaires);

    binaires = fopen("binaires.txt","r");
    if(binaires == NULL){// test si le fichier existe
        perror("Erreur d'ouverture du fichier: ");
        exit(1);
    }
    contenu = NULL;
    contenu = (char*)malloc (taille_du_binaire*sizeof(char));
    if(contenu){//Si l'allocation memoire est réussie
        fread(contenu, sizeof(char), taille_du_binaire, fichier);
    }
    fclose(binaires);

    /*printf("\nContenuuuuuu ************************\n");
    for(i=0; i<taille_du_binaire; i++){
        printf("%c", contenu[i]);
    }
    exit(1);*/

    fichier = fopen(nom_fichier, "w");
    if (fichier == NULL){           // test si le fichier existe
        perror("Erreur d'ouverture fichier");
        exit(1);
    }

    //printf("%d\n", nombre_feuille);
    //Ecrire dans le fichier compressé la longueur des binaires
    int marge = 8 - (taille_du_binaire%8);
    fwrite(&marge,sizeof(char),1,fichier);
    //fprintf(fichier, "%d\n", taille_du_binaire);
    //Ecrire dans le fichier l'entete
    creer_entete(fichier);

    char bits[9];
    int compteur = 0, octet;
    i=0;
    //Recuperer et ecrire dans le fichier 8 bits simultanement
    while(compteur < taille_du_binaire){
        if(marge >0){
            bits[i] = '0';
            marge--;
            i++;
        }
        else{
            bits[i] = contenu[compteur];
            compteur++;
            if(i == 7){
                octet = atoi(bits);
                printf("Notre octetttt %d\n", octet);
                //octet = convertirEnDecimale(octet);
                fwrite(&octet, sizeof(int), 1, fichier);
                i=0;
            }
            else{
                i++;
            }
        }
    }

    fclose(fichier);
    free(contenu);
    free(alphabet);
}

void decompression(char *nom_fichier){
    FILE *fichier = fopen(nom_fichier,"r");
    if (fichier == NULL){
        perror("Erreur d'ouverture fichier: ");
        exit(1);
	}
    char caract, buffer;
    fread(&caract, sizeof(char),1,fichier);
    int marge = (int) caract;
    fread(&nombre_feuille,sizeof(char),1,fichier);

    printf("La marge avant decompression: %d\n", marge);
    printf("Le nombre de feuille avant decompression: %d\n",nombre_feuille);

    //Lecture de l'entete
    int code, bits, compteur=0;
    //nombre_feuille = _nombre_feuille;
    while(compteur < nombre_feuille){
        /*fwrite(&alphabet[i]->c, sizeof(char), 1, fichier);
        fwrite(&alphabet[i]->code, sizeof(int), 1, fichier);
        fwrite(&alphabet[i]->bits,  sizeof(char), 1, fichier);*/
        fread(&caract, sizeof(char),1,fichier);
        fread(&code, sizeof(int),1,fichier);
        fread(&buffer, sizeof(char),1,fichier);
        bits = (int) buffer;
        alphabet[caract] = malloc(sizeof(noeud));
        alphabet[caract]->c = caract;
        alphabet[caract]->bits = bits;
        alphabet[caract]->code = code;
        alphabet[caract]->gauche = NULL;
        alphabet[caract]->droite = NULL;
        printf("Lecture caract %c:%d:%d\n ", caract, alphabet[caract]->code, bits);
        compteur++;
	}
    printf("FIN Decodage ENTETE\n");
    printf("Decodage contenu\n");

    FILE *binaires = fopen("binaires.txt","w");
    if(binaires == NULL){
        perror("Erreur d'ouverture du fichier: ");
        exit(1);
    }
    taille_du_binaire = 0;
    int i,j, digit;
    while(fread(&code, sizeof(int),1,fichier) !=0){
        digit = nombre_digit(code);
        for (i=0; i < (8-digit); i++){
            fprintf(binaires, "0");
        }
        fprintf(binaires, "%d",code);
        taille_du_binaire = taille_du_binaire+8;
    }
    fclose(binaires);

    binaires = fopen("binaires.txt","r");
    unsigned char *contenu = (char*) malloc(taille_du_binaire*sizeof(char));
    if(contenu){//Si l'allocation memoire est réussie
        fread(contenu, sizeof(char), taille_du_binaire, binaires);
    }
    fclose(binaires);
    fclose(fichier);
    printf("Rappel Marge %d Taille_du_binaire %d et taille_du_binaire \n", marge, taille_du_binaire, sizeof(contenu));
    //printf("Notre contenu: %s\n", contenu );

    fichier = fopen(nom_fichier,"w");
    if(fichier == NULL){
        perror("Erreur d'ouverture du fichier: ");
        exit(1);
    }
    int nbre_bits=1;
    code=0;
    for (i=marge; i<taille_du_binaire;i++){
        printf("%c", contenu[i]);
        if(contenu[i] =='0'){
            code = code*10 + 0;
        }
        else{
            code = code*10 + 1;
        }

        for(j=0; j<LONGUEUR_ASCII; j++){
            //On decode les caractères au fil et à mesur qu'on avance dans les binaires
            if(alphabet[j] != NULL && alphabet[j]->bits == nbre_bits && alphabet[j]->code == code){
                //Le caractère décode est enregistre dans le fichier
                fprintf(fichier,"%c",alphabet[j]->c);
                printf("Notre code: %d", code);
                nbre_bits = 0;
                code = 0;
            }
        }
        nbre_bits++;
    }
    fclose(fichier);
    free(fichier);
    free(alphabet);
    //exit(1);
   // printf("Lecture binaires en lecture 2222\n");
    //printf("\n\nLe contenu \n*****************\n\n");
    /*compteur=0;
    while((caract=fgetc(binaires)) != EOF){
        compteur++;*/
        /*if(caract =='0'){
            code = code*10 + 0;
        }
        else{
            code = code*10 + 1;
        }

        for(j=0; j<LONGUEUR_ASCII; j++){
            //On decode les caractères au fil et à mesur qu'on avance dans les binaires
            if(alphabet[j] != NULL && alphabet[j]->bits == nbre_bits && alphabet[j]->code == code){
                contenu[i] = alphabet[j]->c; //Le caractère décode est stocké dans contenu[]
                printf("Decodage %c et indice %d\n",contenu[i], i);
                i++; //Compter les codes ASCII decoder
                nbre_bits = 0;
                code = 0;
            }
        }
        nbre_bits++;*/
    //}
    /*printf("Nombre des bits lu: %d\n", compteur);
    printf("Fin Decodage\n");*/
    fclose(binaires);
    /*fclose(fichier);*/

    /*fichier = fopen(nom_fichier,"w");
    if (fichier == NULL){
        perror("Erreur d'ouverture fichier: ");
        exit(1);
    }*/

    /*printf("Fin Decodage 22\n");
    for(j = 0; j<i; j++){
        fprintf(fichier,"%c",contenu[j]);
    }*/

    //Lecture du code hexadecimale et le convertir en binaire
    /*binaires = fopen("binaires.txt","w");
    if (binaires == NULL){
        perror("Erreur d'ouverture fichier: ");
        exit(1);
    }*/
    /*printf("Lecture contenu taille_du_binaire %d\n", taille_du_binaire);
    int aux, aux_taille_du_binaire = (taille_du_binaire/8);
    int i;
    for (i = 0; i < aux_taille_du_binaire; i++){
        aux=fgetc(fichier);
        convertirEnBinaire2(binaires, aux, 8);
    }*/
    /*int rest = taille_du_binaire%8;
    aux=fgetc(fichier);
    printf("Reste de la div %d et taille_du_binaire %d\n", rest, taille_du_binaire);
    convertirEnBinaire2(binaires, aux, rest);*/

    /*while((aux=fgetc(fichier)) != EOF){
        if(aux>=0){
            if(taille_du_binaire >= 8){
                printf("Lu Hex %u\n", aux);
                convertirEnBinaire2(binaires, aux, 8);
            }
            else{
                //printf("Dans le dernier octet octet %d \n", taille_du_binaire);
                convertirEnBinaire2(binaires, aux, taille_du_binaire);
            }
            //taille_du_binaire = taille_du_binaire-8;
        }
    }*/

    /*printf("Convertir en binaires\n");*/
    //Lire le dernier caractère et sa taille en binaire vaut taille_du_binaire%8
    /*caract = fgetc(fichier);
    convertirEnBinaire(binaires, caract, (taille_du_binaire%8));*/


    /*binaires = fopen("binaires.txt","r");
    if (binaires == NULL){
        perror("Erreur d'ouverture fichier: ");
        exit(1);
    }*/

    //printf("Lecture binaires en lecture\n");
    //LONGUEUR_MAX: Longueur max supposé des caractères
    //char *contenu = malloc(LONGUEUR_MAX*sizeof(char));
    //char *contenu = (char*)malloc (LONGUEUR_MAX*sizeof(char));
}

void creer_entete(FILE *fichier){
    int i;
    //On écrit le nombre des binaires
    //write_taille_binaire(fichier);
    //On écrit le nombre de feuille unique
    fwrite(&nombre_feuille,sizeof(char),1,fichier);
    for(i=0; i<LONGUEUR_ASCII; i++){
        if(alphabet[i] != NULL){
            printf("%c:%d:%d\n",alphabet[i]->c,alphabet[i]->code,alphabet[i]->bits);
            fwrite(&alphabet[i]->c, sizeof(char), 1, fichier);
            fwrite(&alphabet[i]->code, sizeof(int), 1, fichier);
            fwrite(&alphabet[i]->bits,  sizeof(char), 1, fichier);
        }
    }
}

void lire_contenu(char *nom_fichier){
    FILE *fichier = fopen(nom_fichier,"r");
    int compteur = 0;
    if(fichier == NULL){
        perror("Probleme ouverture fichier: ");
        exit(1);
    }

    char caract;
    while((caract=fgetc(fichier)) != EOF){
        printf("%c",caract);
        compteur++;
    }
    fclose(fichier);
    free(fichier);
}


void afficherHuffman(struct noeud *sommet,int niveau){
	int i;
	//si on n'est sur une feuille
    if(sommet->droite == NULL && sommet->gauche==NULL){
        for(i=0; i<niveau; i++){
            printf("  ");
        }
        if(sommet->c == ' '){
            printf("* \n");
        }
        else{
            printf("%c \n",sommet->c);
        }
        return ; //On sort de la fonction
	}

    //si non, on continue l'affichage
	afficherHuffman(sommet->droite, niveau+1);
	for(i=0; i<niveau; i++){
        printf("  ");
	}
	printf("%d \n",sommet->occurence);
	afficherHuffman(sommet->gauche, niveau+1);
}

void convertirEnBinaire2(FILE* fichier, int nombre, int taille){
    int binaire = 0, i=1;
    int resteDivision, nbre_digit=0;
    while(nombre != 0) {
        resteDivision = nombre%2;
        nombre = nombre/2;
        binaire = binaire + (resteDivision*i);
        i = i*10;;
        nbre_digit++;
    }

    for(i=0; i<(taille-nbre_digit); i++){
        fprintf(fichier,"0");
        //printf("0");
    }

    fprintf(fichier,"%d",binaire);
    //printf("%d\n",binaire);
}

int convertirEnDecimale(long binaire){
    int aux, decimale = 0, base = 1, resteDivision;
    aux = binaire;
    while (binaire > 0){
        resteDivision = binaire % 10;
        decimale = decimale + resteDivision * base;
        binaire = binaire / 10 ;
        base = base * 2;
    }
    return decimale;
}

/*int convertirEnBinaire(long decimale){
    long binaire = 0, i=1;
    int resteDivision, nbre_digit=0;
    while(decimale != 0) {
        resteDivision = decimale%2;
        decimale = decimale/2;
        binaire = binaire + (resteDivision*i);
        i = i*10;;
        nbre_digit++;
    }
    return binaire;
}*/

/*void write_taille_binaire(FILE *fichier){
    int aux = taille_du_binaire;
    int r = aux%255;
    aux = aux - r;
    int nbre1 = aux/255;
    int nbre2 = 255;
    int nbre3 = r;
    fwrite(&nbre1, sizeof(unsigned char), 1, fichier);
    fwrite(&nbre2, sizeof(unsigned char), 1, fichier);
    fwrite(&nbre3, sizeof(unsigned char), 1, fichier);
}
*/
