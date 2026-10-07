#!/usr/bin/env python3
"""Calcula el progreso de la descompilacion y escribe docs/PROGRESS.md y docs/progress.json.

Lee notes/functions_*.csv (tools/export_functions.py), notes/names_*.csv y src/ (C propio).
Uso: python tools/progress.py
"""
import csv
import json
import os
import re
import datetime

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
N = lambda *p: os.path.join(ROOT, *p)

# programa -> (csv de funciones, csv de nombres, rango de codigo [lo, hi])
PROGRAMS = {
    "MAIN0.EXE (nucleo del juego)": ("functions_main0.csv", "names_main0.csv", (0x800163F4, 0x800778EF)),
    "X000.BIN (overlay AREA00)": ("functions_x000.csv", "names_x000.csv", (0x800E8028, 0x8013C0F8)),
}


def read_csv(path):
    if not os.path.exists(path):
        return []
    with open(path, newline="") as f:
        return list(csv.DictReader(f))


def count_matching_c():
    """Bytes de funciones con C 'matching' declaradas en src/ con el marcador // MATCHING <direccion> <bytes>."""
    total = 0
    src = N("src")
    if not os.path.isdir(src):
        return 0
    for dp, _, files in os.walk(src):
        for fn in files:
            if fn.endswith(".c"):
                for m in re.finditer(r"//\s*MATCHING\s+\w+\s+(\d+)", open(os.path.join(dp, fn), errors="replace").read()):
                    total += int(m.group(1))
    return total


def analyse(csv_f, names_f, rng):
    funcs = read_csv(N("notes", csv_f))
    ours = {}
    for r in read_csv(N("notes", names_f)):
        ours[int(r["address"], 16)] = r
    d = dict(lib=0, named=0, unnamed=0, typed=0, n_lib=0, n_named=0, n_unnamed=0, n_typed=0,
             excluded=0, n_excluded=0, unnamed_list=[])
    for r in funcs:
        a, size = int(r["address"], 16), int(r["size"])
        if not (rng[0] <= a <= rng[1]):
            d["excluded"] += size
            d["n_excluded"] += 1
            continue
        mine = ours.get(a)
        is_fun = r["name"].startswith(("FUN_", "thunk_FUN_", "func_"))
        if mine is not None and "Psy-Q" in mine.get("comment", ""):
            d["lib"] += size; d["n_lib"] += 1
        elif mine is not None:
            d["named"] += size; d["n_named"] += 1
            if r["typed"] == "1": d["typed"] += size; d["n_typed"] += 1
        elif not is_fun:
            d["lib"] += size; d["n_lib"] += 1          # nombre dado por Ghidra (firma de libreria)
        else:
            d["unnamed"] += size; d["n_unnamed"] += 1
            d["unnamed_list"].append((size, r["address"]))
            if r["typed"] == "1": d["typed"] += size; d["n_typed"] += 1
    d["game"] = d["named"] + d["unnamed"]
    d["n_game"] = d["n_named"] + d["n_unnamed"]
    d["unnamed_list"].sort(reverse=True)
    return d


def pct(a, b):
    return 100.0 * a / b if b else 0.0


def bar(p, w=24):
    k = int(round(p / 100 * w))
    return "[" + "#" * k + "." * (w - k) + "]"


