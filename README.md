# **README – APNÉE 3 : Codage & Décodage Huffman**

## 🎯 **Objectif du projet**

Cette APNÉE consiste à **implémenter un codeur/décodeur Huffman complet**, permettant de :

* Lire un fichier texte
* Construire la table d’occurrences
* Générer l’arbre de Huffman
* Produire la table des codes
* Encoder le fichier bit à bit
* Décoder un fichier `.huff` pour retrouver exactement le fichier original

L’objectif final est d’**évaluer l’efficacité du codage de Huffman** selon les fichiers testés.

---

# 📁 **Structure du projet**

```
.
├── huff_encode.c      # Codeur à compléter (fait)
├── huff_decode.c      # Décodeur à compléter (fait)
├── arbrebin.c/.h      # Module d’arbres binaires fourni
├── fap.c/.h           # File à priorité fournie
├── bfile.c/.h         # Lecture/écriture bit à bit
├── Makefile
├── tests/             # Fichiers de tests divers
└── test_huffman.sh    # Script d’automatisation des tests
```

---

# 🚀 **Compilation**

```bash
make
```

Génère :

* **huff_encode**
* **huff_decode**

---

# 🔧 **Utilisation**

## **Encodage**

```bash
./huff_encode fichier.txt fichier.huff
```

## **Décodage**

```bash
./huff_decode fichier.huff fichier.out
```

## **Vérification**

```bash
diff fichier.txt fichier.out
```

Si rien ne s’affiche : ✔ fichiers identiques.

---

# 🧠 **Fonctionnalités implémentées**

### ✔ 1. Construction de la table d’occurrences

Lecture caractère par caractère (via `fgetc`) et remplissage du tableau de 256 occurrences
→ conforme à l’énoncé .

### ✔ 2. Construction de l’arbre de Huffman

Utilisation de la file à priorités (`fap.c`), fusion des deux plus petites priorités jusqu’à obtenir la racine.

### ✔ 3. Génération de la table des codes

Parcours récursif gauche/droite :

* gauche → 0
* droite → 1

Stockage dans `HuffmanCode[i]`.

### ✔ 4. Encodage bit à bit

Écriture de l’arbre + écriture du texte encodé via `bitwrite`.

### ✔ 5. Décodage

Reconstruction de l’arbre avec `LireArbre`, puis navigation bit à bit jusqu’aux feuilles.

### ✔ 6. Validation automatique

Script Bash :

```bash
./test_huffman.sh fichier
```

Exécute encodage → décodage → vérification → mesure de taille.

---

# 🧪 **Batterie de tests**

Les tests ont été réalisés sur divers fichiers (texte long, fichiers courts, répétitions, fichiers uniformes, images, PDF), conformément aux recommandations de l’énoncé .

| Fichier        | Nature               | Taille originale | Taille compressée | Validation | Compression utile ? |
| -------------- | -------------------- | ---------------- | ----------------- | ---------- | ------------------- |
| Candide.txt    | Gros texte           | 201 Ko           | 115 Ko            | ✔          | ✔                   |
| Gargantua.txt  | Gros texte           | 270 Ko           | 154 Ko            | ✔          | ✔                   |
| huff_encode.c  | Code source          | 5,8 Ko           | 5,3 Ko            | ✔          | ✔                   |
| repetition.txt | motif ABAB…          | 200 o            | 67 o              | ✔          | ✔                   |
| test.txt       | petit fichier        | 7 o              | 92 o              | ✔          | ✘                   |
| multiple.txt   | “aaaaa”              | 5 o              | 19 o              | ✔          | ✘                   |
| uniform.txt    | caractères distincts | 107 o            | 2434 o            | ✔          | ✘                   |
| img.png        | image                | 98 Ko            | 105 Ko            | ✔          | ✘                   |
| test_pdf       | PDF                  | 1,56 Mo          | 1,57 Mo           | ✔          | ✘                   |
| vide.txt       | 0 octet              | 0 o              | 6 o               | ✔          | ✘                   |

### 🔍 **Bilan**

* **10/10 fichiers correctement décodés** → programme correct & robuste ✔
* **Compression efficace sur 40% des fichiers**, en cohérence avec la théorie Huffman
  (répartition inégale, gros fichiers, beaucoup de redondance)

---

# 📊 **Analyse**

Selon vos résultats  :

* Huffman **compresse très bien** :

  * gros fichiers textuels
  * distribution inégale
  * motifs répétitifs

* Huffman **est inefficace** :

  * petits fichiers (overhead de l’arbre)
  * répartition uniforme
  * fichiers déjà compressés (PNG, PDF, ZIP…)

Ces observations confirment parfaitement le comportement théorique attendu.

---

# ✅ **Conclusion**

L’implémentation du codeur/décodeur Huffman est :

✔ **fonctionnelle**
✔ **fidèle à l’énoncé**
✔ **validée sur de nombreux tests**
✔ **robuste même sur les cas particuliers**

Le programme reconstruit toujours le fichier original sans perte, et son efficacité dépend logiquement de la structure statistique du fichier en entrée.

participant : Zaidi Arris et [@Lydiacmd]((https://github.com/lydiacmd))
