import os
import pandas as pd

# Instancias usadas para ajustar los parámetros (T5): se excluyen de la tabla final
AJUSTE = ["tai20_10_9", "tai50_20_9", "tai100_10_9", "tai200_20_9"]


def cargar(fichero):
    df = pd.read_csv(fichero)
    df["rpd"] = (df["cmax"] - df["ub"]) / df["ub"] * 100
    return df


df = cargar("traza.csv")

# ---- Tabla por instancia (como antes) ----
g = df.groupby(["instancia", "n", "m"]).agg(
    mejor_cmax=("cmax", "min"),
    promedio_cmax=("cmax", "mean"),
    desv_estandar=("cmax", "std"),
    neh=("neh", "first"),
    ub=("ub", "first"),
    promedio_cpu=("tiempo_cpu", "mean"),
).reset_index()

g["rpd_ub"] = (g["promedio_cmax"] - g["ub"]) / g["ub"] * 100
g["rpd_neh"] = (g["promedio_cmax"] - g["neh"]) / g["neh"] * 100

g = g.rename(columns={"n": "ntrabajos", "m": "nmaquinas"})
g = g[["instancia", "ntrabajos", "nmaquinas", "mejor_cmax", "promedio_cmax",
       "desv_estandar", "neh", "promedio_cpu", "rpd_ub", "rpd_neh"]]
g.to_csv("resultados.csv", index=False, float_format="%.2f")

# ---- Tabla por grupo (T6a): RPD medio respecto al UB, por algoritmo ----
ev = df[~df["instancia"].isin(AJUSTE)]
tabla = ev.groupby(["n", "m"]).agg(AG=("rpd", "mean")).reset_index()

# NEH: un valor por instancia
neh = ev.drop_duplicates("instancia").copy()
neh["rpd"] = (neh["neh"] - neh["ub"]) / neh["ub"] * 100
tabla = tabla.merge(neh.groupby(["n", "m"]).agg(NEH=("rpd", "mean")).reset_index(), on=["n", "m"])

# Búsqueda aleatoria (si existe su fichero)
if os.path.exists("traza_aleatoria.csv"):
    al = cargar("traza_aleatoria.csv")
    al = al[~al["instancia"].isin(AJUSTE)]
    tabla = tabla.merge(al.groupby(["n", "m"]).agg(Aleatoria=("rpd", "mean")).reset_index(), on=["n", "m"])

cols = ["n", "m"] + [c for c in ["Aleatoria", "NEH", "AG"] if c in tabla.columns]
tabla = tabla[cols].sort_values(["n", "m"])
media = {c: tabla[c].mean() for c in cols[2:]}
tabla.to_csv("resultados_grupos.csv", index=False, float_format="%.2f")

print("RPD medio (%) respecto al UB, por grupo n x m")
print(tabla.to_string(index=False, float_format=lambda x: f"{x:.2f}"))
print("Media de los grupos:", {k: round(v, 2) for k, v in media.items()})