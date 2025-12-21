#!/bin/bash

FICHIER_ENTREE="usines_volumes.dat"

FICHIER_TRIE="_histo_trie.tmp"
USINES_50_PLUS_PETITS="_histo_50_plus_petits.tmp"
USINES_10_PLUS_GRANDS="_histo_10_plus_grands.tmp"

[ -f "$FICHIER_ENTREE" ] || exit 1

tail -n +2 "$FICHIER_ENTREE" \
  | sort -t';' -k2,2n \
  > "$FICHIER_TRIE"

head -n 50 "$FICHIER_TRIE" > "$USINES_50_PLUS_PETITS"
tail -n 10 "$FICHIER_TRIE" | tac > "$USINES_10_PLUS_GRANDS"

gnuplot << EOF
set terminal pngcairo size 1600,700
set datafile separator ";"

set style data histograms
set style fill solid 1.0 border -1
set boxwidth 0.8

set xlabel "Identifiants des usines"
set ylabel "Volume (M.m³.an⁻¹)"
set yrange [0:*]
set xtics rotate by -90 font ",8"

set output "histo_50_usines_plus_petits_volumes.png"
set title "Volumes de traitement - 50 plus faibles"
plot "$USINES_50_PLUS_PETITS" using (\$2/1000):xtic(1) with boxes notitle

set output "histo_10_usines_plus_grands_volumes.png"
set title "Volumes de traitement - 10 plus élevés"
plot "$USINES_10_PLUS_GRANDS" using (\$2/1000):xtic(1) with boxes notitle
EOF

rm -f "$FICHIER_TRIE" \
      "$USINES_50_PLUS_PETITS" \
      "$USINES_10_PLUS_GRANDS"
echo "Histogrammes générés avec succès !"
echo "  - histo_10_usines_plus_grands_volumes.png"
echo "  - histo_50_usines_plus_petits_volumes.png"