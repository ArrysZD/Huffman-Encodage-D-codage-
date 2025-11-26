#include "arbrebin.h"
#include "bfile.h"
#include "fap.h"
#include "huffman_code.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int tab[256] ;
} TableOcc_t;

struct code_char HuffmanCode[256];

void ConstruireTableOcc(FILE *fichier, TableOcc_t *TableOcc) {

    int c;
    for (int i = 0; i < 256; i++) {
        TableOcc->tab[i] = 0;
    }

    c = fgetc(fichier);

    while (c != EOF) {
        unsigned char uc = (unsigned char)c; // operationnel sur tout les fichiers avec accents nonASCII ... 
        TableOcc->tab[uc]++;
        c = fgetc(fichier);
    };

    int i;
    for (i = 0; i < 256; i++) {
        if (TableOcc->tab[i] != 0)
            printf("Occurences du caractere %c (code %d) : %d\n", i, i,
                   TableOcc->tab[i]);
    }
}



fap InitHuffman(TableOcc_t *TableOcc) {
    // 1) Creer une file a priorite vide
    fap F = creer_fap_vide();

    // 2) Pour chaque caractere possible (0..255)
    for (int i = 0; i < 256; i++) {
        int occ = TableOcc->tab[i];

        if (occ > 0) {
            // Creer une feuille de Huffman
            Arbre a = NouveauNoeud(ArbreVide(),(Element)i,ArbreVide());
            if (a == NULL) {
                perror("malloc");
                exit(1);
            }
            // L’inserer dans la FAP avec priorite = nb d’occurrences
            F = inserer(F, a, occ);
        }
    }

    // 3) Retourner la FAP initiale pour Huffman
    return F;
}

Arbre ConstruireArbre(fap F) {
    if (est_fap_vide(F)) {
        return NULL;
    }

    Arbre e1, e2;
    int p1, p2;

    while (1) {
        // Extraire 1er element
        F = extraire(F, &e1, &p1);

        // Si plus qu'un seul : c'est la racine
        if (est_fap_vide(F)) {
            return e1;
        }

        // Extraire 2e element
        F = extraire(F, &e2, &p2);

        // Creer un parent
        int p = p1 + p2;   // p : etiquette avec la somme des occurrences
        Arbre parent = NouveauNoeud(e1,(Element)'a',e2);
        if (!parent) {
            perror("malloc");
            exit(1);
        }

        // Reinserer dans la FAP
        F = inserer(F, parent, p);
    }
}


int EstFeuille(Arbre a){
    return !EstVide(a) && EstVide(FilsGauche(a)) && EstVide(FilsDroit(a));
}

void ConstruireCodeRec(Arbre a, int *code_courant, int longueur) {
    if (EstVide(a))
        return;

    // Si feuille → on enregistre le code du caractere
    if (EstFeuille(a)) {
        unsigned char c = (unsigned char)Etiq(a); // caractere ASCII

        HuffmanCode[c].lg = longueur;

        for (int i = 0; i < longueur; i++) {
            HuffmanCode[c].code[i] = code_courant[i];
        }

        return;
    }

    // Descente a gauche = 0
    code_courant[longueur] = 0;
    ConstruireCodeRec(FilsGauche(a), code_courant, longueur + 1);

    // Descente a droite = 1
    code_courant[longueur] = 1;
    ConstruireCodeRec(FilsDroit(a), code_courant, longueur + 1);
}

void ConstruireCode(Arbre huff) {
    int tmp[256];

    // Initialiser la table des codes
    for (int i = 0; i < 256; i++) {
        HuffmanCode[i].lg = 0;
        // HuffmanCode[i].code est un tableau, pas besoin de l’initialiser
    }

    if (EstVide(huff))
        return;

    // Cas special : un seul caractere dans le fichier
    if (EstFeuille(huff)) {
        unsigned char c = (unsigned char)Etiq(huff);
        HuffmanCode[c].lg = 1;
        HuffmanCode[c].code[0] = 0;  // on lui donne le code "0"
        return;
    }

    // Cas general
    ConstruireCodeRec(huff, tmp, 0);

    /*// TEST : afficher les codes huffman calcules
    for (int i = 0; i < 256; i++) {
        if (HuffmanCode[i].lg > 0) {
            printf("Caractere %c : code = ", i);
            for (int b = 0; b < HuffmanCode[i].lg; b++) {
                printf("%d", HuffmanCode[i].code[b]);
            }
            printf(" (lg=%d)\n", HuffmanCode[i].lg);
        }
    }*/
}

void Encoder(FILE *fic_in, FILE *fic_out, Arbre ArbreHuffman) 
{
    // 1) Ecrire l'arbre en debut de fichier 
    EcrireArbre(fic_out, ArbreHuffman);

    // Vider le buffer avant l'acces bit a bit
    fflush(fic_out); 

    // 2)  Ouvrir un acces bit a bit en ecriture 
    BFILE *bf = bstart(fic_out, "w");
    if (bf == NULL) {
        perror("bstart");
        exit(EXIT_FAILURE);
    }

    // 3) Lire le fichier a encoder 
    int c;
    while ((c = fgetc(fic_in)) != EOF) {
        unsigned char uc = (unsigned char)c;

        int lg = HuffmanCode[uc].lg;

        if (lg == 0) {
            fprintf(stderr, "Erreur : pas de code pour %d\n", uc);
            exit(EXIT_FAILURE);
        }

        // 4) ecrire chaque bit du code Huffman 
        for (int i = 0; i < lg; i++) {
            int bit = HuffmanCode[uc].code[i];   // 0 ou 1
            if (bitwrite(bf, bit) == -1) {
                perror("bitwrite");
                exit(EXIT_FAILURE);
            }
        }
    }

    // 5) Fermer proprement le flux bit a bit 
    if (bstop(bf) != 0) {
        perror("bstop");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {

    TableOcc_t TableOcc;
    FILE *fichier;
    FILE *fichier_encode;

    fichier = fopen(argv[1], "r");
    /* Construire la table d'occurences */
    ConstruireTableOcc(fichier, &TableOcc);
    fclose(fichier);

    /* Initialiser la FAP */
    fap file = InitHuffman(&TableOcc);

    /* Construire l'arbre d'Huffman */
    Arbre ArbreHuffman = ConstruireArbre(file);

        AfficherArbre(ArbreHuffman);

    /* Construire la table de codage */
    ConstruireCode(ArbreHuffman);

    /* Encodage */
    fichier = fopen(argv[1], "r");
    fichier_encode = fopen(argv[2], "w");

    Encoder(fichier, fichier_encode, ArbreHuffman);

    fclose(fichier_encode);
    fclose(fichier);

    return 0;
}