#!/bin/bash
EXE=./build/pfsp
DIR=instances/pruebas

if [ ! -x "$EXE" ]; then echo "No existe el ejecutable $EXE"; exit 1; fi

shopt -s nullglob
ficheros=("$DIR"/*.fsp)
if [ ${#ficheros[@]} -eq 0 ]; then echo "No hay ficheros .fsp en $DIR"; exit 1; fi

rm -f traza.csv traza_aleatoria.csv resultados.csv

# AG (como antes)
for f in "${ficheros[@]}"; do
  for s in $(seq 1 10); do
    "$EXE" "$f" 50 0.8 0.4 2 50000 "$s" 1 || { echo "Fallo en $f con semilla $s"; exit 1; }
  done
done

# Búsqueda aleatoria (nuevo, con -r al final)
for f in "${ficheros[@]}"; do
  for s in $(seq 1 10); do
    "$EXE" "$f" 50 0.8 0.4 2 50000 "$s" 1 -r || { echo "Fallo en $f con semilla $s"; exit 1; }
  done
done

python3 tabla.py