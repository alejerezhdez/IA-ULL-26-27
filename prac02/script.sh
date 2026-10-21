#!/bin/bash
EXE=./build/pfsp     # cambia por el nombre real de tu ejecutable
DIR=instances          # carpeta con los ficheros de Taillard

if [ ! -x "$EXE" ]; then echo "No existe el ejecutable $EXE"; exit 1; fi

shopt -s nullglob
ficheros=("$DIR"/*.fsp)
if [ ${#ficheros[@]} -eq 0 ]; then echo "No hay ficheros .txt en $DIR"; exit 1; fi

rm -f resultados.csv
for f in "${ficheros[@]}"; do
  for s in $(seq 1 10); do
    "$EXE" "$f" 50 0.8 0.1 2 50000 "$s" 1 || { echo "Fallo en $f con semilla $s"; exit 1; }
  done
done

python3 tabla.py