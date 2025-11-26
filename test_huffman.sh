#!/bin/bash

# Vérification des arguments
if [ $# -ne 1 ]; then
    echo "Usage: $0 fichier_a_tester"
    exit 1
fi

INPUT="$1"
ENC="$INPUT.huff"
OUT="$INPUT.out"


#encode
./huff_encode "$INPUT"  "$ENC"

#decode
./huff_decode "$ENC" "$OUT"

echo "------- Test sur $INPUT --------"

#Verifier si les fichier sont identiques
if diff "$INPUT" "$OUT" > /dev/null; then
    echo "Le décodage est correct."
else
    echo "!!! Le fichier décode est différent !!!"
fi

# Affichage de tailles
ORIG=$(stat -c%s "$INPUT")
COMP=$(stat -c%s "$ENC")

echo "Taille originale  : $ORIG octets"
echo "Taille compressée : $COMP octets"
