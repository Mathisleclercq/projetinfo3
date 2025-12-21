#!/bin/bash
FICHIER_ENTREE="vol_traitement.tmp"
FICHIER_TRIE="_histo_traitement_trie.tmp"
USINES_50_PLUS_PETITS="_histo_traitement_50_plus_petits.tmp"
USINES_10_PLUS_GRANDS="_histo_traitement_10_plus_grands.tmp"
# Vérifier l'existence du fichier d'entrée
[ -f "$FICHIER_ENTREE" ] || exit 1
# Trier les usines par volume de traitement (colonne 2)
tail -n +2 "$FICHIER_ENTREE" \
  | sort -t';' -k2,2n \
  > "$FICHIER_TRIE"
# Extraire les 50 plus petites et 10 plus grandes usines
head -n 50 "$FICHIER_TRIE" > "$USINES_50_PLUS_PETITS"
tail -n 10 "$FICHIER_TRIE" | tac > "$USINES_10_PLUS_GRANDS"
# Générer les histogrammes avec gnuplot
gnuplot << EOF
set terminal pngcairo size 1600,700
set datafile separator ";"
set style data histograms
set style fill solid 1.0 border -1
set boxwidth 0.8
set xlabel "Identifiants des usines"
set ylabel "Volume de traitement (M.m³.an⁻¹)"
set yrange [0:*]
set xtics rotate by -90 font ",8"
# Histogramme des 50 plus petites usines
set output "histo_traitement_50_usines_plus_petits_volumes.png"
set title "Volumes de traitement - 50 plus faibles"
plot "$USINES_50_PLUS_PETITS" using (\$2/1000):xtic(1) with boxes notitle linecolor rgb "#FF9800"
# Histogramme des 10 plus grandes usines
set output "histo_traitement_10_usines_plus_grands_volumes.png"
set title "Volumes de traitement - 10 plus élevés"
plot "$USINES_10_PLUS_GRANDS" using (\$2/1000):xtic(1) with boxes notitle linecolor rgb "#9C27B0"
EOF
# Nettoyer les fichiers temporaires
rm -f "$FICHIER_TRIE" \
      "$USINES_50_PLUS_PETITS" \
      "$USINES_10_PLUS_GRANDS"
echo "Histogrammes générés avec succès !"
echo "  - histo_traitement_50_usines_plus_petits_volumes.png"
echo "  - histo_traitement_10_usines_plus_grands_volumes.png"
