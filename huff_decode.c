#include <stdio.h>
#include "arbrebin.h"
#include "assert.h"
#include "bfile.h"

 int EstFeuille(Arbre a) {
    return !EstVide(a) &&
           EstVide(FilsGauche(a)) &&
           EstVide(FilsDroit(a));
}

void Decoder(FILE *fichier_encode, FILE *fichier_decode, Arbre ArbreHuffman) {

    /* ============================================================
       CAS 0 : fichier vide → ArbreHuffman vide
       ============================================================ */
    if (EstVide(ArbreHuffman)) {
        // rien à décoder
        return;
    }

    /* ============================================================
       CAS 1 : un seul caractère distinct → l'arbre est une feuille
       ============================================================ */
    if (EstFeuille(ArbreHuffman)) {

        // Ouvrir la lecture bit à bit
        BFILE *b = bstart(fichier_encode, "r");
        if (b == NULL) {
            perror("bstart");
            return;
        }

        int bit;
        while ((bit = bitread(b)) != -1) {
            // Chaque bit correspond à UNE occurrence du caractère unique
            fputc(Etiq(ArbreHuffman), fichier_decode);
        }

        bstop(b);
        return;
    }

    /* ============================================================
       CAS GENERAL : arbre avec au moins 2 feuilles
       ============================================================ */

    // Ouverture en lecture bit à bit
    BFILE *b = bstart(fichier_encode, "r");
    if (b == NULL) {
        perror("bstart");
        return;
    }

    Arbre courant = ArbreHuffman;
    int bit;

    // On lit les bits du fichier compressé
    while ((bit = bitread(b)) != -1) {

        // Descente dans l'arbre
        if (bit == 0)
            courant = FilsGauche(courant);
        else
            courant = FilsDroit(courant);

        // Quand on atteint une feuille : on décode un caractère
        if (EstFeuille(courant)) {
            fputc(Etiq(courant), fichier_decode);
            courant = ArbreHuffman; // on repart de la racine
        }
    }

    // Fin du flux binaire
    bstop(b);
}


int main(int argc, char *argv[]) {
    FILE *fichier_encode = fopen(argv[1], "r");
    FILE *fichier_decode = fopen(argv[2], "w");

    Arbre ArbreHuffman = LireArbre(fichier_encode);
    Decoder(fichier_encode, fichier_decode, ArbreHuffman);
                AfficherArbre(ArbreHuffman);

    fclose(fichier_encode);
    fclose(fichier_decode);
    
    return 0;
}
