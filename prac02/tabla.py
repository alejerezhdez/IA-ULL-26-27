import glob, os, subprocess, csv, sys
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor
from statistics import mean

EXE = "./programa"
INSTANCIAS = sorted(glob.glob("instancias/*.txt"))
SEMILLAS = range(1, 11)
N, PC, PM, TORNEO, PRESUPUESTO = 50, 0.8, 0.1, 2, 50000
WORKERS = 2

# (nombre en la tabla, algoritmo, use_neh, semillas)
ALGORITMOS = [
    ("AG",          "ga",     1, SEMILLAS),
    ("AG sin NEH",  "ga",     0, SEMILLAS),
    ("Aleatoria",   "random", 0, SEMILLAS),
    ("NEH",         "neh",    1, [1]),          # determinista: una sola ejecución
]

def tamano(fichero):
    # Busca la primera línea con solo enteros y >= 2 valores (n m ...)
    with open(fichero) as f:
        for linea in f:
            partes = linea.split()
            if len(partes) >= 2 and all(p.isdigit() for p in partes):
                return int(partes[0]), int(partes[1])
    raise ValueError(f"No se pudo leer n y m de {fichero}")

def ejecutar(tarea):
    etiqueta, alg, neh, fichero, semilla = tarea
    salida = subprocess.run(
        [EXE, fichero, str(N), str(PC), str(PM), str(TORNEO), str(PRESUPUESTO),
         str(semilla), str(neh), alg],
        capture_output=True, text=True).stdout.strip().splitlines()[-1].split()
    nombre, sem, cmax, rpd = salida[0], int(salida[1]), int(salida[2]), float(salida[3])
    return etiqueta, fichero, sem, cmax, rpd

tareas = [(e, a, u, f, s) for e, a, u, ss in ALGORITMOS for f in INSTANCIAS for s in ss]
print(f"{len(tareas)} ejecuciones...", file=sys.stderr)

with ThreadPoolExecutor(WORKERS) as pool:
    resultados = list(pool.map(ejecutar, tareas))

# Resultados brutos (hay que entregarlos)
with open("resultados_brutos.csv", "w", newline="") as f:
    w = csv.writer(f)
    w.writerow(["algoritmo", "instancia", "semilla", "cmax", "rpd"])
    for e, fich, sem, cmax, rpd in resultados:
        w.writerow([e, os.path.basename(fich), sem, cmax, f"{rpd:.4f}"])

# RPD medio por instancia (sobre semillas) y luego por grupo n x m
por_instancia = defaultdict(list)
for e, fich, sem, cmax, rpd in resultados:
    por_instancia[(e, fich)].append(rpd)

por_grupo = defaultdict(list)
for (e, fich), rpds in por_instancia.items():
    por_grupo[(e, tamano(fich))].append(mean(rpds))

grupos = sorted({g for (_, g) in por_grupo})
etiquetas = [a[0] for a in ALGORITMOS]

with open("tabla_rpd.csv", "w", newline="") as f:
    w = csv.writer(f)
    w.writerow(["n x m"] + etiquetas)
    for g in grupos:
        w.writerow([f"{g[0]}x{g[1]}"] + [f"{mean(por_grupo[(e, g)]):.3f}" for e in etiquetas])
    w.writerow(["Media"] + [f"{mean(mean(por_grupo[(e, g)]) for g in grupos):.3f}" for e in etiquetas])

print("| n x m | " + " | ".join(etiquetas) + " |")
print("|---" * (len(etiquetas) + 1) + "|")
for g in grupos:
    print(f"| {g[0]}x{g[1]} | " + " | ".join(f"{mean(por_grupo[(e, g)]):.3f}" for e in etiquetas) + " |")
print("| Media | " + " | ".join(f"{mean(mean(por_grupo[(e, g)]) for g in grupos):.3f}" for e in etiquetas) + " |")