# T5: ajuste de parámetros sobre instancias DISTINTAS de las de evaluación.
# Uso:  python ajuste.py
import itertools, os, subprocess, tempfile

INSTANCIAS = ["tai20_10_9", "tai50_20_9", "tai100_10_9", "tai200_20_9"]
VALORES_N = [50, 100]
VALORES_PM = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]
PC, TORNEO, PRESUPUESTO = 0.8, 2, 50000
SEMILLAS = range(1, 11)         # 10 ejecuciones por configuración

EXE = next((os.path.abspath(p) for p in ["build/pfsp.exe", "build/Release/pfsp.exe",
                                         "pfsp.exe", "build/pfsp", "pfsp"] if os.path.exists(p)), None)
if EXE is None:
    raise SystemExit("No encuentro el ejecutable (build/pfsp.exe, pfsp.exe...)")


def ub_de(fichero):
    with open(fichero) as f:
        f.readline()
        return int(f.readline().split()[3])


def rpd_medio(n_pob, pc, pm, neh=1):
    """RPD medio (%) respecto al UB sobre las instancias de ajuste y las semillas."""
    total, cuenta = 0.0, 0
    with tempfile.TemporaryDirectory() as tmp:       # los csv que escribe pfsp van aquí
        for inst in INSTANCIAS:
            fichero = os.path.abspath(f"instances/{inst}.fsp")
            ub = ub_de(fichero)
            for s in SEMILLAS:
                out = subprocess.run([EXE, fichero, str(n_pob), str(pc), str(pm), str(TORNEO),
                                      str(PRESUPUESTO), str(s), str(neh)],
                                     cwd=tmp, capture_output=True, text=True, check=True).stdout
                cmax = int(out.split()[-1])
                total += 100 * (cmax - ub) / ub
                cuenta += 1
    return total / cuenta


print(f"Ajuste sobre {INSTANCIAS}, {len(SEMILLAS)} semillas, pc={PC}, torneo={TORNEO}")
print("\nN     pm     RPD medio (%)")
resultados = {}
for n_pob, pm in itertools.product(VALORES_N, VALORES_PM):
    resultados[(n_pob, pm)] = rpd_medio(n_pob, PC, pm)
    print(f"{n_pob:<5} {pm:<6} {resultados[(n_pob, pm)]:.3f}", flush=True)

mejor_n, mejor_pm = min(resultados, key=resultados.get)
print(f"\nMejor configuración: N={mejor_n}, pm={mejor_pm}")

# Ablación (para el análisis T7): qué pasa si se quita un componente
print("\nAblación con la mejor configuración (RPD medio %):")
print(f"  AG completo:             {resultados[(mejor_n, mejor_pm)]:.3f}")
print(f"  sin NEH en la población: {rpd_medio(mejor_n, PC, mejor_pm, neh=0):.3f}")
print(f"  sin mutación (pm=0):     {rpd_medio(mejor_n, PC, 0.0):.3f}")
print(f"  sin cruce (pc=0):        {rpd_medio(mejor_n, 0.0, mejor_pm):.3f}")