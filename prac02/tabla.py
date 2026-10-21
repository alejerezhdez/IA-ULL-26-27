import pandas as pd

df = pd.read_csv("traza.csv")

g = df.groupby(["instancia", "n", "m"]).agg(
    mejor_cmax=("cmax", "min"),
    promedio_cmax=("cmax", "mean"),
    desv_estandar=("cmax", "std"),
    neh=("neh", "first"),
    ub=("ub", "first"),
    promedio_cpu=("tiempo_cpu", "mean"),
).reset_index()

g["rpd_ub"]  = (g["promedio_cmax"] - g["ub"])  / g["ub"]  * 100
g["rpd_neh"] = (g["promedio_cmax"] - g["neh"]) / g["neh"] * 100

g = g.rename(columns={"n": "ntrabajos", "m": "nmaquinas"})
g = g[["instancia", "ntrabajos", "nmaquinas", "mejor_cmax", "promedio_cmax",
       "desv_estandar", "neh", "promedio_cpu", "rpd_ub", "rpd_neh"]]

g.to_csv("resultados.csv", index=False, float_format="%.2f")
print(g)