def main():
    res = {k: analyse(*v) for k, v in PROGRAMS.items()}
    tot = {key: sum(r[key] for r in res.values()) for key in
           ("lib", "named", "unnamed", "typed", "game", "n_game", "n_named", "n_unnamed", "n_typed", "n_lib", "excluded")}
    matching = count_matching_c()
    levels = [
        ("Funciones de juego con nombre propio", tot["named"], "bytes"),
        ("Funciones de juego con tipos (TObj)", tot["typed"], "bytes"),
        ("C 'matching' en src/", matching, "bytes"),
    ]
    out = []
    w = out.append
    w("# Progreso de la descompilacion de Tombi! (PAL espanol, SCES_013.31)\n")
    w("_Generado con `python tools/progress.py` (%s). No editar a mano._\n" % datetime.date.today().isoformat())
    w("## Resumen\n")
    w("| Nivel | Progreso | Bytes | Funciones |")
    w("|---|---|---|---|")
    w("| **C que coincide byte a byte (matching)** | **%.1f %%** `%s` | %d / %d | 0 |" % (pct(matching, tot["game"]), bar(pct(matching, tot["game"])), matching, tot["game"]))
    w("| Nombradas por nosotros | %.1f %% `%s` | %d / %d | %d / %d |" % (pct(tot["named"], tot["game"]), bar(pct(tot["named"], tot["game"])), tot["named"], tot["game"], tot["n_named"], tot["n_game"]))
    w("| Con la estructura TObj aplicada (cobertura, no es avance de C) | %.1f %% `%s` | %d / %d | %d / %d |" % (pct(tot["typed"], tot["game"]), bar(pct(tot["typed"], tot["game"])), tot["typed"], tot["game"], tot["n_typed"], tot["n_game"]))
    w("")
    w("\"Codigo de juego\" = funciones dentro del codigo de los programas analizados, **sin** contar las de la")
    w("libreria de Sony (Psy-Q), que Ghidra ya identifica. Esas son %d bytes (%d funciones) aparte.\n" % (tot["lib"], tot["n_lib"]))
    w("## Desglose por programa\n")
    w("| Programa | Codigo de juego | Nombrado | Tipado (TObj) | Matching | Libreria Psy-Q |")
    w("|---|---|---|---|---|---|")
    for k, r in res.items():
        w("| %s | %d B (%d f) | %.1f %% | %.1f %% | 0.0 %% | %d B |" % (
            k, r["game"], r["n_game"], pct(r["named"], r["game"]), pct(r["typed"], r["game"]), r["lib"]))
    w("")
    w("## Que NO esta contado (el denominador real es mayor)\n")
    w("- `MAIN1..8.EXE`: comparten ~98 %% del codigo (`.text`) con MAIN0; se tratan como variantes, no se suman.")
    w("- `SCES_013.31` (cargador, 651 KB): casi todo es libreria Psy-Q; sin analizar.")
    w("- Resto de overlays `X*.BIN` de las 20 areas (solo se ha analizado `AREA00/X000.BIN`).")
    w("- Funciones que solo llama un overlay y que Ghidra no reconoce, y overlays aun sin identificar")
    w("  (172 destinos de MAIN0 en `0x800E8000+` no caen en X000).")
    w("- Funciones fuera del codigo (`%d` bytes de falsos positivos de Ghidra en RAM sin contenido)." % tot["excluded"])
    w("")
    w("## Funciones sin nombrar mas grandes (siguientes objetivos)\n")
    allun = []
    for k, r in res.items():
        for size, addr in r["unnamed_list"]:
            allun.append((size, addr, k.split()[0]))
    allun.sort(reverse=True)
    w("| Tamano | Direccion | Programa |")
    w("|---|---|---|")
    for size, addr, k in allun[:15]:
        w("| %d B | `%s` | %s |" % (size, addr, k))
    w("")
    w("## Como se mide\n")
    w("- Unidad: bytes de codigo por funcion (el tamano que da Ghidra).")
    w("- *Nombrada*: esta en `notes/names_*.csv` y no es de libreria. *Tipada*: su primer parametro es `TObj *`.")
    w("- *Matching*: funciones en `src/*.c` marcadas con `// MATCHING <direccion> <bytes>`; hoy `src/` no existe,")
    w("  asi que es 0 %. Todavia no hay toolchain para recompilar y comparar con el original.")
    w("- Las cifras de tipado salen del ultimo export de Ghidra (`notes/functions_*.csv`) y pueden ir por detras.")
    open(N("docs", "PROGRESS.md"), "w").write("\n".join(out) + "\n")
    json.dump({"date": datetime.date.today().isoformat(), "totals": {k: v for k, v in tot.items()},
               "matching_bytes": matching,
               "programs": {k: {x: y for x, y in r.items() if x != "unnamed_list"} for k, r in res.items()}},
              open(N("docs", "progress.json"), "w"), indent=2)
    print("\n".join(out[:16]))


if __name__ == "__main__":
    main()
