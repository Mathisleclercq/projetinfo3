#!/bin/bash

INPUT_FILE="c-wildwater_v3.dat"
OUTPUT_FILE="usines_volumes.dat"

[ -f "$INPUT_FILE" ] || exit 1

echo "plant_id;max_volume(k.m3.year-1)" > "$OUTPUT_FILE"

awk -F';' '
{
  if ($1 == "-" && $2 != "-" && $3 == "-" && $4 != "-" && $5 == "-") {
    print $2 ";" $4
  }
}
' "$INPUT_FILE" >> "$OUTPUT_FILE"