# Práctica 2 IA: Algoritmo genético para el PFSP

## Compilación
    Desde la carpeta build ejecutar make
    cmake --build build
(o: g++ -std=c++17 -O2 -DNDEBUG -o pfsp.exe main.cpp instance.cpp genetic_algorithm.cpp random_search.cpp)
Para activar la comprobación de factibilidad (assert): -DCMAKE_BUILD_TYPE=Debug

## Ejecución
    pfsp instancia N pc pm torneo presupuesto semilla neh [-v] [-r] [-c]
- neh: 1 = incluye la solución NEH en la población inicial, 0 = no.
- -v: imprime la permutación. -r: búsqueda aleatoria en vez del AG.
- -c: guarda la curva de convergencia en conv_<alg>_<instancia>_<semilla>.csv.
Ejemplo: pfsp instances/tai50_10_0.fsp 50 0.8 0.1 2 50000 1 1

## Experimentos
- ajuste.py: ajuste de N y pm (T5) sobre tai20_10_9, tai50_20_9, tai100_10_9, tai200_20_9.
- script.sh: 10 semillas por instancia, AG y búsqueda aleatoria; genera traza.csv y
  traza_aleatoria.csv (resultados brutos, con semilla) y llama a tabla.py.
- tabla.py: genera resultados.csv (por instancia) y resultados_grupos.csv (por grupo).
  Las instancias de ajuste se excluyen de la tabla final.
- convergencia.py: genera convergencia.png.

## Decisiones de diseño
- Representación: permutación de trabajos.
- OX con relleno de izquierda a derecha (la variante descrita en el enunciado).
- Mutación: inserción o intercambio con probabilidad 0.5 cada una.
- Reemplazo generacional con elitismo.
- El presupuesto cuenta las evaluaciones del AG; las de la construcción de NEH no se cuentan.
- Dependencias de los scripts: Python 3, pandas, matplotlib.