#!/bin/bash
DEBUT=$(date +%s%3N)

# Vérification des arguments
if [ $# -lt 2 ]; then
    echo "Erreur: nombre d'arguments insuffisant"
    echo "Usage: $0 <fichier.csv> <commande> [arguments...]"
    exit 1
fi

FICHIER_DATA="$1"
COMMANDE="$2"

if [ ! -f "$FICHIER_DATA" ]; then
    echo "Erreur: le fichier '$FICHIER_DATA' n'existe pas"
    exit 1
fi

# Commande 'leaks'
if [ "$COMMANDE" = "leaks" ]; then
    
    if [ $# -ne 3 ]; then
        echo "Erreur: la commande 'leaks' nécessite un identifiant d'usine"
        echo "Usage: $0 <fichier.csv> leaks \"<identifiant_usine>\""
        exit 1
    fi
    
    IDENTIFIANT_USINE="$3"
    FICHIER_SORTIE="test.txt"
    FICHIER_VOLUMES="vol_traitement.tmp"
    
    # Vérification de l'existence de l'usine dans le fichier
    if ! grep -q "$IDENTIFIANT_USINE" "$FICHIER_DATA"; then
        echo "Erreur: l'usine '$IDENTIFIANT_USINE' n'existe pas dans le fichier"
        exit 1
    fi
    
    # Recherche du volume dans vol_traitement.tmp
    VOLUME=$(grep "$IDENTIFIANT_USINE" "$FICHIER_VOLUMES" 2>/dev/null | cut -d';' -f2)
    
    # Création de l'en-tête (UNE SEULE ligne avec -;-)
    echo "$IDENTIFIANT_USINE;$VOLUME" > "$FICHIER_SORTIE"
    
    # Filtrage rapide avec awk (ignorer les lignes avec "-")
    awk -F';' -v usine="$IDENTIFIANT_USINE" '
        ($1 == usine || $2 == usine) && !($1 == "-" && $3 == "-" && $5 == "-") {
            print usine ";" $3 ";" $5
        }
    ' "$FICHIER_DATA" >> "$FICHIER_SORTIE"
    
    echo "Le fichier de résultats a été généré : $FICHIER_SORTIE"
    
    # Compilation et exécution de leaks.c
    if [ -f "leaks.c" ]; then
        echo "Compilation de leaks.c..."
        gcc -o leaks leaks.c -lm
        
        if [ $? -eq 0 ]; then
            echo "Exécution du programme leaks..."
            ./leaks
            
            if [ -f "vol_fuite.txt" ]; then
                echo "Fichier vol_fuite.txt généré avec succès"
                cat vol_fuite.txt
            fi
        else
            echo "Erreur lors de la compilation de leaks.c"
        fi
    else
        echo "Erreur: le fichier leaks.c n'existe pas"
    fi
fi

# Durée d'exécution
FIN=$(date +%s%3N)
DUREE=$((FIN - DEBUT))
echo "Durée totale : $DUREE ms"

exit 0