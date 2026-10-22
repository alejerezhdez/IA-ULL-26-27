# T6b: curvas de convergencia (media de 10 semillas) de AG y búsqueda aleatoria.
# Uso:  python convergencia.py      (necesita matplotlib: pip install matplotlib)
import os, shutil, subprocess
import numpy as np
import pandas as pd
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

INSTANCIAS = ["tai50_10_0", "tai200_20_0"]
N, PC, PM, TORNEO, PRESUPUESTO = 50, 0.8, 0.1, 2, 50000   # pon aquí la configuración final de la T5
SEMILLAS = range(1, 11)

EXE = next((os.path.abspath(p) for p in ["build/pfsp.exe", "build/Release/pfsp.exe",
                                         "pfsp.exe", "build/pfsp", "pfsp"] if os.path.exists(p)), None)
if EXE is None:
    raise SystemExit("No encuentro el ejecutable (build/pfsp.exe, pfsp.exe...)")

os.makedirs("convergencia", exist_ok=True)
grid = np.arange(0, PRESUPUESTO + 1, 50)


def curva(alg, inst, semilla):
    """Mejor cmax en cada punto de la rejilla (función escalonada)."""
    h = pd.read_csv(f"convergencia/conv_{alg}_{inst}_{semilla}.csv")
    idx = np.clip(np.searchsorted(h["evaluaciones"].values, grid, side="right") - 1, 0, None)
    return h["cmax"].values[idx]


fig, ejes = plt.subplots(1, len(INSTANCIAS), figsize=(6 * len(INSTANCIAS), 4.5))
for eje, inst in zip(np.atleast_1d(ejes), INSTANCIAS):
    fichero = os.path.abspath(f"instances/{inst}.fsp")
    with open(fichero) as f:
        f.readline()
        ub = int(f.readline().split()[3])
    for alg, flag in (("ag", []), ("aleatoria", ["-r"])):
        for s in SEMILLAS:
            subprocess.run([EXE, fichero, str(N), str(PC), str(PM), str(TORNEO),
                            str(PRESUPUESTO), str(s), "1", "-c"] + flag,
                           cwd="convergencia", check=True, capture_output=True)
        media = np.mean([curva(alg, inst, s) for s in SEMILLAS], axis=0)
        eje.plot(grid, media, label="AG" if alg == "ag" else "Búsqueda aleatoria")
    neh = pd.read_csv("convergencia/traza.csv").query("instancia == @inst")["neh"].iloc[0]
    eje.axhline(neh, color="gray", linestyle="--", label="NEH")
    eje.axhline(ub, color="green", linestyle=":", label="UB")
    eje.set_title(f"{inst} (media de {len(SEMILLAS)} ejecuciones)")
    eje.set_xlabel("Evaluaciones del makespan")
    eje.set_ylabel("Mejor makespan")
    eje.legend()
plt.tight_layout()
plt.savefig("convergencia.png", dpi=150)
shutil.rmtree("convergencia")      # borra la carpeta con los csv temporales
print("Guardado convergencia.png")