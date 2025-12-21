Projet C-Wildwater

Ce projet analyse la distribution d’eau dans un réseau hydraulique.
Il permet de :

filtrer les données brutes

générer trois histogrammes
• histogramme des sources (histo_src)
• histogramme des valeurs maximales (histo_max)
• histogramme réel (histo_real)

calculer les pertes d’eau (fuites) d’une usine spécifique

Le projet repose sur :
• des scripts Shell pour le pré-traitement
• un programme C utilisant des arbres AVL
• Gnuplot pour la génération d’histogrammes


Prérequis

Pour exécuter le projet, vous devez disposer de :

Logiciels nécessaires
• GCC → compiler les programmes C
• Make → exécuter le Makefile
• Bash → scripts Shell
• Gnuplot → histogrammes

Fichiers présents dans le même dossier
• Makefile
• Cwildwater.sh
• leaks.c
• avl.c
• c-wildwater_v3.dat
• Scripts Shell :
• extract_sources.sh
• extract_usines.sh
• histo_src.sh
• histo_max.sh
• histo_real.sh


Installation et fonctionnement

Compiler automatiquement les programmes nécessaires

Dans le terminal, à la racine du projet :

make

Cela :
• exécute les scripts Shell d’extraction
• compile avl.c
• génère vol_captation et vol_traitement
• prépare les données pour les histogrammes

Rendre les scripts exécutables

chmod +x Cwildwater.sh

Génération des histogrammes

Trois scripts permettent de créer des histogrammes :

Script Donnée analysée
histo_src.sh histogramme des sources
histo_max.sh histogramme des maxima
histo_real.sh histogramme réel

Chacun s’appuie sur un fichier texte généré par le Makefile.

Calcul des fuites d’une usine

Le fichier leaks.c permet de calculer le volume total des pertes en eau d’une usine.

Compilation du programme de fuite

Ce programme n’est pas compilé automatiquement par le Makefile.
Il faut l’exécuter manuellement :

gcc -o leaks leaks.c

Utilisation

./Cwildwater.sh c-wildwater_v3.dat leaks "nom_de_l_usine"

Exemple concret :

./Cwildwater.sh c-wildwater_v3.dat leaks "PLANT #DH200396E"

Le résultat sera écrit dans un fichier texte comme vol_fuite.txt.

Exemple d’exécution complète

make
chmod +x Cwildwater.sh
./Cwildwater.sh c-wildwater_v3.dat leaks "PLANT #TU200233Q"

Sortie attendue (exemple simplifié) :

PLANT #TU200233Q : 36389.74

Nettoyage

Pour supprimer les fichiers générés :

make clean

Auteurs
• Thimothee Aubree
• Mathis Leclercq
