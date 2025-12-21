#!/bin/bash

INPUT_FILE="c-wildwater_v3.dat"

ID_FILE="plants_id.dat"
VOL_FILE="sources_volumes.dat"
LEAK_FILE="sources_leaks.dat"

[ -f "$INPUT_FILE" ] || exit 1

echo "plant_id" > "$ID_FILE"
echo "capted_volume(k.m3.year-1)" > "$VOL_FILE"
echo "leak_percent" > "$LEAK_FILE"

awk -F';' -v idf="$ID_FILE" -v volf="$VOL_FILE" -v leakf="$LEAK_FILE" '
{
  if ($1 == "-" &&
      $2 != "-" &&
      $3 != "-" &&
      $4 != "-" &&
      $5 != "-") {

    print $3 >> idf
    print $4 >> volf
    print $5 >> leakf
  }
}
' "$INPUT_FILE"